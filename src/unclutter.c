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
static void display_init();

Display *display;

int num_screens;
Window *roots;   // root of each screen

// screen and root window the cursor is in, as set in cursor_find
int active_screen;
Window active_root;

// the screen to use with the onescreen option
int default_screen;

Config config = {
    .timeout = 5,
    .jitter = 0,
    .exclude_root = false,
    .ignore_scrolling = false,
    .ignore_buttons.count = 0,
    .ignore_buttons.buttons = NULL,
    .hide_on_touch = false,
    .fork = false,
    .debug = false,
    .onescreen = false,
    .ignore_matches = false,
    .matches = NULL,
    .start_hidden = false
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

    display_init();
    extensions_init();
    if (config.start_hidden)
        cursor_hide();
    event_init();

    exit(EXIT_SUCCESS);
}

static void on_exit_hook(void) {
    cursor_show();

    if (display != NULL)
        XCloseDisplay(display);
}

static void parse_args(int argc, char *argv[]) {
    int c,
        opt_index = 0;
    static struct option long_options[] = {
        /* Compatibility options */
        { "display", required_argument, 0, 0 },
        { "idle", required_argument, 0, 0 },
        { "keystroke", no_argument, 0, 0 },
        { "grab", no_argument, 0, 0 },
        { "noevents", no_argument, 0, 0 },
        { "reset", no_argument, 0, 0 },
        { "root", no_argument, 0, 0 },
        { "onescreen", no_argument, 0, 0 },
        { "not", no_argument, 0, 0 },

        /* unclutter-xfixes options */
        { "timeout", required_argument, 0, 0 },
        { "jitter", required_argument, 0, 0 },
        { "exclude-root", no_argument, 0, 0 },
        { "ignore-scrolling", no_argument, 0, 0 },
        { "ignore-buttons", required_argument, 0, 0 },
        { "hide-on-touch", no_argument, 0, 0 },
        { "fork", no_argument, 0, 'b' },
        { "version", no_argument, 0, 'v' },
        { "help", no_argument, 0, 'h' },
        { "debug", no_argument, 0, 0 },
        { "start-hidden", no_argument, 0, 0},
        { 0, 0, 0, 0 }
    };

    while ((c = getopt_long_only(argc, argv, "t:j:bvhd:", long_options, &opt_index)) != -1) {
        long value;
        double value_double;
        const char *opt_name = long_options[opt_index].name;

#define OPT_NAME_IS(name) (strcmp(opt_name, (name)) == 0)

        switch (c) {
            case 0:
                if (OPT_NAME_IS("display")) {
                    setenv("DISPLAY", optarg, true);
                    break;
                } else if (OPT_NAME_IS("timeout") || OPT_NAME_IS("idle")) {
                    value_double = parse_double(optarg);
                    if (value_double < 0)
                        ELOG("Invalid timeout specified.");
                    else
                        config.timeout = value_double;

                    break;
                } else if (OPT_NAME_IS("jitter")) {
                    value = parse_int(optarg);
                    if (value < 0)
                        ELOG("Invalid jitter value specified.");
                    else
                        config.jitter = value;

                    break;
                } else if (OPT_NAME_IS("exclude-root")) {
                    config.exclude_root = true;
                    break;
                } else if (OPT_NAME_IS("hide-on-touch")) {
                    config.hide_on_touch = true;
                    break;
                } else if (OPT_NAME_IS("root")) {
                    config.exclude_root = false;
                    break;
                } else if (OPT_NAME_IS("onescreen")) {
                    config.onescreen = true;
                    break;
                } else if (OPT_NAME_IS("not")) {
                    config.ignore_matches = true;
                    break;
                } else if (OPT_NAME_IS("ignore-scrolling")) {
                    config.ignore_scrolling = true;
                    break;
                } else if (OPT_NAME_IS("ignore-buttons")) {
                    parse_buttons_numbers(optarg, &config.ignore_buttons);
                    break;
                } else if (OPT_NAME_IS("debug")) {
                    config.debug = true;
                    break;
                } else if (OPT_NAME_IS("keystroke") || OPT_NAME_IS("grab") || OPT_NAME_IS("noevents") || OPT_NAME_IS("reset")) {
                    ELOG("Using unsupported unclutter argument \"%s\", ignoring.", opt_name);
                    break;
                } else if (OPT_NAME_IS("start-hidden")) {
                    config.start_hidden = true;
                    break;
                }

                print_usage(argv);
                break;
            case 'b':
                config.fork = true;
                break;
            case 'v':
                fprintf(stderr, "unclutter-xfixes version %s\n", __VERSION);
                exit(EXIT_SUCCESS);
                break;
            case 'd':
                setenv("DISPLAY", optarg, true);
                break;
            case 'h':
            default:
                print_usage(argv);
                break;
        }
    }

    if (config.ignore_matches) {
        config.matches = calloc(sizeof(match_t), argc - optind + 1);
        if (config.matches == NULL)
            bail("Failed to allocate space for matches");
        for (c = 0; optind < argc; c++, optind++) {
            char *name = argv[optind];
            config.matches[c].name = name;
            config.matches[c].len = name ? strlen(name) : 0;
        }
    }

#undef OPT_NAME_IS

}

static void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s [--timeout <n>] [--jitter <radius>] [--exclude-root] [--ignore-scrolling] [--ignore-buttons <buttons>] [--hide-on-touch] [-b|--fork] [-v|--version] [-h|--help] [--start_hidden]", argv[0]);
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

static void display_init() {
    int screen;
    Window child;
    int root_x, root_y;

    num_screens = ScreenCount(display);
    roots = calloc(sizeof(Window), num_screens);
    if (roots == NULL)
        bail("Failed to allocate root windows.");

    for (screen = 0; screen < num_screens; screen++)
        roots[screen] = XRootWindow(display, screen);
    active_screen = DefaultScreen(display);
    active_root = RootWindow(display, active_screen);
    default_screen = active_screen;
    cursor_find(&child, &root_x, &root_y);
}
