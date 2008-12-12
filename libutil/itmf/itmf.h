#ifndef MP4V2_UTIL_ITMF_ITMF_H
#define MP4V2_UTIL_ITMF_ITMF_H

/// @namespace mp4v2::util::itmf (private) iTunes Metadata Format.
/// <b>WARNING: THIS IS A PRIVATE NAMESPACE. NOT FOR PUBLIC CONSUMPTION.</b>
///
/// This namespace implements some features that are specified by the
/// iTunes Metadata Format Specification, revision 2008-04-16.
///
/// This namespace provides a rich C++ interface, and is not intended to be
/// exposed as public API in its current state, especially given its curren
/// parent namespace.
///
/// The dependents of this module are libutil/utilities.
///
namespace mp4v2 { namespace util { namespace itmf {
    using namespace std;
    using namespace mp4v2::impl;
}}}

///////////////////////////////////////////////////////////////////////////////

#include "libutil/util.h"

#include "libutil/itmf/type.h"
#include "libutil/itmf/CoverArtBox.h"

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_UTIL_ITMF_ITMF_H
