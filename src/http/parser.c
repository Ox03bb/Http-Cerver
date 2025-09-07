#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>

#define URL_PATH_PATTERN "^(/|/[a-zA-Z0-9_-]+(/[a-zA-Z0-9_-]+)*/?)$"
#define QUERY_PARAM_PATTERN "^[a-zA-Z0-9_]+=[a-zA-Z0-9_]+(&[a-zA-Z0-9_]+=[a-zA-Z0-9_]+)*$"

char* methods[9] = {"GET","POST","PUT","DELETE","HEAD","OPTIONS","PATCH","TRACE","CONNECT"};
char* versions[3] = {"HTTP/1.0","HTTP/1.1","HTTP/2.0"};

typedef struct Header{
	char* key;
	char* value;
} Header;

typedef struct Headers {
	Header* headers;
	char* host;
	int content_length;
	char* content_type;
	char* user_agent;
	char* accept;
	char* connection;
} Headers;

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
	Headers* headers;
	char* body;
} HttpRequest;

int8_t parse_http_method(char* request, char* ptr, HttpRequest* req) {
	char* method[8];
	if (!method) {
		perror("malloc failed");
		return -1;
	}

	int8_t cur = 0;

	while (*ptr != ' ' && cur < 7) {
		method[cur++] = *ptr++;
	}
	method[cur] = '\0';
	ptr++;

	for (int8_t j = 0; j < 9; j++) {
		if (strcmp(method, methods[j]) == 0) {
			req->method = method;
			return 0;
		}
	}
	return -1;
}
int8_t parse_http_path(char* request, char* ptr , HttpRequest* req) {
	char* path[2048];
	char* query_params[1024];
	
	memset(path, 0, sizeof(path));
	memset(query_params, 0, sizeof(query_params));

	int cur = 0;

	while (*ptr != ' ' && *ptr != '\r' && *ptr != '\0') {
		path[cur++] = *ptr++;
		if (*ptr == '?') {
			ptr++;
			int cur2 = 0;
			while (*ptr != ' ' && *ptr != '\r' && *ptr != '\0') {
				query_params[cur2++] = *ptr++;
			}
			query_params[cur2] = '\0';
			req->query_params = query_params;
			
			break;
		}
	}
	path[cur++] = '\0';
	

	regex_t regex;
	if (regcomp(&regex, URL_PATH_PATTERN, REG_EXTENDED) != 0) {
		printf("Could not compile regex\n");
		free(path);
		return -1;
	}

	if (regexec(&regex, path, 0, NULL, 0) != 0) {
		printf("Invalid URL path\n");
		free(path);
		return -1;
	}
	regfree(&regex);

	req->path = path;
	return 0;
}
char* parse_http_version(char* request, char* ptr) {
	char* version = malloc(16);
	if (!version) {
		perror("malloc failed");
		return NULL;
	}
	memset(version, 0, 16);

	int8_t cur = 0;
	while (*ptr != '\r' && cur < 9) {
		version[cur++] = *ptr++;
	}
	version[cur] = '\0';
	for (int8_t j = 0; j < 3; j++) {
		if (strcmp(version, versions[j]) == 0) {
			return version;
		}
	}
	free(version);
	return NULL;
}

HttpRequest* parse_http_request(char* request) {
	HttpRequest* req = malloc(sizeof(HttpRequest));
	if (!req) {
		perror("malloc failed");
		return NULL;
	}
	memset(req, 0, sizeof(HttpRequest));

	regex_t regex;

	char method[8];
	char path[1024];
	char query_params[1024];
	char version[16];
	char headers[2048];
	char body[4096];

	char* ptr = (char*)request;
	int8_t cur = 0; 

	req->method = parse_http_method(request, ptr);
	if (req->method == NULL) {
		printf("Invalid HTTP method\n");
		free(req);
		return NULL;
	}
	ptr += strlen(req->method) + 1;
	req->path = parse_http_path(request, ptr);
	if (req->path == NULL) {
		printf("Invalid URL path\n");
		free(req->method);
		free(req);
		return NULL;
	}
	ptr += strlen(req->path) + 1;
	req->version = parse_http_version(request, ptr);
	if (req->version == NULL) {
		printf("Invalid HTTP version \n");
		free(req->method);
		free(req->path);
		free(req);
		return NULL;
	}

	
// ========================================
	// // Parse headers
	// cur = 0;
	// ptr += 2;
	// while (!(ptr[0] == '\r' && ptr[1] == '\n')) {
	// 	headers[cur++] = *ptr++;
	// }
	// req->headers = headers;
	return req;
}