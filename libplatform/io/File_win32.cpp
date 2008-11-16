#include "libplatform/impl.h"
#include <io.h>

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getPosition( Size& pos_ )
{
    pos_ = _ftelli64( _handle );
    return pos_ == -1;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getSize( Size& size_ )
{
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
    return _fseeki64( _handle, pos_, SEEK_SET ) != 0;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
