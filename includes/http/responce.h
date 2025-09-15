#ifndef RESPONCE_H
#define RESPONCE_H

void res_404(int client_socket);
char *res_200(const char *body);
int file_response(int client_socket, const char *file_path);

#endif // RESPONCE_H