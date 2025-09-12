#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 

#include "http/parser.h"


#include "http/responce_headers.h"
#include "http/status_codes.h"


char* res_404() {
	char response[2048];
	const char* body = "<html><body><h1>404 Not Found</h1></body></html>";
	int body_length = strlen(body);

	char* date = date_header();
	char* content_length = content_length_header(body_length);

	snprintf(response, sizeof(response),
		"%s%s%s%s%s%s%s%s",
		HTTP_404,
		SERVER_HEADER,
		CONTENT_TYPE_HTML,
		content_length,
		date,
		CONNECTION_CLOSE,
		CRLF,
		body
	);

	free(date);
	free(content_length);

	return strdup(response);
}

char* res_200(const char* body) {
	char response[8192];
	int body_length = strlen(body);

	char* date = date_header();
	char* content_length = content_length_header(body_length);

	snprintf(response, sizeof(response),
		"%s%s%s%s%s%s%s%s",
		HTTP_200,
		SERVER_HEADER,
		CONTENT_TYPE_HTML,
		content_length,
		date,
		CONNECTION_CLOSE,
		CRLF,
		body
	);

	free(date);
	free(content_length);

	return strdup(response);
}