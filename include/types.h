// vim:ts=4:sw=4:expandtab
#pragma once

typedef struct Config {
    long timeout;
    long jitter;
    bool exclude_root;
    bool ignore_scrolling;
    bool fork;
    bool debug;
} Config;

typedef struct coordinates_t {
    int x;
    int y;
} coordinates_t;
