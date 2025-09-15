#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logging.h"
#include "parser.h"

void http_log(HttpRequest *request, char *level, char *code) {
    time_t now;
    struct tm *t;
    char buf[64];

    time(&now);
    t = localtime(&now);

    strftime(buf, sizeof(buf) - 1, "%Y-%m-%d %H:%M:%S", t);

    printf("%s [%s] - %s %s %s | %s\n", buf, level, request->method, request->path,
           request->version, code);
}

char *status_code_to_string(int code) {
    switch (code) {
    case 101:
        return CODE_101;
    case 200:
        return CODE_200;
    case 400:
        return CODE_400;
    case 401:
        return CODE_401;
    case 403:
        return CODE_403;
    case 404:
        return CODE_404;
    case 500:
        return CODE_500;
    default:
        return "\033[33mUnknown\033[0m"; // Unknown code in yellow
    }
}