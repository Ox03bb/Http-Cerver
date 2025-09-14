#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "parser.h"
#include "logging.h"


void http_log(HttpRequest *request, char*level, char* code){
	time_t now;
	struct tm *t;
	char buf[64];

	time(&now);
	t = localtime(&now);

	strftime(buf, sizeof(buf)-1, "%Y-%m-%d %H:%M:%S", t);

	printf("%s [%s]: %s %s %s = %s\n", buf, level, request->method, request->path, request->version, code);

}