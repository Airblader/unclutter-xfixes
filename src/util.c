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

void parse_buttons_numbers(char *str, ignore_buttons_t *ignore_buttons) {
    char *button = strtok(str, ",");
    while (button != NULL) {
        long number = atol(button);
        button = strtok(NULL, ",");
        if (number < 0 || number > UINT_MAX) {
            continue;
        }

        ignore_buttons->count++;
        if (ignore_buttons->count == UCHAR_MAX) {
            bail("Too much buttons numbers");
        }
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

    if (!ignore_buttons->count) {
        bail("Amount of buttons to ignore = 0");
    }
}
