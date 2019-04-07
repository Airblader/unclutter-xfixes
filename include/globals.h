// vim:ts=4:sw=4:expandtab
#pragma once

#include <X11/Xlib.h>
#include "types.h"

extern Display *display;

extern int xi_ext_opcode;

extern Config config;

extern int num_screens;
extern Window *roots;

extern int active_screen;
extern Window active_root;

extern int default_screen;
