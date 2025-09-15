#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "http/responce_headers.h"

char *date_header() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char *buffer = malloc(100);
    strftime(buffer, 100, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", tm_info);
    return buffer;
}

char *content_length_header(int length) {
    char *buffer = malloc(50);
    sprintf(buffer, "Content-Length: %d\r\n", length);
    return buffer;
}

char *content_type_header(const char *filename) {
    char *buffer = malloc(100);
    const char *ext = strrchr(filename, '.');
    if (ext != NULL) {
        if (strcmp(ext, ".html") == 0) {
            sprintf(buffer, CONTENT_TYPE_HTML);
        } else if (strcmp(ext, ".css") == 0) {
            sprintf(buffer, CONTENT_TYPE_CSS);
        } else if (strcmp(ext, ".js") == 0) {
            sprintf(buffer, CONTENT_TYPE_JS);
        } else if (strcmp(ext, ".png") == 0) {
            sprintf(buffer, CONTENT_TYPE_PNG);
        } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
            sprintf(buffer, CONTENT_TYPE_JPEG);
        } else if (strcmp(ext, ".gif") == 0) {
            sprintf(buffer, CONTENT_TYPE_GIF);
        } else if (strcmp(ext, ".svg") == 0) {
            sprintf(buffer, CONTENT_TYPE_SVG);
        } else if (strcmp(ext, ".json") == 0) {
            sprintf(buffer, CONTENT_TYPE_JSON);
        } else if (strcmp(ext, ".txt") == 0) {
            sprintf(buffer, CONTENT_TYPE_TEXT);
        } else {
            sprintf(buffer, CONTENT_TYPE_OCTET_STREAM);
        }
    } else {
        sprintf(buffer, CONTENT_TYPE_OCTET_STREAM);
    }
    return buffer;
}

char *cache_control_header(const char *directive) {
    char *buffer = malloc(100);
    sprintf(buffer, "Cache-Control: %s\r\n", directive);
    return buffer;
}
