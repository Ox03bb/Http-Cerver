#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/stat.h> // <-- Add this line

#include "http/parser.h"


#include "http/responce_headers.h"
#include "http/status_codes.h"

#define PUBLIC_PATH "/public"


void res_404(int client_socket) {
	char response[2048];
	char html_path[512] = "./public/www/html/404.html";
	int file = open(html_path, O_RDONLY);
	if (file == -1) {
        // File not found, send 404
        const char *not_found = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";
        write(client_socket, not_found, strlen(not_found));
        return;
    }

	// Get file size
    struct stat st;
    fstat(file, &st);
    off_t file_size = st.st_size;

	char header[1024];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
			CONTENT_TYPE_HTML,
             "Content-Length: %ld\r\n",
             "\r\n", file_size);
	
	write(client_socket, header, strlen(header));

	// Send file content
	char buffer[4096];
	ssize_t bytes_read;
	while ((bytes_read = read(file, buffer, sizeof(buffer))) > 0) {
		write(client_socket, buffer, bytes_read);
	}

	close(file);

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

void file_response(int client_socket, const char* file_path) {
	char full_path[512];
	if (strcmp(file_path, "/") == 0){
		snprintf(full_path, sizeof(full_path), ".%s/www/html/index.html", PUBLIC_PATH);
	}
	else{
		snprintf(full_path, sizeof(full_path), ".%s%s", PUBLIC_PATH, file_path);
	}

	int file = open(full_path, O_RDONLY);
	printf("\n==>full_path: %s\n", full_path);
	if (file == -1) {
		res_404(client_socket);
		return;
	}

	struct stat st;
	fstat(file, &st);
	off_t file_size = st.st_size;

	char* content_type = content_type_header(full_path);

	printf("content_type:\n%s", content_type);

	char header[1024];
	snprintf(header, sizeof(header),
		"HTTP/1.1 200 OK\r\n"
		"%s"
		"Content-Length: %ld\r\n"
		"\r\n", content_type, file_size
	);


	write(client_socket, header, strlen(header));

	char buffer[4096];
	ssize_t bytes_read;
	while ((bytes_read = read(file, buffer, sizeof(buffer))) > 0) {
		write(client_socket, buffer, bytes_read);
	}

	close(file);
}
