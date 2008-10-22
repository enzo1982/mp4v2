#ifndef MP4V2_PLATFORM_WIN32_WIN32_H
#define MP4V2_PLATFORM_WIN32_WIN32_H

///////////////////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <stdint.h>
#include <sys/stat.h>
#include <winsock2.h>
#include <io.h>
#include <fcntl.h>

#ifdef HAVE_GETOPT_LONG_ONLY
#   include <getopt.h>
#endif

#ifdef HAVE_SYS_TIME_H
#   include <sys/time.h>
#endif

///////////////////////////////////////////////////////////////////////////////

#ifdef min
#   undef min
#endif

#ifdef max
#   undef max
#endif

///////////////////////////////////////////////////////////////////////////////

#define ftello    ftello64
#define fseeko    fseeko64
#define srandom   srand
#define random()  int((rand() & 0xffff) << 16 | (rand() & 0xfff))

///////////////////////////////////////////////////////////////////////////////

// initial namespace decl
namespace mp4v2 { namespace platform { namespace win32 {
}}} // namespace mp4v2::platform::win32

#ifndef HAVE_GETTIMEOFDAY
#   include "gettimeofday.h"
#endif

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_PLATFORM_WIN32_WIN32_H
