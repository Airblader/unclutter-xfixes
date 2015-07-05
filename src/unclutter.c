// vim:ts=4:sw=4:expandtab
#include "all.h"
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef __VERSION
#define __VERSION "unknown"
#endif

typedef void (*callback)(void);

/* Forward declarations */
static void run(void);
static void on_exit_hook(void);
static void parse_args(int argc, char *argv[]);
static void print_usage(char *argv[]);
static void safe_fork(callback child_callback);

Display *display;

Config config = {
    .timeout = 5,
    .fork = false,
    .debug = false
};

int main(int argc, char *argv[]) {
    atexit(on_exit_hook);
    parse_args(argc, argv);

    if (config.fork) {
        DLOG("Forking to background.");
        safe_fork(run);
    } else
        run();
}

static void run(void) {
    display = XOpenDisplay(NULL);
    if (display == NULL)
        bail("Failed to connect to the X server.");

    extensions_init();
    event_init();

    exit(EXIT_SUCCESS);
}

static void on_exit_hook(void) {
    cursor_show();

    if (display != NULL)
        XCloseDisplay(display);
}

static void parse_args(int argc, char *argv[]) {
    int c;
    while ((c = getopt(argc, argv, "t:bvhd")) != -1) {
        long value;

        switch (c) {
            case 't':
                value = parse_int(optarg);
                if (value < 0)
                    ELOG("Invalid timeout specified.");
                else
                    config.timeout = value;

                break;
            case 'b':
                config.fork = true;
                break;
            case 'v':
                fprintf(stderr, "unclutter-xfixes version %s\n", __VERSION);
                exit(EXIT_SUCCESS);
                break;
            case 'd':
                config.debug = true;
                DLOG("Debug logging enabled.");
                break;
            case 'h':
            default:
                print_usage(argv);
                break;
        }
    }
}

static void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s [-t <n>] [-b] [-v] [-h]", argv[0]);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

static void safe_fork(callback child_callback) {
    pid_t pid = fork();
    if (!pid) {
        if (!fork()) {
            (*child_callback)();
        } else {
            exit(EXIT_SUCCESS);
        }
    } else {
        waitpid(pid, NULL, 0);
    }
}
