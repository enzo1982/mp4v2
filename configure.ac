###############################################################################
# prelude
###############################################################################

m4_include([project/project.m4sugar])

m4_define([PRJ_version_hex],m4_format([0x%04x%02x%02x],PRJ_version_major,PRJ_version_minor,PRJ_version_point))
m4_define([PRJ_version],m4_format([%s.%s.%s],PRJ_version_major,PRJ_version_minor,PRJ_version_point))

###############################################################################
# initialization
###############################################################################

AC_PREREQ([2.69])
AC_INIT([PRJ_name],[PRJ_version],[PRJ_support],[PRJ_name_lower],[PRJ_url_website])

AC_MSG_NOTICE([
  -->
  --> Configuring ]AC_PACKAGE_STRING[
  -->])

AC_CONFIG_AUX_DIR([autoaux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.9.6 foreign -Wall -Wno-portability -Werror subdir-objects nostdinc no-dist-gzip dist-bzip2 dejagnu])

AC_CONFIG_SRCDIR([src/mp4.cpp])

# disable unused langs (improves config speed, reduces configure file size)
AC_DEFUN([_LT_AC_LANG_F77_CONFIG], [:])
AC_DEFUN([_LT_AC_LANG_GCJ_CONFIG], [:])
AC_DEFUN([_LT_AC_LANG_RC_CONFIG], [:])

AC_CANONICAL_HOST

###############################################################################
# add configure options
###############################################################################

if test -z "$enable_dependency_tracking"; then
    enable_dependency_tracking=no
fi

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--disable-debug],[disable debugging compilation])])
AC_ARG_ENABLE([optimize],
    [AS_HELP_STRING([--disable-optimize],[disable optimizing compilation])])
AC_ARG_ENABLE([fvisibility],
    [AS_HELP_STRING([--disable-fvisibility],[disable use of -fvisibility])])
AC_ARG_ENABLE([largefile],
    [AS_HELP_STRING([--disable-largefile],[disable LFS (large file support)])])
AC_ARG_ENABLE([util],
    [AS_HELP_STRING([--disable-util],[disable build of command-line utilities])])
AC_ARG_ENABLE([gch],
    [AS_HELP_STRING([--enable-gch],[enable use of precompiled headers])])
AC_ARG_ENABLE([bi],
    [AS_HELP_STRING([--enable-bi=ARCH],[enable -mARCH for bi-arch compilation])])
AC_ARG_ENABLE([ub],
    [AS_HELP_STRING([--enable-ub@<:@=ARCHS@:>@],[enable -arch ARCH for universal-binaries (OSX only)])])
AC_ARG_ENABLE([cygwin_win32],
    [AS_HELP_STRING([--enable-cygwin-win32],[when building with Cygwin use -mwin32])])
AC_ARG_ENABLE([mingw_threads],
    [AS_HELP_STRING([--enable-mingw-threads],[when building with MinGW use -mthreads])])

###############################################################################
# use libtool
###############################################################################

LT_INIT([disable-static win32-dll])

###############################################################################
# use pkg-config
###############################################################################

PKG_PROG_PKG_CONFIG

###############################################################################
# checks for programs
###############################################################################

AC_PROG_CXX

AC_CHECK_PROG([FOUND_HELP2MAN],[help2man],[yes],[no])

###############################################################################
# top-level platform check
###############################################################################

AC_MSG_CHECKING([$PACKAGE_NAME platform portability])
X_PLATFORM=posix
case ${host} in
*-*-cygwin*)
    X_CXX_W="$X_CXX_W -Wno-format"
    if test "$enable_cygwin_win32" = "yes"; then
        X_PLATFORM=win32
        X_CXX_ARCH="$X_CXX_ARCH -mwin32"
    fi
    ;;
*-*-msys*|*-*-mingw*)
    X_PLATFORM=win32
    X_MINGW_LIBS="$X_MINGW_LIBS"
    X_CXX_W="$X_CXX_W -Wno-format"
    if test "$enable_mingw_threads" = "yes"; then
        X_CXX_ARCH="$X_CXX_ARCH -mthreads"
    fi
    ;;
esac
AC_MSG_RESULT([$X_PLATFORM])

###############################################################################
# prepare project metadata
###############################################################################

