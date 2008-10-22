#ifndef MP4V2_IMPL_UTIL_H
#define MP4V2_IMPL_UTIL_H

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

inline uint8  max( int8  a, int8  b ) { return ( a > b ) ? a : b; }
inline uint16 max( int16 a, int16 b ) { return ( a > b ) ? a : b; }
inline uint32 max( int32 a, int32 b ) { return ( a > b ) ? a : b; }
inline uint64 max( int64 a, int64 b ) { return ( a > b ) ? a : b; }

inline uint8  max( uint8  a, uint8  b ) { return ( a > b ) ? a : b; }
inline uint16 max( uint16 a, uint16 b ) { return ( a > b ) ? a : b; }
inline uint32 max( uint32 a, uint32 b ) { return ( a > b ) ? a : b; }
inline uint64 max( uint64 a, uint64 b ) { return ( a > b ) ? a : b; }

inline uint8  min( int8  a, int8  b ) { return ( a < b ) ? a : b; }
inline uint16 min( int16 a, int16 b ) { return ( a < b ) ? a : b; }
inline uint32 min( int32 a, int32 b ) { return ( a < b ) ? a : b; }
inline uint64 min( int64 a, int64 b ) { return ( a < b ) ? a : b; }

inline uint8  min( uint8  a, uint8  b ) { return ( a < b ) ? a : b; }
inline uint16 min( uint16 a, uint16 b ) { return ( a < b ) ? a : b; }
inline uint32 min( uint32 a, uint32 b ) { return ( a < b ) ? a : b; }
inline uint64 min( uint64 a, uint64 b ) { return ( a < b ) ? a : b; }

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

#endif // MP4V2_IMPL_UTIL_H
