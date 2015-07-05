// vim:ts=4:sw=4:expandtab
#include "all.h"

static struct ev_loop *loop;
static ev_timer idle_watcher;
static ev_io x_watcher;

/* Forward declarations */
static void event_init_x_loop(void);
static void event_init_timer(void);
static void x_cb(EV_P_ ev_io *w, int revents);
static void idle_cb(EV_P_ ev_timer *w, int revents);

void event_init(void) {
    loop = EV_DEFAULT;

    event_init_x_loop();
    event_init_timer();

    ev_run(loop, 0);
}

static void event_init_x_loop(void) {
    ev_io_init(&x_watcher, x_cb, XConnectionNumber(display), EV_READ);
    ev_io_start(loop, &x_watcher);
}

static void event_init_timer(void) {
    // TODO make 1s configurable
    ev_timer_init(&idle_watcher, idle_cb, 0, 1);
    ev_timer_start(loop, &idle_watcher);
}

static void x_cb(EV_P_ ev_io *w, int revents) {
    // TODO show mouse if it was moved
    DLOG("called x_cb");

    // TODO only call this when the mouse has moved
    ev_timer_again(loop, &idle_watcher);
}

static void idle_cb(EV_P_ ev_timer *w, int revents) {
    // TODO hide mouse if this was called
    DLOG("called idle_cb");
}
