#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(ekho, 6.4.4, hgneng at gmail.com)
AC_CONFIG_SRCDIR([lib/ekho.cpp])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([-Wall foreign 1.9 tar-pax subdir-objects])
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lfriso':
AC_CHECK_LIB([friso], [main])
# FIXME: Replace `main' with a function in `-lsndfile':
AC_CHECK_LIB([sndfile], [main])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h locale.h malloc.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([gethostbyname inet_ntoa memmove memset realpath setlocale socket strdup strrchr strstr strtol])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
