#ifndef MP4V2_IMPL_TYPE_H
#define MP4V2_IMPL_TYPE_H

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

// TODO-KB: cleanup
// this is experimental -- since we have our own namespace it might just be
// a good idea to change C++ source code to use some consistent types
#if 1
typedef ::int8_t  int8;
typedef ::int16_t int16;
typedef ::int32_t int32;
typedef ::int64_t int64;

typedef ::uint8_t  uint8;
typedef ::uint16_t uint16;
typedef ::uint32_t uint32;
typedef ::uint64_t uint64;

using std::size_t;
using std::ptrdiff_t;
#endif

///////////////////////////////////////////////////////////////////////////////

}
} // namespace mp4v2::impl

#endif // MP4V2_IMPL_TYPE_H
