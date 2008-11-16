#include "libplatform/impl.h"
#include <sys/stat.h>

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getPosition( Size& pos_ )
{
    pos_ = ftello( _handle );
    return pos_ == -1;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getSize( Size& size_ )
{
    struct stat buf;
    if( fstat( fileno( _handle ), &buf ))
        return true;
    size_ = buf.st_size;
    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::setPosition( Size pos_ )
{
    return fseeko( _handle, pos_, SEEK_SET ) != 0;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
