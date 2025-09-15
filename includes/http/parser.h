#ifndef PARSER_H
#define PARSER_H

typedef struct Header {
    char *key;
    char *value;
} Header;

typedef struct HttpRequest {
    char *method;
    char *path;
    char *query_params;
    char *version;
    Header *host;
    Header *user_agent;
    Header *content_length;
    Header *headers;
    int header_count;
    char *body;
} HttpRequest;

HttpRequest *parse_http_request(char *request);

#endif // PARSER_H