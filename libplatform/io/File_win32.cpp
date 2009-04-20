#include "libplatform/impl.h"
#include <io.h>

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getPosition( Size& pos_ )
{
    fflush( _handle ); // must flush because we're using _fileno routines
    pos_ = _telli64( _fileno( _handle ));
    return pos_ == -1;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getSize( Size& size_ )
{
    fflush( _handle ); // must flush because we're using _fileno routines
    LARGE_INTEGER n;
    if( !GetFileSizeEx( (HANDLE)_get_osfhandle( _fileno( _handle )), &n ))
        return true;
    size_ = n.QuadPart;
    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::setPosition( Size pos_ )
{
    fflush( _handle ); // must flush because we're using _fileno routines
    return _lseeki64( _fileno( _handle ), pos_, SEEK_SET ) == -1;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
