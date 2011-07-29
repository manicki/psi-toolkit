#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([PSI-TOOLS], [2011.07.11], [BUG-REPORT-ADDRESS])

AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([tests/cxxtest/TestRunner.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

compiler_flags="\
        -Wall \
        "

CPPFLAGS="$compiler_flags $CPP_FLAGS"

# Checks for libraries.

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC

AC_SUBST([LIBS], ["$LIBS -lboost_program_options"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
