#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 

#include "http/parser.h"

#define PORT 4500
#define BUFSIZE 4096

int main(int argc, char const *argv[]){
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	int opt = 1;

	char* buffer = (char*)malloc(BUFSIZE * sizeof(char));
	memset(buffer, 0, BUFSIZE); // clear buffer

	char* server_msg = "server is running";

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd < 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET; // IPv4
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    addr.sin_port = htons(PORT); 

	if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

	if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	printf("Server listening on address %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	while (1) {
		int new_socket = accept(server_fd, (struct sockaddr*)&addr, &addr_len);
		if (new_socket < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		read(new_socket, buffer, BUFSIZE);
		// printf("Client: %s\n", buffer);
		HttpRequest* req = parse_http_request(buffer);
		if (req) {
			printf("Method: %s\n", req->method);
			printf("Path: %s\n", req->path);
			printf("Version: %s\n", req->version);
		}

		send(new_socket, server_msg, strlen(server_msg), 0);
		printf("Server message sent\n");

		close(new_socket);
		
		memset(buffer, 0, BUFSIZE); // clear buffer
	}
	free(buffer);
	return 0;
}
