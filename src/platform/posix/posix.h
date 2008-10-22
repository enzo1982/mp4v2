#ifndef MP4V2_PLATFORM_POSIX_POSIX_H
#define MP4V2_PLATFORM_POSIX_POSIX_H

///////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>

#include <sys/stat.h>
#include <sys/time.h>

#include <fcntl.h>
#include <arpa/inet.h>
#include <getopt.h>

///////////////////////////////////////////////////////////////////////////////

// win32 platform requires O_BINARY when using old open() calls so we make
// this unecessary bit-flag available as a harmless no-value on posix
#ifndef O_BINARY
#   define O_BINARY  0
#endif

///////////////////////////////////////////////////////////////////////////////

// initial namespace decl
namespace mp4v2 { namespace platform { namespace posix {
}}} // namespace mp4v2::platform::posix

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_PLATFORM_POSIX_POSIX_H
