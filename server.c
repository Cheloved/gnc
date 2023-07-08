// Input/output
#include <stdio.h>

// Contains exit() function
#include <stdlib.h>

// Working with strings;
// Contains also bzero() function
#include <string.h>

// --- Libs for networking --- //
// Contains definitions of a number of
// data types used in system calls.
// These types are used in the next two includes
#include <sys/types.h>

// Includes a number of definitions
// of structures needed for sockets
#include <sys/socket.h>

// Contains constants and structures
// needed for Internet domain addresses
#include <netinet/in.h>

// inet_ntoa()
#include <arpa/inet.h>

// For read(int file_descriptor, void* buf, size_t count) function
#include <unistd.h>

#define BUFFER_SIZE 8192

// Prints error message and exits
void error_exit(char* msg)
{
    perror(msg);
    exit(1);
}

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

// Reads file by path
int parse_response_file(char* path, char** message)
{
    // Check if file exists
    // (if not, return 404page)
    char* selected = path;
    if (access(path, F_OK) != 0)
        selected = (char*)"page404.html";

    // Open file
    FILE* f = fopen(selected, "rb");

    // Get length of file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory for message
    *message = calloc(fsize + 1, 1);

    // Read all
    fread(*message, fsize, 1, f);

    // Close file and return
    fclose(f);

    return fsize;
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

int loop(int* sockfd, struct sockaddr_in* cli_addr, int* quit)
{
    int n, client_len, newsockfd;
    char buffer[BUFFER_SIZE];
    while ( !*quit )
    {
        // The accept() system call causes the process to block until a
        // client connects to the server. When connected, it returns a new 
        // file descriptor.
        client_len = sizeof(&cli_addr);
        newsockfd = accept(*sockfd, (struct sockaddr*)cli_addr, &client_len);
        if ( newsockfd < 0 )
            error_exit( "[ERROR] on accept" );

        // Clear buffer and read from new socket
        bzero(buffer, BUFFER_SIZE);
        n = read(newsockfd, buffer, BUFFER_SIZE - 1);
        if ( n < 0 )
            error_exit(" [ERROR] reading from socket");

        char *ip = inet_ntoa(cli_addr->sin_addr);

        printf(" [SERVER] recieved the message from %s\n%s\n", ip, buffer);
        /* printf(" [SERVER] recieved the message from %s\n", ip); */

        // Parse endpoint path
        char* path; int pathlen = parse_path(buffer, BUFFER_SIZE, &path);

        // Define variables for messages
        char* message; int message_len;
        char* type;
        char* response; int response_len;

        // If / path
        if ( strlen(path) == 0 )
        {
            message = (char*)"Server's test response message";
            type = (char*)"text/plain";
        } else {
            message; message_len = parse_response_file(path, &message);
            type = (char*)"text/html";
        }

        // Create response
        response_len = create_http_response(&response, message, type);

        // Send data
        n = write(newsockfd, response, response_len);

        if (n < 0)
            error_exit(" [ERROR] writing to socket");
    }

    return 0;
}

int main(int argc, char* argv[])
{
    // Variable to send quit message
    int quit = 0;

    // These two variables are file descriptors.
    // They store the values returned by the socket
    // system call and the accept system call
    int sockfd, newsockfd;

    // Stores the port number on which the server
    // accepts connections
    int port_number;

    // Stores the size of the address of the client.
    // Is needed for the accept system call
    int client_len;

    // n is the return value for the read() and write()
    // calls; It contains the number of characters read or
    // written
    int n;

    // The server reads characters from the socket
    // connection into this buffer
    char buffer[BUFFER_SIZE];

    // sockaddr_in is a structure containing an Internet
    // address. This structure is defined in netinet/in.h
    // struct sockaddr_in
    // {
    //     short sin_family; // AF_INET for Internet connection
    //     u_short sin_port;
    //     struct in_addr sin_addr;
    //     char sin_zero[8]; // not used, must be zero
    // }
    // in_addr structure contains only one field, an unsigned
    // long called s_addr.
    // serv_addr will contain the address of the server,
    // and cli_addr will contain the address of the client
    // which connects to the server.
    struct sockaddr_in serv_addr, cli_addr;

    // Handle wrong input argument count
    if ( argc < 2 )
        error_exit(" [ERROR] No port provided!");

    // The socket() system call creates a new socket. 
    // It takes 3 arguments:
    //     1) Address domain of the socket
    //         a) AF_UNIX for two processes which share a common
    //            file system;
    //         b) AF_INET for two hosts on the Internet;
    //     2) Type of socket
    //         a) SOCK_STREAM in which characters are read
    //            in a continuous stream;
    //         b) SOCK_DGRAM for datagram in which messages
    //            are read in chunks;
    //     3) The protocol (should always be zero except for
    //        unusual circumstances)
    // The socket system call returns an entry into the file
    // desctiptor table (i.e. a small integer). This value is
    // used for all subsequent references to this socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Allow reusing
    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if ( sockfd < 0 )
        error_exit(" [ERROR] Error while opening socket!");

    // Sets all values in a buffer to zero
    bzero( (char*)&serv_addr, sizeof(serv_addr) );

    // Read port number
    port_number = atoi(argv[1]);

    // Filling serv_addr data
    serv_addr.sin_family = AF_INET;

    // Port number should be converted to network byte order
    // using the function htons()
    serv_addr.sin_port = htons(port_number);

    // Set current IP address for server
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // bind() system call binds a socket to an address. 
    // Takes 3 arguments:
    //     1) Socket file descriptor
    //     2) The address to which socket is bound
    //     3) Size of the address

    if ( bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error_exit(" [ERROR] when binding");

    // The listen system call allows the process to listen on
    // the socket for connections. The first argument is the
    // socket file descriptor, and the second is the size of the
    // backlog queue, i.e. the number of connections that can
    // wait while the process is handling a particular
    // connection
    listen(sockfd, 5);

    // Loop responce cycle forever
    loop(&sockfd, &cli_addr, &quit);

    return 0;
};
