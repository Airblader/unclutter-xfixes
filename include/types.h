// vim:ts=4:sw=4:expandtab -*- c-basic-offset:4 tab-width:4 -*-
#pragma once

typedef struct match_t {
    char *name;
    int len;
} match_t;

typedef struct Config {
    long timeout;
    long jitter;
    bool exclude_root;
    bool ignore_scrolling;
    bool touch;
    bool fork;
    bool debug;
    bool onescreen;
    bool ignore_matches;
    match_t *matches;
} Config;

typedef struct coordinates_t {
    int x;
    int y;
} coordinates_t;

