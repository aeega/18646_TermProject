#include "wilkes/custom_kernel_implementation/globals.h"

#include <errno.h>
#include <limits.h>

// MSVC apparently doesn't have C's my_getline
// This is a poor man's implementation of it using fgets.
// See the man page at
// http://pubs.opengroup.org/onlinepubs/9699919799/functions/getdelim.html
size_t my_getline(char** buf, size_t* bufLen, FILE* f) {
    if (buf == NULL || bufLen == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (fgets(*buf, (int)(*bufLen), f) != *buf) return -1;

    while (1) {
        const size_t amountRead = strlen(*buf);
        // If the length of the string is less than the whole buffer
        // (minus the last space for \0) or the last character is a newline,
        // we win. Done.
        if (amountRead != *bufLen - 1 || (*buf)[amountRead - 1] == '\n')
            return amountRead;

        // We didn't have enought room, expand with realloc.

        // First make sure we can.

        // If we can't take any more, give up.
        if (*bufLen == LONG_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        // If the MSB of bufLen is 1, doubling will overflow.
        const int willOverflow = (*bufLen >> (sizeof(size_t) * 8 - 1)) == 1;
        const size_t newSize = willOverflow ? LONG_MAX : *bufLen * 2;

        char* newBuf = (char*)(realloc(*buf, newSize));
        if (newBuf == NULL) {
            errno = ENOMEM;
            return -1;
        }
        // We succeeded in expanding.
        *buf = newBuf;
        *bufLen = newSize;
        // Keep reading where we left off
        char* const resumePoint = *buf + amountRead;
        if (fgets(resumePoint, (int)(*bufLen - amountRead), f) != resumePoint)
            return -1;
    }
}