PROJECT_name="PRJ_name"
PROJECT_name_lower="PRJ_name_lower"
PROJECT_name_upper="PRJ_name_upper"
PROJECT_name_formal="PRJ_name PRJ_version"
PROJECT_url_website="PRJ_url_website"
PROJECT_url_downloads="PRJ_url_downloads"
PROJECT_url_discussion="PRJ_url_discussion"
PROJECT_url_bugreport="PRJ_url_bugreport"
PROJECT_support="<PRJ_support>"

PROJECT_version="PRJ_version"
PROJECT_version_hex="PRJ_version_hex"
PROJECT_version_major="PRJ_version_major"
PROJECT_version_minor="PRJ_version_minor"
PROJECT_version_point="PRJ_version_point"
PROJECT_repo_url="PRJ_repo_url"
SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(date +%s)}"
PROJECT_build=$(date -u -d "@$SOURCE_DATE_EPOCH" 2>/dev/null || date -u -r "$SOURCE_DATE_EPOCH" 2>/dev/null || date -u)

test -z "$PROJECT_version_hex"   && PROJECT_version_hex="0x0000000000000000LL"
test -z "$PROJECT_version_major" && PROJECT_version_major="0"
test -z "$PROJECT_version_minor" && PROJECT_version_minor="0"
test -z "$PROJECT_version_point" && PROJECT_version_point="0"
test -z "$PROJECT_repo_url"      && PROJECT_repo_url="git@nowhere.com:project.git"
test -z "$PROJECT_build"         && PROJECT_build="unknown"

AC_SUBST([PROJECT_name])
AC_SUBST([PROJECT_name_lower])
AC_SUBST([PROJECT_name_upper])
AC_SUBST([PROJECT_name_formal])
AC_SUBST([PROJECT_url_website])
AC_SUBST([PROJECT_url_downloads])
AC_SUBST([PROJECT_url_discussion])
AC_SUBST([PROJECT_url_bugreport])
AC_SUBST([PROJECT_support])
AC_SUBST([PROJECT_version])
AC_SUBST([PROJECT_version_hex])
AC_SUBST([PROJECT_version_major])
AC_SUBST([PROJECT_version_minor])
AC_SUBST([PROJECT_version_point])
AC_SUBST([PROJECT_repo_url])
AC_SUBST([PROJECT_build])

###############################################################################
# checks for libraries
###############################################################################

###############################################################################
# checks for header files
###############################################################################

###############################################################################
# checks for typedefs, structures, and compiler characteristics
###############################################################################

###############################################################################
# replace -ggdb3 with -ggdb2 to avoid GNU 'as' from failing
# See http://code.google.com/p/mp4v2/issues/detail?id=27
###############################################################################
if test "$GXX" = "yes"; then
	CXXFLAGS_GGDB3=`echo "$CXXFLAGS" | sed -e 's/-ggdb3/-ggdb2/'`
	CXXFLAGS="$CXXFLAGS_GGDB3"
fi

###############################################################################
# additional ldflags
###############################################################################

case ${host} in
    *-*-darwin*)
        X_libmp4v2_la_LDFLAGS='-Wl,-current_version,$(PROJECT_version_major).$(PROJECT_version_minor).$(PROJECT_version_point) -Wl,-compatibility_version,$(PROJECT_version_major).0.0'
        ;;
    *-*-cygwin*|*-*-msys*|*-*-mingw*)
        X_libmp4v2_la_LDFLAGS='-no-undefined'
        ;;
    *)
        X_libmp4v2_la_LDFLAGS=
        ;;
esac

AC_SUBST([X_libmp4v2_la_LDFLAGS])

###############################################################################
# check for --disable-fvisibility
###############################################################################

if test "$enable_fvisibility" != "no" -a "$GXX" = "yes"; then
    case ${host} in
        *-*-cygwin*|*-*-msys*|*-*-mingw*)
            ;;
        *)
            AC_LANG(C++)
            AC_CACHE_CHECK([if $CXX supports -fvisibility],[x_cv_fvisibility],[
                x_save="$CXXFLAGS"
                CXXFLAGS="$CXXFLAGS -fvisibility=hidden"
                AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[x_cv_fvisibility=yes],[x_cv_fvisibility=no])
                if test "$x_cv_fvisibility" != "yes"; then
                    CXXFLAGS="$x_save"
                fi
                x_save=
            ])
            ;;
    esac
fi

###############################################################################
# check for --disable-gch
###############################################################################

AC_MSG_CHECKING([if precompiled-headers should be created])

X_GCH=0
X_GCH_FLAGS=
X_GCH_STATIC=0
X_GCH_STATIC_FLAGS="$lt_prog_compiler_static_CXX"
X_GCH_SHARED=0
X_GCH_SHARED_FLAGS="$lt_prog_compiler_pic_CXX"

