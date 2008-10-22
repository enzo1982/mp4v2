#include "impl.h"
#include <sys/timeb.h>

namespace mp4v2 { namespace platform { namespace win32 {

///////////////////////////////////////////////////////////////////////////////

int
gettimeofday( struct timeval* tp, void* tzp )
{
    struct _timeb buf;
    _ftime( &buf );
    tp->tv_sec  = buf.time;
    tp->tv_usec = buf.millitm * 1000;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::win32
