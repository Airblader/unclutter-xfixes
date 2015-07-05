// vim:ts=4:sw=4:expandtab
#include "all.h"

void bail(char *message) {
    ELOG("Received error: %s", message);
    errx(EXIT_FAILURE, message);
}

long parse_int(char *str) {
    char *endptr = NULL;
    long parsed = strtol(str, &endptr, 10);
    if (parsed == LONG_MIN || parsed == LONG_MAX ||
            parsed < 0 || endptr == str) {

        return -1;
    }

    return parsed;
}
