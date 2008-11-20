#include "libplatform/impl.h"

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

bool
FileSystem::exists( string path_ )
{
    WIN32_FIND_DATAA buf;
    HANDLE h = FindFirstFileA( path_.c_str(), &buf );
    if( h == INVALID_HANDLE_VALUE )
        return false;
    FindClose( h );
    return true;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileSystem::isDirectory( string path_ )
{
    WIN32_FIND_DATAA buf;
    HANDLE h = FindFirstFileA( path_.c_str(), &buf );
    if( h == INVALID_HANDLE_VALUE )
        return false;
    FindClose( h );
    return (buf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileSystem::isFile( string path_ )
{
    WIN32_FIND_DATAA buf;
    HANDLE h = FindFirstFileA( path_.c_str(), &buf );
    if( h == INVALID_HANDLE_VALUE )
        return false;
    FindClose( h );
    return (buf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileSystem::rename( string from, string to )
{
    if( exists( to )) {
        if( ::remove( to.c_str() ))
            return true;
    }
    return ::rename( from.c_str(), to.c_str() ) != 0;
}

///////////////////////////////////////////////////////////////////////////////

string FileSystem::DIR_SEPARATOR  = "\\";
string FileSystem::PATH_SEPARATOR = ";";

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
