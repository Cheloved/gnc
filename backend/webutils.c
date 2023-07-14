#include "include/webutils.h"

// Returns composed response message
int create_http_response(char** response, int return_code,
                         char* type, char* message)
{
    // Template with HTTP headers
    char* template = (char*)"HTTP/1.1 %d OK\r\n" \
                            "Content-Type: %s\r\n" \
                            "Content-Length: %d\r\n\r\n" \
                            "%s";

    // Calculating lengths
    int message_len = strlen(message);

    // Create buffer with a bit more space,
    // so that we don't need to calculate lengths of integers
    // and other symbols
    size_t buffer_size = message_len + strlen(template)*2;
    char buffer[buffer_size];
    bzero(buffer, buffer_size); // Clear buffer

    // Insert data according to the template
    sprintf(buffer, template, return_code, type, message_len, message);
    int response_len = strlen(buffer) + 1; // +1 for null-terminator

    // Allocate memory for response message
    *response = (char*)calloc(response_len, 1);

    // Copy from buffer 
    strcpy(*response, buffer);

    return response_len;
}
