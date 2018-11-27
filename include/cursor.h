// vim:ts=4:sw=4:expandtab -*- c-basic-offset:4 tab-width:4 -*-
#pragma once

#include <X11/Xlib.h>

void cursor_show(void);

void cursor_hide(void);

void cursor_find(Window *child, int *root_x, int *root_y);
