AC_PREREQ(2.61)

AC_INIT([unclutter-xfixes], 1.6)
AC_DEFINE_UNQUOTED([VERSION], ["$PACKAGE_VERSION"])

AC_CONFIG_AUX_DIR([autostuff])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADER([config.h])

AM_PROG_AR
AC_PROG_CC
AC_PROG_INSTALL

# libev has no pkg-config support
AC_CHECK_HEADERS([ev.h], [], [AC_MSG_ERROR([ev.h not found - please install libev])])

PKG_CHECK_MODULES([X11], [x11 xi xfixes])

AC_CHECK_PROG([has_a2x], [a2x], [true], [false])
AM_CONDITIONAL([HAS_A2X], [test "x$has_a2x" = xtrue])
if test "x$has_a2x" = xfalse
then
	AC_MSG_NOTICE([a2x not found - needed to generate man pages])
fi

AC_CONFIG_FILES([Makefile
		 include/Makefile
		 src/Makefile
		 man/Makefile])

AC_OUTPUT
