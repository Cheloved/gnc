#include "include/webutils.h"

// Returns composed response message
int create_http_response(char** response, char* message, char* type)
{
    // Template with HTTP headers
    char* template = (char*)"HTTP/1.1 200 OK\r\n" \
                            "Content-Type: %s\r\n" \
                            "Content-Length: %d\r\n\r\n" \
                            "%s";

    // Calculating lengths
    int message_len = strlen(message);
    int response_len = message_len + strlen(template) + strlen(type);

    // Allocate memory for response message
    *response = (char*)calloc(response_len, 1);

    // Insert data according with template
    sprintf(*response, template, type, message_len, message);

    return response_len;
}
