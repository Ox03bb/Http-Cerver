#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


char* date_header() {
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);
	char *buffer = malloc(100);
	strftime(buffer, 100, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", tm_info);
	return buffer;
}

char* content_length_header(int length) {
	char *buffer = malloc(50);
	sprintf(buffer, "Content-Length: %d\r\n", length);
	return buffer;
}

char* cache_control_header(const char* directive) {
	char *buffer = malloc(100);
	sprintf(buffer, "Cache-Control: %s\r\n", directive);
	return buffer;
}

