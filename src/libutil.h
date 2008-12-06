#ifndef MP4V2_IMPL_LIBUTIL_H
#define MP4V2_IMPL_LIBUTIL_H

#include <mp4v2/mp4v2.h>

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "itmf.h"
#include "qtff.h"
#include "subtitle.h"

/// @namespace mp4v2::util (private) Command-line utility support.
/// <b>WARNING: THIS IS A PRIVATE NAMESPACE. NOT FOR PUBLIC CONSUMPTION.</b>
///
/// This namespace is used for command-line utilities. Some symbols from this
/// namespace are exported from libmp4v2 in order to support new functionality
/// which may or may not make it into some form of public API, at which time
/// it will be moved out of this namespace.

///////////////////////////////////////////////////////////////////////////////

namespace mp4v2 { namespace util {
    using namespace std;

///////////////////////////////////////////////////////////////////////////////

struct FileSummaryInfo {
    typedef set<string>  BrandSet;

    // standard ftyp box attributes
    string   major_brand;
    uint32_t minor_version;
    BrandSet compatible_brands;

    uint32_t nlargesize;
    uint32_t nversion1;
    uint32_t nspecial;
};

/// Fetch mp4 file summary information.
///
/// This function fetches summary information for <b>file</b> and information
/// is stored in <b>info</b>.
///
/// @return On success <b>true</b>.
///     On failure <b>false</b>, and contents of <b>info</b> are undefined.
///
MP4V2_EXPORT
bool fileFetchSummaryInfo( MP4FileHandle file, FileSummaryInfo& info );

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util

#endif // MP4V2_IMPL_LIBUTIL_H
