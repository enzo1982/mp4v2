#ifndef MP4V2_UTIL_UTIL_H
#define MP4V2_UTIL_UTIL_H

///////////////////////////////////////////////////////////////////////////////

#include "src/impl.h"

///////////////////////////////////////////////////////////////////////////////

/// @namespace mp4v2::util (private) Command-line utility support.
/// <b>WARNING: THIS IS A PRIVATE NAMESPACE. NOT FOR PUBLIC CONSUMPTION.</b>
///
/// This namespace is used for command-line utilities. Some symbols from this
/// namespace are exported from libmp4v2 in order to support new functionality
/// which may or may not make it into some form of public API, at which time
/// it will be moved out of this namespace.
///
namespace mp4v2 { namespace util {
    using namespace std;
    using namespace platform;

    //< static init which must be called to init libutil @ runtime begin.
    MP4V2_EXPORT
    void sinit();

    //< static shutdown which must be called to cleanup libutil @ runtime end.
    MP4V2_EXPORT
    void sshutdown();
}} // namespace mp4v2::util

///////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "UtilException.h"
#include "Database.h"
#include "Timecode.h"
#include "Utility.h"
#include "crc.h"
#include "other.h"

#include "itmf/itmf.h"
#include "qtff/qtff.h"

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_UTIL_UTIL_H
