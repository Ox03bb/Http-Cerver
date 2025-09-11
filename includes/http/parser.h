#ifndef PARSER_H
#define PARSER_H

typedef struct Header {
    char* key;
    char* value;
} Header;

typedef struct HttpRequest {
    char* method;
    char* path;
    char* query_params;
    char* version;
    Header* headers;
    char* body;
} HttpRequest;

HttpRequest* parse_http_request(char* request);

#endif // PARSER_H