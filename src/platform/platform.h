#ifndef MP4V2_PLATFORM_PLATFORM_H
#define MP4V2_PLATFORM_PLATFORM_H

///////////////////////////////////////////////////////////////////////////////

#include "platform/config.h"

///////////////////////////////////////////////////////////////////////////////

// constant macros are not usually used with C++ so the standard mechanism to
// activate it is to define before stdint.h
#ifndef __STDC_CONSTANT_MACROS
#   define __STDC_CONSTANT_MACROS
#endif

// format macros are not usually used with C++ so the standard mechanism to
// activate it is to define before inttypes.h
#ifndef __STDC_FORMAT_MACROS
#   define __STDC_FORMAT_MACROS
#endif

#ifdef NEED_LFS_ACTIVATION
#   ifndef _LARGEFILE_SOURCE
#       define _LARGEFILE_SOURCE
#       define _FILE_OFFSET_BITS 64
#   endif
#endif

///////////////////////////////////////////////////////////////////////////////

// C++ standard-C includes are portable enough to generally avoid placement
// into platform-specific files

#include <cassert>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

///////////////////////////////////////////////////////////////////////////////

#if defined( _WIN32 )
#   include "win32/win32.h"
#else
#   include "posix/posix.h"
#endif

///////////////////////////////////////////////////////////////////////////////

namespace mp4v2 { namespace platform {
#if defined( _WIN32 )
    using namespace mp4v2::platform::win32;
#else
    using namespace mp4v2::platform::posix;
#endif
}} // namespace mp4v2::platform

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_PLATFORM_PLATFORM_H
