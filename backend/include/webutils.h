#ifndef H_WEBUTILS
#define H_WEBUTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_http_response(char** response, int return_code, char* type, char* message);

#endif