if test "$enable_gch" = "yes"; then
    x_gch_result="yes"
    X_GCH=1
    if test "$enable_static" = "yes"; then
        X_GCH_STATIC=1
    fi
    if test "$enable_shared" = "yes"; then
        X_GCH_SHARED=1
    fi
else
    x_gch_result="no"
fi

AC_MSG_RESULT([$x_gch_result])

AC_SUBST([X_GCH])
AC_SUBST([X_GCH_FLAGS])
AC_SUBST([X_GCH_STATIC])
AC_SUBST([X_GCH_STATIC_FLAGS])
AC_SUBST([X_GCH_SHARED])
AC_SUBST([X_GCH_SHARED_FLAGS])

###############################################################################
# check for --disable-largefile
###############################################################################

if test "$enable_largefile" != "no" -a "$ac_cv_header_unistd_h" = "yes"; then
    AC_LANG(C++)
    AC_CACHE_CHECK([if LFS (large file support) is required],[x_cv_largefile],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
#ifndef _LFS_LARGEFILE
    error: not LFS conformant
#endif
        ]])],[x_cv_largefile=yes],[x_cv_largefile=no])
    ])
    if test "$x_cv_largefile" = "yes"; then
        AC_DEFINE([NEED_LFS_ACTIVATION],[1],[Define to 1 if LFS should be activated])
    fi
fi

###############################################################################
# set arch flags
###############################################################################

if test "$GXX" = "yes"; then
    AC_MSG_CHECKING([arch flags])
    x_flags=
    x_save_IFS=$IFS; IFS=,
    for arch in ${enable_ub}; do
        case "$arch" in
            ""|no)
                ;;
            yes)
                case ${host} in
                    *-*-darwin*)
                        x_flags="$xflags -arch x86_64 -arch arm64"
                        ;;
                esac
                ;;
            *)
                x_flags="$x_flags -arch $arch"
                ;;
        esac
    done
    IFS=$x_save_IFS

    case "$enable_bi" in
        ""|no|yes)
            ;;
        *)
            x_flags="$x_flags -m${enable_bi}"
            ;;
    esac

    if test -n "$x_flags"; then
        AC_MSG_RESULT([$x_flags])
        X_CXX_ARCH="$X_CXX_ARCH $x_flags"
    else
        AC_MSG_RESULT([none])
    fi

    x_flags=
fi

###############################################################################
# disable debugging
###############################################################################

if test "$enable_debug" = "no"; then
    changequote(<<,>>)dnl
    if test "$GCC" = "yes"; then
        CFLAGS=`echo "-DNDEBUG $CFLAGS" | sed -e 's/-g[^ ]*[ ]*//' -e 's/^ //' -e 's/ $//'`
    fi
    if test "$GXX" = "yes"; then
        CXXFLAGS=`echo "-DNDEBUG $CXXFLAGS" | sed -e 's/-g[^ ]*[ ]*//' -e 's/^ //' -e 's/ $//'`
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

###############################################################################
# conditional compilation
###############################################################################

AM_CONDITIONAL([ADD_PLATFORM_POSIX],[test "$X_PLATFORM" = "posix"])
AM_CONDITIONAL([ADD_PLATFORM_WIN32],[test "$X_PLATFORM" = "win32"])

AM_CONDITIONAL([ADD_UTIL],[test "$enable_util" != "no"])
AM_CONDITIONAL([ADD_MANS],[test "$X_PLATFORM" != "win32"])

###############################################################################
# declare common substitutions
###############################################################################

AC_SUBST([X_CXX_ARCH])
AC_SUBST([X_CXX_W])
AC_SUBST([X_CXX_D])
AC_SUBST([X_CXX_I])
AC_SUBST([X_MINGW_LIBS])

AH_VERBATIM([MP4V2_EXPORTS],
[/* libtool defines DLL_EXPORT for windows dll
   builds, but we use MP4V2_EXPORTS instead. */
#ifdef DLL_EXPORT
# define MP4V2_EXPORTS
#else
# define MP4V2_USE_STATIC_LIB
#endif])

###############################################################################
# output files
###############################################################################

AC_CONFIG_HEADERS([libplatform/config.h])

AC_CONFIG_FILES([
    GNUmakefile
    mp4v2.pc
    project/project.m4
    include/mp4v2/project.h
])

AC_OUTPUT
