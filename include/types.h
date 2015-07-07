// vim:ts=4:sw=4:expandtab
#pragma once

typedef struct Config {
    long timeout;
    bool exclude_root;
    bool fork;
    bool debug;
} Config;
