#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
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

# Checks for enable Java support. If options is not specified, enable it only
# if Java is available. User has to set two environment variables: JAVA_HOME
# and LD_LIBRARY_PATH.
AC_ARG_WITH([java],
            [AS_HELP_STRING([--with-java],
                [Compiling Java components @<:@default-check@:>@])],
            [],
            [with_java=no])

AC_MSG_CHECKING(wheter to compile Java components)
AS_IF([test "x$with_java" != xno],
    [
    AC_MSG_RESULT(yes)

    AC_MSG_CHECKING(whether the JAVA_HOME is set)
    AS_IF([test "$JAVA_HOME" = ""],
        AC_MSG_RESULT(no),

        AC_MSG_RESULT(yes)
        java_path="$JAVA_HOME"
        AC_CHECK_FILE($java_path/bin/java)
        AC_CHECK_FILE($java_path/include/jni.h)
        AC_SUBST(java_path)

        # FIXME: May be changed if the system architecture was determined,
        # then the variable should be '$java_path/jre/lib/i386/server' or
        # '$java_path/jre/lib/amd64/server'
        AC_MSG_CHECKING(whether the LD_LIBRARY_PATH is set)
        AS_IF([test "$LD_LIBRARY_PATH" = ""],
            AC_MSG_RESULT(no),
            AC_MSG_RESULT(yes)
        )

        CPPFLAGS="$CPPFLAGS -I$java_path/include -I$java_path/include/linux -ljvm"
        AC_SUBST([LIBS], ["$LIBS -ljvm -L$java_path/jre/lib/amd64/server/"])
    )],
    [AC_MSG_RESULT(no)])
AM_CONDITIONAL(WITH_JAVA, [test "x$with_java" != "xno"])

# Checks whether to compile puddle shallow parser.
# As long as puddle is using external classes as its internal data structure,
# path to those files must be specified.

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
