#ifndef MP4V2_UTIL_QTFF_CODING_H
#define MP4V2_UTIL_QTFF_CODING_H

namespace mp4v2 { namespace util { namespace qtff {

///////////////////////////////////////////////////////////////////////////////

bool findCoding( MP4FileHandle file, uint16_t trackIndex, MP4Atom*& coding );

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::qtff

#endif // MP4V2_UTIL_QTTF_CODING_H
