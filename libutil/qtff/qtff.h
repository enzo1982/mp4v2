#ifndef MP4V2_UTIL_QTFF_QTFF_H
#define MP4V2_UTIL_QTFF_QTFF_H

/// @namespace mp4v2::util::qtff (private) QuickTime File Format.
/// <b>WARNING: THIS IS A PRIVATE NAMESPACE. NOT FOR PUBLIC CONSUMPTION.</b>
///
/// This namespace implements some features that are specified by the
/// QuickTime File Format Specification, revision 2007-09-04.
///
/// This namespace provides a rich C++ interface, and is not intended to be
/// exposed as public API in its current state, especially given its curren
/// parent namespace.
///
namespace mp4v2 { namespace util { namespace qtff {
    using namespace std;
    using namespace mp4v2::impl;
}}}

///////////////////////////////////////////////////////////////////////////////

#include "libutil/util.h"

#include "libutil/qtff/ColorParameterBox.h"
#include "libutil/qtff/PictureAspectRatioBox.h"
#include "libutil/qtff/coding.h"

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_UTIL_QTTF_QTFF_H
