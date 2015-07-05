// vim:ts=4:sw=4:expandtab
#include "all.h"

static bool hidden = false;

void cursor_show(void) {
    if (!hidden)
        return;

    DLOG("Showing the cursor.");
    XFixesShowCursor(display, DefaultRootWindow(display));
    XFlush(display);
    hidden = false;
}

void cursor_hide(void) {
    if (hidden)
        return;

    DLOG("Hiding the cursor.");
    XFixesHideCursor(display, DefaultRootWindow(display));
    XFlush(display);
    hidden = true;
}
