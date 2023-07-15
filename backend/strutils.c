#include "include/strutils.h"

int ends_with(const char *str, const char *suffix)
{
    // Check if empty
    if (!str || !suffix)
        return 0;

    // Compare length
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if ( lensuffix > lenstr )
        return 0;

    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

// Reads file by path
int read_file(char* path, char** data)
{
    int return_code = 200;

    // Check if file exists
    // (if not, return 404page)
    char* selected = path;
    if (access(path, F_OK) != 0)
    {
        selected = (char*)"./frontend/page404.html";
        return_code = 404;
    }

    // Open file
    FILE* f = fopen(selected, "rb");

    // Get length of file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory for data
    *data = calloc(fsize + 1, 1);

    // Read all
    if( fread(*data, fsize, 1, f) == 0 )
        printf(" [SERVER] ERROR: File read error %s\n", selected);

    // Close file and return
    fclose(f);

    return return_code;
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

    // Allocate memory for path
    // + 11 for './frontend/'
    // + 1 for null-terminator
    *path = (char*)calloc(pathlen + 12, 1);

    char prefix[11] = "./frontend/";
    for ( int i = 0; i < 11; i++ )
        (*path)[i] = prefix[i];

    // Copy from buffer
    for ( int i = 11; i < pathlen+11; i++ )
        (*path)[i] = buffer[i-11];

    return strlen(*path);
}
