#ifndef RESPONCE_H
#define RESPONCE_H

void res_404(SSL *ssl);
char *res_200(const char *body);
int file_response(SSL *ssl, const char *file_path);

#endif // RESPONCE_H