#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <openssl/err.h>
#include <openssl/ssl.h>

#include "config.h"
#include "http/parser.h"
#include "http/responce.h"
#include "logging.h"
#include "proxy.h"
#include "secure_con.h"

#define PORT    4500
#define BUFSIZE 4096

int server() {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int opt = 1;

    Config cfg = {0};

    if (load_config("./config.toml", &cfg) != 0) {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    printf("Server will start at %s:%d\n", cfg.server.host, cfg.server.port);
    printf("Public directory: %s\n", cfg.server.cert_file);

    char *buffer = malloc(BUFSIZE);
    memset(buffer, 0, BUFSIZE);

    SSL_library_init();
    SSL_CTX *ctx = init_server_ctx();
    load_certificates(ctx, cfg.server.cert_file, cfg.server.key_file);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(cfg.server.port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("HTTPS Server listening on %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    int status;
    while (1) {
        int new_socket = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, new_socket);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            close(new_socket);
            continue;
        } else {
            int bytes = SSL_read(ssl, buffer, BUFSIZE);
            if (bytes > 0) {
                buffer[bytes] = 0;

                HttpRequest *req = parse_http_request(buffer);
                if (req) {
                    if (cfg.proxy.enable == 1 &&
                        strncmp(req->path, cfg.proxy.prefix, strlen(cfg.proxy.prefix)) == 0) {
                        status = proxy(ssl, buffer, cfg);
                    } else {
                        status = file_response(ssl, req->path);
                    }
                    http_log(req, status_code_to_level(status), status_code_to_string(status));
                    free_http_request(req);
                }
            }
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(new_socket);
        memset(buffer, 0, BUFSIZE);
    }

    free(buffer);
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}