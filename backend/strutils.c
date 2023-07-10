#include "include/strutils.h"

// Reads file by path
int read_file(char* path, char** data)
{
    // Check if file exists
    // (if not, return 404page)
    char* selected = path;
    if (access(path, F_OK) != 0)
        selected = (char*)"frontend/page404.html";

    // Open file
    FILE* f = fopen(selected, "rb");

    // Get length of file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory for data
    *data = calloc(fsize + 1, 1);

    // Read all
    if( fread(*data, fsize, 1, f) != 10 )
    {
        if( feof(f) )
            printf(" [SERVER] ERROR: Premature end of file while reading %s\n", selected);
        else
            printf(" [SERVER] ERROR: File read error %s\n", selected);
    }

    // Close file and return
    fclose(f);

    return fsize;
}

// Extracts file path from request message
int parse_path(char* request, int request_size, char** path, const int buffer_size)
{
    // Create buffer filled with zeros
    char buffer[buffer_size];
    bzero(buffer, buffer_size);

    int started = 0; // 1 if pointer is within the path
    int pathlen = 0; // Length of path in symbols

    // Iterate over symbols in request message
    for ( int i = 0; i < request_size; i++ )
    {
        // first "/" indicates the beginning of the path
        if ( request[i] == '/' )
        {
            started = 1;
            continue;
        }

        // First space after '/' indicates the end of the path
        if ( request[i] == ' ' )
            if ( started )
                break;

        // Skip everything before path
        if ( !started )
            continue;

        // Copy path into buffer
        buffer[pathlen++] = request[i];
    }

    // Allocate memory for path + 1 for null-terminator
    *path = (char*)calloc(pathlen + 1, 1);

    // Copy from buffer
    for ( int i = 0; i < pathlen; i++ )
        (*path)[i] = buffer[i];

    return pathlen;
}
