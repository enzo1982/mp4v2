#include "libplatform/impl.h"

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

namespace {
    const File::Size __maxChunkSize = 1024*1024;
}

///////////////////////////////////////////////////////////////////////////////

File::File( std::string name_, bool owner_ )
    : _name  ( name_ )
    , _owner ( owner_ )
    , name   ( _name )
{
}

///////////////////////////////////////////////////////////////////////////////

File::~File()
{
}

///////////////////////////////////////////////////////////////////////////////

StdioFile::StdioFile( std::string name_, bool owner_ )
    : File    ( name_, owner_ )
    , _handle ( 0 )
    , handle  ( _handle )
{
}

///////////////////////////////////////////////////////////////////////////////

StdioFile::StdioFile( Handle handle_, bool owner_ )
    : File    ( "", owner_ )
    , _handle ( handle_ )
    , handle  ( _handle )
{
}

///////////////////////////////////////////////////////////////////////////////

StdioFile::~StdioFile()
{
    close();
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::close()
{
    if( !_handle )
        return false;

    if( !_owner ) {
        _handle = 0;
        return false;
    }

    bool result = std::fclose( _handle ) != 0;
    _handle = 0;
    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getPosition( Handle handle_, Size& pos_ )
{
    return StdioFile( handle_ ).getPosition( pos_ );
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::getSize( Handle handle_, Size& size_ )
{
    return StdioFile( handle_ ).getSize( size_ );
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::open( string mode_ )
{
    close();
    _handle = std::fopen( _name.c_str(), mode_.c_str() );
    return _handle != 0;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::open( string mode_, string name_ )
{
    _name = name_;
    return open( mode_ );
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::read( void* buffer_, Size size_, Size& nin_, Size maxChunkSize_ )
{
    nin_ = 0;

    if( !maxChunkSize_ )
        maxChunkSize_ = __maxChunkSize;

    char* const pmax = (char*)buffer_ + size_;
    for ( char* p = (char*)buffer_; p < pmax; p += maxChunkSize_ ) {
        ptrdiff_t psize = pmax - p;
        if( psize > maxChunkSize_ )
            psize = maxChunkSize_;

        if( std::fread( p, psize, 1, _handle ) != 1)
            return true;

        nin_ += psize;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::setPosition( Handle handle_, Size pos_ )
{
    return StdioFile( handle_ ).setPosition( pos_ );
}

///////////////////////////////////////////////////////////////////////////////

bool
StdioFile::write( const void* buffer_, Size size_, Size& nout_, Size maxChunkSize_ )
{
    nout_ = 0;

    if( !maxChunkSize_ )
        maxChunkSize_ = __maxChunkSize;

    char* const pmax = (char*)buffer_ + size_;
    for ( char* p = (char*)buffer_; p < pmax; p += maxChunkSize_ ) {
        ptrdiff_t psize = pmax - p;
        if( psize > maxChunkSize_ )
            psize = maxChunkSize_;

        if( std::fwrite( p, psize, 1, _handle ) != 1)
            return true;

        nout_ += psize;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
