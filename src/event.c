// vim:ts=4:sw=4:expandtab
#include "all.h"

static struct ev_loop *loop;
static struct ev_timer *idle_watcher;
static struct ev_io *x_watcher;
static struct ev_check *x_check;

static coordinates_t last_cursor_pos;
static Window last_avoided = None;

/* Forward declarations */
static void event_init_x_loop(void);
static void event_init_timer(void);
static void x_cb(EV_P_ ev_io *w, int revents);
static void x_check_cb(EV_P_ ev_check *w, int revents);
static void idle_cb(EV_P_ ev_timer *w, int revents);
static void event_select_xi(void);

void event_init(void) {
    event_select_xi();

    loop = EV_DEFAULT;
    event_init_x_loop();
    event_init_timer();
    ev_run(loop, 0);
}

static void event_init_x_loop(void) {
    x_watcher = calloc(sizeof(struct ev_io), 1);
    ev_io_init(x_watcher, x_cb, XConnectionNumber(display), EV_READ);
    ev_io_start(loop, x_watcher);

    x_check = calloc(sizeof(struct ev_check), 1);
    ev_check_init(x_check, x_check_cb);
    ev_check_start(loop, x_check);
}

static void event_init_timer(void) {
    idle_watcher = calloc(sizeof(struct ev_timer), 1);
    ev_timer_init(idle_watcher, idle_cb, config.timeout, config.timeout);
    ev_timer_start(loop, idle_watcher);
}

static void x_cb(EV_P_ ev_io *w, int revents) {
    /* Deliberately empty. */
}

static bool is_button_ignored(const XIRawEvent *data) {
    if (config.ignore_scrolling && (data->detail == 4 || data->detail == 5)) {
        return true;
    }

    for (int i = 0; i < config.ignore_buttons.count; ++i) {
        if (data->detail == config.ignore_buttons.buttons[i]) {
            return true;
        }
    }

    return false;
}

static void x_check_cb(EV_P_ ev_check *w, int revents) {
    XEvent ev;
    while (XPending(display) > 0) {
        XNextEvent(display, &ev);

        XGenericEventCookie *cookie = &ev.xcookie;
        if (cookie->type != GenericEvent || cookie->extension != xi_ext_opcode ||
            !XGetEventData(display, cookie)) {
            continue;
        }

        if (cookie->evtype == XI_RawButtonPress) {
            const XIRawEvent *data = (const XIRawEvent *) cookie->data;
            if (is_button_ignored(data)) {
                XFreeEventData(display, cookie);
                continue;
            }
        }

        XFreeEventData(display, cookie);

        if (config.jitter > 0 && cookie->evtype == XI_RawMotion) {
            Window child;
            int root_x, root_y;

            cursor_find(&child, &root_x, &root_y);
            int dx = last_cursor_pos.x - root_x;
            int dy = last_cursor_pos.y - root_y;
            if (dx * dx + dy * dy < config.jitter * config.jitter) {
                continue;
            }

            last_cursor_pos.x = root_x;
            last_cursor_pos.y = root_y;
        }

        if (config.hide_on_touch && (cookie->evtype == XI_RawTouchBegin || cookie->evtype == XI_RawTouchUpdate)) {
            cursor_hide();
        } else {
            /* We don't bother checking the exact event since we only select events that interest us. */
            cursor_show();
        }
        ev_timer_again(loop, idle_watcher);
    }
}

static bool name_matches(Window win) {
    match_t *ignored;
    XClassHint hint;
    char *name;
    bool found = false;

    if (XFetchName(display, win, &name)) {
        for (ignored = config.matches; ignored->name; ignored++)
            if((found = strncasecmp(ignored->name, name, ignored->len)==0))
                break;
        XFree(name);
        if (found) return true;
    }
    if (XGetClassHint(display, win, &hint)) {
        for (ignored = config.matches; ignored->name; ignored++)
            if((found =
                strncasecmp(ignored->name, hint.res_name, ignored->len)  == 0 ||
                strncasecmp(ignored->name, hint.res_class, ignored->len) == 0))
                break;
        XFree(hint.res_name);
        XFree(hint.res_class);
        if (found) return true;
    }

    return false;
}

static bool is_on_ignore_list(Window win) {
    Window child_in;
    Window win_in;
    Window win_dummy;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask;

    if (win == last_avoided) return true;

    last_avoided = None;
    child_in = win_in = win;

    do {
        win_in = child_in;
        if (name_matches(win_in)) {
            last_avoided = win;
            return true;
        }
    } while (XQueryPointer(display, win_in, &win_dummy, &child_in, &root_x, &root_y, &win_x, &win_y, &mask)
             && child_in != None);

    return false;
}

static void idle_cb(EV_P_ ev_timer *w, int revents) {
    Window child;
    int root_x, root_y;

    cursor_find(&child, &root_x, &root_y);
    if (child) { // not on root
        if (!config.onescreen || active_screen == default_screen)
            if (!config.ignore_matches || !is_on_ignore_list(child))
                cursor_hide();
    } else { // on root
        if (!config.exclude_root)
            if (!config.onescreen || active_screen == default_screen)
                cursor_hide();
    }
}

static void event_select_xi(void) {
    XIEventMask masks[1];
    unsigned char mask[(XI_LASTEVENT + 7)/8];

    memset(mask, 0, sizeof(mask));
    XISetMask(mask, XI_RawMotion);
    XISetMask(mask, XI_RawButtonPress);
    XISetMask(mask, XI_RawTouchUpdate);
    if (config.hide_on_touch) {
        XISetMask(mask, XI_RawTouchBegin);
        XISetMask(mask, XI_RawTouchUpdate);
    }

    masks[0].deviceid = XIAllMasterDevices;
    masks[0].mask_len = sizeof(mask);
    masks[0].mask = mask;

    XISelectEvents(display, active_root, masks, 1);
    XFlush(display);
}
