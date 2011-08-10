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

AC_ARG_WITH([puddle],
              [AS_HELP_STRING([--with-puddle],
                              [compiles puddle shallow parser @<:@default=no@:>@])],
                              [],
                              [with_puddle=no]
                              )

AC_MSG_CHECKING(whether to compile puddle shallow parser)
AS_IF([test "x$with_puddle" != "xno"],
      [
       AC_ARG_WITH([bonsai-path],
                   [AS_HELP_STRING([--with-bonsai-path=dir],
                                   [Bonsai common files path (required by puddle) @<:@default=/home/user/poleng4/src/bonsai/common@:>@])],
                                   [bonsai_path="$withval"],
                                   [bonsai_path="/home/user/poleng4/src/bonsai/common"])
       CPPFLAGS="$CPPFLAGS -I$bonsai_path"
       LIBS="$LIBS -lre2 -lboost_graph -lboost_serialization"
       AC_DEFINE([WITH_PUDDLE],[1],[Enables puddle])
       AC_SUBST([BONSAI_PATH],[$bonsai_path])
       AC_MSG_RESULT(yes)
       ],
       [
        AC_MSG_RESULT(no)
        AC_DEFINE([WITH_PUDDLE],[0],[Disables puddle])
        ]
       )

AM_CONDITIONAL(WITH_PUDDLE, test "x$with_puddle" == "xyes")

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
