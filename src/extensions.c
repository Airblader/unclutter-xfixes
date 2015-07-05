// vim:ts=4:sw=4:expandtab
#include "all.h"

int xi_ext_opcode = -1;

void extensions_init(void) {
    int event, error;
    if (!XQueryExtension(display, "XInputExtension", &xi_ext_opcode, &event, &error))
        bail("XInput extension is not available.");

    int major_op = 2;
    int minor_op = 2;

    int result = XIQueryVersion(display, &major_op, &minor_op);
    if (result == BadRequest)
        bail("XI2 is not supported in a sufficient version (or at all).");
    else if (result != Success)
        bail("Failed to query XI2 extension.");
}
