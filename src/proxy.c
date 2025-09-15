#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http/parser.h"
#include "logging.h"
#include "proxy.h"

int proxy(int client_socket, char *request) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[4096] = {0};
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return 500;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PROXY_PORT);

    if (inet_pton(AF_INET, PROXY_ADDR, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return 500;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return 500;
    }

    send(client_fd, request, strlen(request), 0);

    while (valread = read(client_fd, buffer, 4096 - 1) > 0) {
        write(client_socket, buffer, strlen(buffer));
        memset(buffer, 0, valread);
    }

    close(client_fd);
    return 200;
}