#ifndef PROXY_H
#define PROXY_H

#define PROXY_PORT 8000
#define PROXY_ADDR "127.0.0.1"

#define PROXY_PREFIX "/proxy/"
#define PROXY_MODE   1

int proxy(SSL *ssl, char *request);
void remove_substring(char *str, const char *sub);

#endif // PROXY_H