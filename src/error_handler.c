#include <stdio.h>
#include "error_handler.h"

void handleError(ErrorCode errCode)
{
    fprintf(stderr, ROUGE("Error: %s (code %d)\n"), getErrorMessage(errCode), errCode);
    exit(EXIT_FAILURE);
}

/**
 * @brief get the error message from the error code
 * @param error the error code
 * @return the error message
*/
char *getErrorMessage(ErrorCode errCode)
{
    switch (errCode)
    {
    case INVALID_ARGUMENT_ERROR:
        return "Invalid argument";
    case MALLOC_ERROR:
        return "Malloc";
    case REALLOC_ERROR:
        return "Realloc";
    default:
        return "Unknown";
    }
}
