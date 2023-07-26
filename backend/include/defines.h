#ifndef H_DEFINES
#define H_DEFINES

#define BUFFER_SIZE 8192

// Info about single host
struct s_host
{
    // Connection data
    char ip[16]; // IP in human-readable format
    int ip_bin;  // IP in binary representation
    int port;

    // Amount of threads it wills to share
    int threads;

    // Additional info (optional)
    char* extra_info;
    int extra_len;
};

#endif
