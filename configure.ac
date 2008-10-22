###############################################################################
# prelude
###############################################################################

AC_PREREQ([2.62])
AC_INIT([MP4v2], [1.0.0], [eddyg@myreflection.org])

AC_MSG_NOTICE([
  -->
  --> Configuring ]AC_PACKAGE_STRING[
  -->])

AM_MAINTAINER_MODE
AC_CONFIG_AUX_DIR([autoaux])
AM_INIT_AUTOMAKE([1.10.1 foreign -Wall -Werror subdir-objects nostdinc no-dependencies no-dist-gzip dist-bzip2])

AC_CONFIG_SRCDIR([src/mp4.cpp])

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--disable-debug],[disable debugging compilation])])
AC_ARG_ENABLE([optimize],
    [AS_HELP_STRING([--disable-optimize],[disable optimizing compilation])])
AC_ARG_ENABLE([largefile],
    [AS_HELP_STRING([--disable-largefile],[disable LFS check (large file support)])])
AC_ARG_ENABLE([cygwin-mwin32],
    [AS_HELP_STRING([--enable-cygwin-mwin32],[enable WIN32 mode when building with Cygwin)])])

# disable unused langs (improves config speed, reduces configure file size)
AC_DEFUN([_LT_AC_LANG_F77_CONFIG], [:])
AC_DEFUN([_LT_AC_LANG_GCJ_CONFIG], [:])
AC_DEFUN([_LT_AC_LANG_RC_CONFIG], [:])

###############################################################################
# checks for programs
###############################################################################

AC_PROG_MKDIR_P
AC_PROG_CXX
AC_PROG_LIBTOOL

AC_CHECK_PROG([HAVE_SVN],[svn],[yes],[no])

###############################################################################
# prepare project metadata
###############################################################################

PROJECT_version_hex=`echo $PACKAGE_VERSION | awk -F. '{ printf("0x%04x%02x%02x", $1, $2, $3 ) }' 2>/dev/null`
PROJECT_version_major=`echo $PACKAGE_VERSION | awk -F. '{ print $1 }' 2>/dev/null`
PROJECT_version_minor=`echo $PACKAGE_VERSION | awk -F. '{ print $2 }' 2>/dev/null`
PROJECT_version_point=`echo $PACKAGE_VERSION | awk -F. '{ print $3 }' 2>/dev/null`
PROJECT_build=`date`

# pull svn info fields with awk script
if test "$HAVE_SVN" = "yes"; then
    changequote(<<,>>)dnl
    eval `svn info $ac_abs_confdir | awk -F'^[^:]+: *' -f $ac_aux_dir/svninfo.awk prefix=PROJECT_`
    changequote([,])dnl
fi

test -z "$PROJECT_version_hex"   && PROJECT_version_hex="0x0"
test -z "$PROJECT_version_major" && PROJECT_version_major="0"
test -z "$PROJECT_version_minor" && PROJECT_version_minor="0"
test -z "$PROJECT_version_point" && PROJECT_version_point="0"
test -z "$PROJECT_build"         && PROJECT_build="unknown"
test -z "$PROJECT_repo_url"      && PROJECT_repo_url="svn://nowhere.com/project/unknown"
test -z "$PROJECT_repo_uuid"     && PROJECT_repo_uuid="00000000-0000-0000-0000-000000000000"
test -z "$PROJECT_repo_rev"      && PROJECT_repo_rev="0"
test -z "$PROJECT_repo_date"     && PROJECT_repo_date=$PROJECT_build
test -z "$PROJECT_repo_type"     && PROJECT_repo_type="unknown"

AC_SUBST([PROJECT_version_hex])
AC_SUBST([PROJECT_version_major])
AC_SUBST([PROJECT_version_minor])
AC_SUBST([PROJECT_version_point])
AC_SUBST([PROJECT_build])
AC_SUBST([PROJECT_repo_url])
AC_SUBST([PROJECT_repo_uuid])
AC_SUBST([PROJECT_repo_rev])
AC_SUBST([PROJECT_repo_date])
AC_SUBST([PROJECT_repo_type])

. $srcdir/project/project.defs
AC_SUBST([PROJECT_website])
AC_SUBST([PROJECT_irc])

###############################################################################
# checks for libraries
###############################################################################

###############################################################################
# checks for header files
###############################################################################

AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([winsock2.h])

