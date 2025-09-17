#ifndef SSL_H
#define SSL_H

#include <openssl/err.h>
#include <openssl/ssl.h>

#define CERT_FILE "certs/server.crt"
#define KEY_FILE  "certs/server.key"

SSL_CTX *init_server_ctx(void);
void load_certificates(SSL_CTX *ctx, const char *cert_file, const char *key_file);

#endif // SSL_H