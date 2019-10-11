AM_CFLAGS = -D'__VERSION="${VERSION}"' "-I$(top_srcdir)/include"
AM_CFLAGS += -std=gnu99 -Wall -Wundef -Wshadow -Wformat-security
AM_CFLAGS += $(X11_CFLAGS)
AM_LDFLAGS = -lev $(X11_LIBS)

bin_PROGRAMS = unclutter

unclutter_SOURCES = \
	cursor.c \
	event.c \
	extensions.c \
	unclutter.c \
	util.c
