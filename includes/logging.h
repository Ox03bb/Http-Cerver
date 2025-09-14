#ifndef LOGGING_H
#define LOGGING_H

#define INFO "\033[32mINFO\033[0m"
#define ERROR "\033[31mERROR\033[0m"
#define WARN "\033[33mWARN\033[0m"
#define DEBUG "\033[34mDEBUG\033[0m"

#define CODE_101 "\033[34m101\033[0m" // Switching Protocols
#define CODE_200 "\033[32m200\033[0m" // OK
#define CODE_400 "\033[31m400\033[0m" // Bad Request
#define CODE_401 "\033[31m401\033[0m" // Unauthorized
#define CODE_403 "\033[31m403\033[0m" // Forbidden
#define CODE_404 "\033[31m404\033[0m" // Not Found
#define CODE_500 "\033[31m500\033[0m" // Internal Server Error

void http_log(HttpRequest *request, char *level, char *code);
char* status_code_to_string(int code);

#endif // LOGGING_H