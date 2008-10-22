#ifndef MP4V2_PLATFORM_WIN32_GETTIMEOFDAY_H
#define MP4V2_PLATFORM_WIN32_GETTIMEOFDAY_H

namespace mp4v2 { namespace platform { namespace win32 {

//////////////////////////////////////////////////////////////////////////////

int gettimeofday( struct timeval* tp, void* tzp );

//////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::win32

#endif // MP4V2_PLATFORM_WIN32_GETTIMEOFDAY_H
