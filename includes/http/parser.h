#ifndef PARSER_H
#define PARSER_H

typedef struct HttpRequest {
    char* method;
    char* path;
    char* version;
    char* headers;
    char* body;
} HttpRequest;

HttpRequest* parse_http_request(const char* request);

#endif // PARSER_H