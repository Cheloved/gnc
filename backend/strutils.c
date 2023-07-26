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
int read_file(char* file_path, int pathlen, char** data)
{
    int return_code = 200;

    // Prepend folder name
    char prepend[11] = "./frontend";

    // Create full path
    int full_path_len = pathlen + 11 + 1;
    char path[full_path_len];
    bzero(path, full_path_len);

    // Copy folder and path
    strcpy(path, prepend);
    strcpy(&(path[10]), file_path);

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

    // If cannot open, retry every second
    while ( f == NULL )
    {
        printf("\n\n [ERROR] File opening error: <%s> %s\n\n", selected, strerror(errno));
        sleep(1);
        f = fopen(selected, "rb");
    }

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
int parse_path(char* request, int request_size, char** path)
{
    // Create buffer filled with zeros
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);

    int started = 0; // 1 if pointer is within the path
    int pathlen = 0; // Length of path in symbols

    // Iterate over symbols in request message
    for ( int i = 0; i < request_size; i++ )
    {
        // first "/" indicates the beginning of the path
        if ( request[i] == '/' && !started )
            started = 1;

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
    // + 1 for null-terminator
    *path = (char*)calloc(pathlen + 1, 1);

    strcpy(*path, buffer);
    return strlen(*path);
}

int parse_data(char* request, int request_size, char** data)
{
    // Create buffer filled with zeros
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);

    // Search for new line since the data is located
    // on the last line of request
    int begin = 0;
    for ( begin = request_size - 1; begin >= 0; begin-- )
        if ( request[begin] == '\n' )
            break;

    // Copy to buffer;
    int datalen = 0;
    for ( int i = begin; i < request_size; i++ )
        buffer[datalen++] = request[i];

    *data = calloc(datalen + 1, 1);
    strcpy(*data, buffer); 

    return datalen;
}
