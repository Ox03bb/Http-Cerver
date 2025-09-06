#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>

#define URL_PATH_PATTERN "^(/|/[a-zA-Z0-9_-]+(/[a-zA-Z0-9_-]+)*/?)$"

char* methods[9] = {"GET","POST","PUT","DELETE","HEAD","OPTIONS","PATCH","TRACE","CONNECT"};
char* versions[3] = {"HTTP/1.0","HTTP/1.1","HTTP/2.0"};

typedef struct HttpRequest {
	char* method;
	char* path;
	char* version;
	char* headers;
	char* body;
} HttpRequest;

HttpRequest* parse_http_request(const char* request) {
	HttpRequest* req = malloc(sizeof(HttpRequest));
	if (!req) {
		perror("malloc failed");
		return NULL;
	}
	memset(req, 0, sizeof(HttpRequest));

	regex_t regex;

	char method[8];
	char path[256];
	char version[16];
	char headers[2048];
	char body[4096];

	char* ptr = (char*)request;
	int8_t cur = 0; 

	// Parse the HTTP method
	while (*ptr != ' ') {
		method[cur++] = *ptr++;
	}
	method[cur] = '\0';
	for (int8_t j = 0; j < 9; j++) {
		if (strcmp(method, methods[j]) == 0) {
			req->method = methods[j];
			break;
		}
	}
	if (req->method == 0) {
		printf("Invalid HTTP method\n");
		return NULL;
	}

	// Parse the URL path
	cur = 0;
	ptr++; 
	while (*ptr != ' ') {
		path[cur++] = *ptr++;
	}
	req->path = path;
	path[cur] = '\0';

	if (regcomp(&regex, URL_PATH_PATTERN, REG_EXTENDED) != 0) {
		printf("Could not compile regex\n");
		return NULL;
	}

	if (regexec(&regex, req->path, 0, NULL, 0) != 0) {
		printf("Invalid URL path\n");
		return NULL;
	}
	regfree(&regex);

	// Parse the HTTP version
	cur = 0;
	ptr++;
	while (*ptr != '\r') {
		version[cur++] = *ptr++;
	}
	version[cur] = '\0';
	for (int8_t j = 0; j < 3; j++) {
		if (strcmp(version, versions[j]) == 0) {
			req->version = versions[j];
			break;
		}
	}
	if (req->version == 0) {
		printf("Invalid HTTP version \n");
		return NULL;
	}
	

	// // Parse headers
	// cur = 0;
	// ptr += 2;
	// while (!(ptr[0] == '\r' && ptr[1] == '\n')) {
	// 	headers[cur++] = *ptr++;
	// }
	// req->headers = headers;
	return req;
}