#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([normalmap], [0.0.2], [h@realh.co.uk])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/options.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for libraries.
PKG_CHECK_MODULES([normalmap], [libpng >= 1.6.0 popt])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup strspn])

AC_CONFIG_FILES([Makefile
                 src/Makefile])

AC_OUTPUT