if test "$ac_cv_header_winsock2_h" = "yes"; then
    AC_SUBST([AC_LIB_WINSOCK2],[-lws2_32])
fi

###############################################################################
# checks for typedefs, structures, and compiler characteristics
###############################################################################

###############################################################################
# check for --disable-largefile
###############################################################################

if test "$enable_largefile" != "no" -a "$ac_cv_header_unistd_h" = "yes"; then
    case ${host_cpu} in
        i386|ppc)
            AC_LANG(C++)
            AC_MSG_CHECKING([for LFS (large file support) conformance])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
#ifndef _LFS_LARGEFILE
    error: not LFS conformant
#endif
]])]
,AC_MSG_RESULT([yes])
 AC_DEFINE([NEED_LFS_ACTIVATION],[1],[Define to 1 if LFS should be activated])
,AC_MSG_RESULT([no]))
            ;;
    esac
fi

###############################################################################
# check for --enable-cygwin-mwin32
###############################################################################

if test "$enable_cygwin_mwin32" == "yes"; then
    case ${host} in
        i?86-*-cygwin)
            test "$GCC" = "yes" && AC_CXX_CYGWIN="$AC_CXX_CYGWIN -mwin32"
            ;;
    esac
fi

###############################################################################
# set arch flags
###############################################################################

if test "$GXX" = "yes"; then
    case ${host} in
        i?86-apple-darwin9*)
            AC_CXX_ARCH="$AC_CXX_ARCH -arch i386"
            ;;
        x86_64-apple-darwin9*)
            AC_CXX_ARCH="$AC_CXX_ARCH -arch x86_64"
            ;;
        powerpc-apple-darwin9*)
            AC_CXX_ARCH="$AC_CXX_ARCH -arch ppc"
            ;;
        powerpc64-apple-darwin9*)
            AC_CXX_ARCH="$AC_CXX_ARCH -arch ppc64"
            ;;
        i?86-*|ppc-*)
            AC_CXX_ARCH="$AC_CXX_ARCH -m32"
            ;;
        x86_64-*|powerpc64-*)
            AC_CXX_ARCH="$AC_CXX_ARCH -m64"
            ;;
    esac
fi

###############################################################################
# disable debugging
###############################################################################

if test "$enable_debug" = "no"; then
    changequote(<<,>>)dnl
    if test "$GCC" = "yes"; then
        CFLAGS=`echo "$CFLAGS" | sed -e 's/-g[^ ]*[ ]*//' -e 's/^ //' -e 's/ $//'`
    fi
    if test "$GXX" = "yes"; then
        CXXFLAGS=`echo "$CXXFLAGS" | sed -e 's/-g[^ ]*[ ]*//' -e 's/^ //' -e 's/ $//'`
    fi
    changequote([,])dnl
fi

###############################################################################
# disable optimizing
###############################################################################

if test "$enable_optimize" = "no"; then
    changequote(<<,>>)dnl
    if test "$GCC" = "yes"; then
        CFLAGS=`echo "$CFLAGS" | sed -e 's/-O[^ ]*[ ]*//' -e 's/^ //' -e 's/ $//'`
    fi
    if test "$GXX" = "yes"; then
        CXXFLAGS=`echo "$CXXFLAGS" | sed -e 's/-O[^ ]*[ ]*//' -e 's/^ //' -e 's/ $//'`
    fi
    changequote([,])dnl
fi

###############################################################################
# checks for library functions
###############################################################################

AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([getopt_long_only])

###############################################################################
# conditional compilation
###############################################################################

AM_CONDITIONAL([ADD_GETTIMEOFDAY],[test "$ac_cv_func_gettimeofday" = "no"])
AM_CONDITIONAL([ADD_UTIL],[test "$ac_cv_func_getopt_long_only" = "yes"])

###############################################################################
# declare common substitutions
###############################################################################

AC_SUBST([AC_CXX_ARCH])
AC_SUBST([AC_CXX_CYGWIN])
AC_SUBST([AC_CXX_W])
AC_SUBST([AC_CXX_D])
AC_SUBST([AC_CXX_I])

###############################################################################
# output files
###############################################################################

AC_CONFIG_HEADERS([src/platform/config.h])

AC_CONFIG_FILES([GNUmakefile GNUmakefile.auto])
AC_CONFIG_FILES([include/mp4v2/project.h])
AC_CONFIG_FILES([project/project.texi])

AC_OUTPUT
