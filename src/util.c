// vim:ts=4:sw=4:expandtab
#include "all.h"

void bail(char *message) {
    ELOG("Received error: %s", message);
    errx(EXIT_FAILURE, "%s", message);
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

double parse_double(char *str) {
    char *endptr = NULL;
    double parsed = strtod(str, &endptr);
    if (parsed == -DBL_MAX || parsed == DBL_MAX ||
            parsed < 0 || endptr == str) {

        return -1;
    }

    return parsed;
}

void parse_buttons_numbers(char *str, ignore_buttons_t *ignore_buttons) {
    char *button = strtok(str, ",");
    while (button != NULL) {
        long number = parse_int(button);
        button = strtok(NULL, ",");
        if (number == -1) {
            continue;
        }

        ignore_buttons->count++;
        unsigned int *buttons = (unsigned int *)realloc(ignore_buttons->buttons,
            ignore_buttons->count * sizeof(unsigned int));
        if (buttons == NULL) {
            free(ignore_buttons->buttons);
            bail("Cannot reallocate memory for ignore-buttons");
        } else {
            ignore_buttons->buttons = buttons;
        }

        ignore_buttons->buttons[ignore_buttons->count - 1] = number;
    }

    if (ignore_buttons->count == 0) {
        bail("--ignore-buttons was specified, but no button could be parsed.");
    }
}
