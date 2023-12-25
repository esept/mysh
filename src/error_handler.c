/**
 * @file error_handler.c
 * @brief Implementation of functions to handle error / return codes.
 */

#include <stdio.h>
#include "error_handler.h"

/**
 * @brief Handles the given error code by printing the error message and exiting the program.
 * 
 * This function takes an error code as input and prints the corresponding error message along with the error code.
 * It then exits the program with a failure status.
 * 
 * @param errCode The error code to handle.
 */
void handleError(ErrorCode errCode)
{
    fprintf(stderr, ROUGE("Error: %s (code %d)\n"), getErrorMessage(errCode), errCode);
    exit(EXIT_FAILURE);
}

/**
 * @brief Retrieves the error message associated with the given error code.
 * 
 * This function takes an error code as input and returns the corresponding error message.
 * If the error code is not recognized, it returns "Unknown".
 * 
 * @param errCode The error code to get the error message for.
 * @return The error message associated with the error code.
 */
char *getErrorMessage(ErrorCode errCode)
{
    switch (errCode)
    {
    case INVALID_ARGUMENT_ERROR:
        return "invalid argument";
    case MALLOC_ERROR:
        return "malloc";
    case REALLOC_ERROR:
        return "realloc";
    case FORK_ERROR:
        return "fork";
    case EXEC_ERROR:
        return "exec";
    case EXECVP_ERROR:
        return "execvp";
    case FGETS_ERROR:
        return "fgets";
    case GETCWD_ERROR:
        return "realloc";
    case CLOSEDIR_ERROR:
        return "invalid argument";
    case FOPEN_ERROR:
        return "malloc";
    case FSCANF_ERROR:
        return "realloc";
    case OPENDIR_ERROR:
        return "malloc";
    case STAT_ERROR:
        return "realloc";
    default:
        return "unknown";
    }
}
