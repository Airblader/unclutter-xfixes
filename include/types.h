// vim:ts=4:sw=4:expandtab
#pragma once

typedef struct match_t {
    char *name;
    int len;
} match_t;

typedef struct ignore_buttons_t {
    unsigned char count;
    unsigned int *buttons;
} ignore_buttons_t;

typedef struct Config {
    double timeout;
    long jitter;
    bool exclude_root;
    bool ignore_scrolling;
    ignore_buttons_t ignore_buttons;
    bool hide_on_touch;
    bool fork;
    bool debug;
    bool onescreen;
    bool ignore_matches;
    match_t *matches;
    bool start_hidden;
} Config;

typedef struct coordinates_t {
    int x;
    int y;
} coordinates_t;

