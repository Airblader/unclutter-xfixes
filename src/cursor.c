// vim:ts=4:sw=4:expandtab
#include "all.h"

static bool hidden = false;

void cursor_show(void) {
    if (!hidden)
        return;

    DLOG("Showing the cursor.");
    XFixesShowCursor(display, active_root);
    XFlush(display);
    hidden = false;
}

void cursor_hide(void) {
    if (hidden)
        return;

    DLOG("Hiding the cursor.");
    XFixesHideCursor(display, active_root);
    XFlush(display);
    hidden = true;
}

void cursor_find(Window *child, int *root_x, int *root_y) {
    Window root;
    int win_x, win_y;
    unsigned int mask;
    int screen;

    for (screen = 0; screen < num_screens; screen++) {
        if (XQueryPointer(display, roots[screen], &root, child, root_x, root_y, &win_x, &win_y, &mask)) {
            active_screen = screen;
            active_root = roots[screen];
            break;
        }
    }
}
