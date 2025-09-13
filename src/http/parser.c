#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>

#define URL_PATH_PATTERN "^(/|/[a-zA-Z0-9._?-]+(/[a-zA-Z0-9._?-]+)*/?)$"
#define QUERY_PARAM_PATTERN "^[a-zA-Z0-9_]+=[a-zA-Z0-9_]+(&[a-zA-Z0-9_]+=[a-zA-Z0-9_]+)*$"

char* methods[9] = {"GET","POST","PUT","DELETE","HEAD","OPTIONS","PATCH","TRACE","CONNECT"};
char* versions[3] = {"HTTP/1.0","HTTP/1.1","HTTP/2.0"};

typedef struct Header{
	char* key;
	char* value;
} Header;

// typedef struct Headers {
// 	Header* headers;
// 	char* host;
// 	int content_length;
// 	char* content_type;
// 	char* user_agent;
// 	char* accept;
// 	char* connection;
// } Headers;

//! in the next version support multiple query parameters
typedef struct QueryParam {
	char* key;
	char* value;
} QueryParam;

typedef struct Path {
	char* path;
	QueryParam* query_params;
} Path;

typedef struct HttpRequest {
	char* method;
	char* path;
	char* query_params;
	char* version;
	Header* host;
	Header* user_agent;
	Header* content_length;
	Header* headers;
    int header_count;
	char* body;
} HttpRequest;

char* parse_http_method(char* ptr, HttpRequest* req) {
	char method[8];

	int8_t cur = 0;

	while (*ptr != ' ' && cur < 7) {
		method[cur++] = *ptr++;
	}
	ptr += 1; 
	method[cur] = '\0';

	for (int8_t j = 0; j < 9; j++) {
		if (strcmp(method, methods[j]) == 0) {
			req->method = strdup(method);
			return ptr;
		}
	}
	return NULL;
}
char* parse_http_path(char* ptr , HttpRequest* req) {
	char path[2048];
	char query_params[1024];
	
	memset(path, 0, sizeof(path));
	memset(query_params, 0, sizeof(query_params));

	int cur = 0;

	while (*ptr != ' ' && *ptr != '\r' && *ptr != '\0') {
		path[cur++] = *ptr++;
		// if (*ptr == '?') {
		// 	ptr++;
		// 	int cur2 = 0;
		// 	while (*ptr != ' ' && *ptr != '\r' && *ptr != '\0') {
		// 		query_params[cur2++] = *ptr++;
		// 	}
		// 	query_params[cur2] = '\0';
		// 	req->query_params = strdup(query_params);
		// 	break;
		// }
	}
	path[cur++] = '\0';
	
	regex_t regex;
	if (regcomp(&regex, URL_PATH_PATTERN, REG_EXTENDED) != 0) {
		printf("Could not compile regex\n");
		return NULL;
	}

	if (regexec(&regex, path, 0, NULL, 0) != 0) {
		printf("Invalid URL path\n");
		return NULL;
	}
	regfree(&regex);

	req->path = strdup(path);
	return ptr;
}
char* parse_http_version(char* ptr, HttpRequest* req) {
	char version[16];
	memset(version, 0, 16);

	int8_t cur = 0;

	while (*ptr == ' ') ptr++;
	while (*ptr != '\r' && cur < 9) {
		version[cur++] = *ptr++;
	}
	version[cur] = '\0';
	for (int8_t j = 0; j < 3; j++) {
		if (strcmp(version, versions[j]) == 0) {
			req->version = strdup(version);
			return ptr;
		}
	}
	return NULL;
}

char* parse_http_headers(char* ptr, HttpRequest* req) {
	Header* headers = malloc(sizeof(Header) * 100);
	int header_num = 0;

	if (!headers) {
		perror("malloc failed");
		return NULL;
	}
	memset(headers, 0, sizeof(Header) * 100);
	
	while (*ptr == ' ' ||*ptr == '\n'|| *ptr == '\r') ptr++;

	while (ptr[0] != '\r' && ptr[1] != '\n'){

        char key[256] = {0};
        char value[1024] = {0};

		int cur = 0;
		
		while (*ptr && *ptr != ':' && cur < 255){
			key[cur++] = *ptr++;
		}
		key[cur] = '\0';
		ptr += 2;

		cur = 0;

		while (*ptr && !(ptr[0] == '\r' && ptr[1] == '\n') && cur < 1023){
			value[cur++] = *ptr++;

		}
		value[cur] = '\0';

		if (strcmp(key, "Host") == 0) {
			req->host = malloc(sizeof(Header));
			req->host->key = strdup(key);
			req->host->value = strdup(value);
		}
		else if (strcmp(key, "User-Agent") == 0) {
			req->user_agent = malloc(sizeof(Header));
			req->user_agent->key = strdup(key);
			req->user_agent->value = strdup(value);
		}
		else if (strcmp(key, "Content-Length") == 0) {
			req->content_length = malloc(sizeof(Header));
			req->content_length->key = strdup(key);
			req->content_length->value = strdup(value);
		}
		else {
			headers[header_num].key = strdup(key);
			headers[header_num].value = strdup(value);
			header_num ++;

		}
		

		if (ptr[0] == '\r' && ptr[1] == '\n') ptr += 2;

	}
	ptr += 2; 

	headers[header_num].key = NULL;
	headers[header_num].value = NULL;
	req->headers = headers;
	req->header_count = header_num;
	return ptr;
}

int8_t parse_http_body(char* ptr, HttpRequest* req) {
	char body[8192];
	memset(body, 0, sizeof(body));

	while (*ptr == ' ' ||*ptr == '\n'|| *ptr == '\r') ptr++;

	if (req->content_length == NULL) {
		req->body = NULL;
		return 0;
	}
	int content_length = atoi(req->content_length->value);

	for (int i = 0; i < content_length; i++) {
		body[i] = *ptr++;
	}
	body[content_length] = '\0';

	req->body = strdup(body);
	return 0;
}



HttpRequest* parse_http_request(char* request) {
	HttpRequest* req = malloc(sizeof(HttpRequest));
	if (!req) {
		perror("malloc failed");
		return NULL;
	}
	memset(req, 0, sizeof(HttpRequest));

	regex_t regex;

	char* ptr = (char*)request;
	int8_t cur = 0; 

	ptr = parse_http_method(ptr, req);
	if (!ptr) {
		printf("Invalid HTTP method\n");
		free(req);
		return NULL;
	}

	ptr = parse_http_path(ptr, req);
	if (!ptr) {
		printf("Invalid URL path\n");
		free(req->method);
		free(req);
		return NULL;
	}

	ptr = parse_http_version(ptr, req);
	if (!ptr) {
		printf("Invalid HTTP version \n");
		free(req->method);
		free(req->path);
		free(req);
		return NULL;
	}

	ptr = parse_http_headers(ptr, req);
	if (!ptr) {
		printf("Invalid HTTP headers \n");
		free(req->method);
		free(req->path);
		free(req->headers);
		free(req);
		return NULL;
	}

	if (parse_http_body(ptr, req) == -1) {
		printf("Invalid HTTP body \n");
		free(req->method);
		free(req->path);
		free(req->headers);
		free(req->body);
		free(req);
		return NULL;
	}

	return req;
}