#include "libplatform/impl.h"

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

bool
FileSystem::tempFilename( string& name_, string dir_, string prefix_, string suffix_ )
{
    ostringstream buf;
    buf << dir_;

    // add dir separator if needed
    if( !dir_.empty() ) {
        buf << dir_;
        if( dir_[dir_.length()-1] != '/' )
            buf << '/';
    }

    buf << prefix_;
    buf << process::getpid();
    buf << suffix_;

    name_ = buf.str();
    return false;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
