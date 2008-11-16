#ifndef MP4V2_PLATFORM_PLATFORM_H
#define MP4V2_PLATFORM_PLATFORM_H

///////////////////////////////////////////////////////////////////////////////

#if defined( _WIN32 )
#   include "libplatform/platform_win32.h"
#else
#   include "libplatform/platform_posix.h"
#endif

///////////////////////////////////////////////////////////////////////////////

#include "libplatform/warning.h"
#include "libplatform/endian.h"

//! @defgroup platform_io libplatform I/O
#include "libplatform/io/File.h"
#include "libplatform/io/FileSystem.h"

#include "libplatform/number/random.h"
#include "libplatform/process/process.h"
#include "libplatform/prog/option.h"
#include "libplatform/sys/error.h"
#include "libplatform/time/time.h"

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_PLATFORM_PLATFORM_H
