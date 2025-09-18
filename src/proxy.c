#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <openssl/err.h>
#include <openssl/ssl.h>

#include "http/parser.h"
#include "logging.h"
#include "proxy.h"
#include "secure_con.h"

int proxy(SSL *ssl, char *request, Config cfg) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[4096] = {0};
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return 500;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(cfg.proxy.target_port);

    if (inet_pton(AF_INET, cfg.proxy.target_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return 500;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return 500;
    }

    remove_substring(request, cfg.proxy.prefix);

    write(client_fd, request, strlen(request));

    while (valread = read(client_fd, buffer, 4096 - 1) > 0) {
        SSL_write(ssl, buffer, strlen(buffer));
        memset(buffer, 0, valread);
    }

    close(client_fd);
    return 200;
}