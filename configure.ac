#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libteemo], [0.0.1], [i@dingtao@org])
AM_INIT_AUTOMAKE([foreign -Wall])
AC_CONFIG_SRCDIR([src/tm_stack.c])
AC_CONFIG_HEADERS([config.h])

LT_INIT

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AM_PROG_AR
_
# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
