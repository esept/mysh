#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdlib.h>
#include <errno.h>

#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"
#define ORANGE(m) "\033[01;33m"m"\033[0m"

//Error codes for the program.
typedef enum
{
    INVALID_ARGUMENT_ERROR, // Error code for invalid arguments
    MALLOC_ERROR,               
    REALLOC_ERROR,          
    FORK_ERROR,
    EXEC_ERROR,
    EXECVP_ERROR,
    FGETS_ERROR,
    GETCWD_ERROR,
    CLOSEDIR_ERROR,
    FOPEN_ERROR,
    FSCANF_ERROR,
    OPENDIR_ERROR,
    STAT_ERROR,
    UNKNOW_ERROR,

} ErrorCode;

void handleError(ErrorCode error);
char *getErrorMessage(ErrorCode error);

#endif
