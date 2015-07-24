#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([jarvis], [0.1], test@mail.com)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cc])
#[src/test_endpoint/main.cc])
#AC_CONFIG_HEADERS(include)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB([mysqlclient], [mysql_init])
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.
#AC_CHECK_HEADERS([/usr/include/mysql/*])
#AC_CHECK_HEADERS([include/*])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile src/test_endpoint/Makefile])
AC_OUTPUT
