
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT(mp4v2, 1.0.0, eddyg@myreflection.org)
AC_CONFIG_SRCDIR([3gp.cpp])
AC_CONFIG_HEADERS([mp4v2_config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AM_INIT_AUTOMAKE([mp4v2], [1.5])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS(sys/time.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostname gettimeofday memmove memset socket strcasecmp strchr strdup strerror strrchr])

AC_CHECK_TYPES([in_port_t, socklen_t, struct iovec, struct sockaddr_storage], , , 
        [#include <sys/types.h>
         #include <sys/socket.h>
         #include <netinet/in.h>])


AC_CONFIG_FILES([Makefile
                 test/Makefile
                 util/Makefile])
AC_OUTPUT
