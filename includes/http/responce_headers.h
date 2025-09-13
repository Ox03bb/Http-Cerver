
#ifndef RESPONCE_HEADERS_H
#define RESPONCE_HEADERS_H

char* date_header(void);
char* content_length_header(int length);
char* cache_control_header(const char* directive);
char* content_type_header(const char* filename);

#define SERVER_HEADER "Server: Http-Cerver/0.1\r\n"

#define CONTENT_TYPE_HTML "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_TYPE_JS "Content-Type: application/javascript\r\n"
#define CONTENT_TYPE_PNG "Content-Type: image/png\r\n"
#define CONTENT_TYPE_JPEG "Content-Type: image/jpeg\r\n"
#define CONTENT_TYPE_PDF "Content-Type: application/pdf\r\n"
#define CONTENT_TYPE_JSON "Content-Type: application/json\r\n"
#define CONTENT_TYPE_TEXT "Content-Type: text/plain\r\n"
#define CONTENT_TYPE_XML "Content-Type: application/xml\r\n"
#define CONTENT_TYPE_FORM "Content-Type: application/x-www-form-urlencoded\r\n"
#define CONTENT_TYPE_GIF "Content-Type: image/gif\r\n"
#define CONTENT_TYPE_SVG "Content-Type: image/svg+xml\r\n"
#define CONTENT_TYPE_OCTET_STREAM "Content-Type: application/octet-stream\r\n"

#define CONTENT_LENGTH "Content-Length: %d\r\n"
#define DATE_HEADER "Date: %s\r\n"
#define LAST_MODIFIED "Last-Modified: %s\r\n"
#define CACHE_CONTROL "Cache-Control: %s\r\n"

#define CONNECTION_CLOSE "Connection: close\r\n"
#define CONNECTION_KEEP_ALIVE "Connection: keep-alive\r\n"


#endif // RESPONCE_HAEDERS_H