#include "include/logging.h"

// Prints error message and exits
void error_exit(char* msg)
{
    perror(msg);
    exit(1);
}
