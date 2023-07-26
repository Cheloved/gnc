#ifndef H_STRUTILS 
#define H_STRUTILS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "defines.h"

// Checks if string ends with suffix
int ends_with(const char *str, const char *suffix);

// Reads file by path
int read_file(char* path, char** data);

// Extracts file path from request message
int parse_path(char* request, int request_size, char** path);

// Extracts payload from request message
int parse_data(char* request, int request_size, char** data);

#endif
