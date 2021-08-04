#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ([2.69])
AC_INIT(lasers, 1.0, sergio.ricossa@gmx.ch)
AC_LANG(C++)

AM_INIT_AUTOMAKE(lasers, 1.0)

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([SDL2], [SDL_Init])
AC_CHECK_LIB([SDL2_image], [IMG_Init])
AC_CHECK_LIB([SDL2_ttf], [TTF_Init])

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

AC_OUTPUT(makefile src/makefile resources/makefile)
AC_OUTPUT
