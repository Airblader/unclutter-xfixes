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
    .fork = false,
    .debug = false
};

int main(int argc, char *argv[]) {
    atexit(on_exit_hook);
    parse_args(argc, argv);

    if (config.fork)
        safe_fork(run);
    else
        run();
}

static void run(void) {
    // TODO

    exit(EXIT_SUCCESS);
}

static void on_exit_hook(void) {
    if (display != NULL)
        XCloseDisplay(display);
}

static void parse_args(int argc, char *argv[]) {
    int c;
    while ((c = getopt(argc, argv, "bvhd")) != -1) {
        switch (c) {
            case 'b':
                config.fork = true;
                break;
            case 'v':
                fprintf(stderr, "%s version %s\n", argv[0], __VERSION);
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
    fprintf(stderr, "Usage: %s [-b] [-v] [-h] [-d]", argv[0]);
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
