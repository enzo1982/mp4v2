///////////////////////////////////////////////////////////////////////////////
//
//  The contents of this file are subject to the Mozilla Public License
//  Version 1.1 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.mozilla.org/MPL/
//
//  Software distributed under the License is distributed on an "AS IS"
//  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//  License for the specific language governing rights and limitations
//  under the License.
// 
//  The Original Code is MP4v2.
// 
//  The Initial Developer of the Original Code is Kona Blend.
//  Portions created by Kona Blend are Copyright (C) 2008.
//  All Rights Reserved.
//
//  Contributors:
//      Kona Blend, kona8lend@@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include "impl.h"

namespace mp4v2 { namespace impl { namespace itmf {

///////////////////////////////////////////////////////////////////////////////

Metadata::Metadata( MP4File& file_ )
    : file            ( file_ )
    , predefinedGenre ( GENRE_UNDEFINED )
{
}

///////////////////////////////////////////////////////////////////////////////

Metadata::Metadata( MP4FileHandle file_ )
    : file            ( *static_cast<MP4File*>(file_) )
    , predefinedGenre ( GENRE_UNDEFINED )
{
}

///////////////////////////////////////////////////////////////////////////////

Metadata::~Metadata()
{
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::c_alloc( MP4Metadata*& mdata )
{
    mdata = new MP4Metadata();
    memset( mdata, sizeof(Metadata), 0 ); // safe: pure C-struct
    mdata->__handle = this;
    c_fetch( mdata );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::c_fetch( MP4Metadata*& mdata )
{
    MP4Metadata& c = *mdata;

    fetchString( CODE_ALBUMNAME, albumName, c.albumName );
    fetchString( CODE_ARTIST,    artist,    c.artist );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::c_free( MP4Metadata*& mdata )
{
    MP4Metadata* m = const_cast<MP4Metadata*>(mdata);
    delete m;
    mdata = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::c_store( MP4Metadata*& mdata )
{
    MP4Metadata& c = *mdata;

    storeString( CODE_ALBUMNAME, albumName, c.albumName );
    storeString( CODE_ARTIST,    artist,    c.artist );
}

///////////////////////////////////////////////////////////////////////////////

bool
Metadata::fetchData( const string& code, uint8_t*& buffer, uint32_t& size )
{
    buffer = NULL;
    size = 0;

    MP4Atom& root = *file.FindAtom( NULL );

    ostringstream oss;
    oss << "moov.udta.meta.ilst." << code << ".data.metadata";

    MP4Property* prop;
    if( !root.FindProperty( oss.str().c_str(), &prop ))
        return true;

    if( prop->GetType() != BytesProperty )
        return true;

    MP4BytesProperty& data = *static_cast<MP4BytesProperty*>(prop);
    data.GetValue( &buffer, &size ); // our caller will free buffer

    return false;
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::fetchInteger( const string& code, uint8_t& cpp, const uint8_t*& c )
{
    cpp = 0;
    c = NULL;

    uint8_t* buffer;
    uint32_t size;
    if( fetchData( code, buffer, size ))
        return;

    cpp = buffer[0];
    c = &cpp;

    MP4Free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::fetchInteger( const string& code, uint32_t& cpp, const uint32_t*& c )
{
    cpp = 0;
    c = NULL;

    uint8_t* buffer; 
    uint32_t size;
    if( fetchData( code, buffer, size ))
        return;

    cpp = (uint32_t(buffer[3]) << 24)
        | (uint32_t(buffer[2]) << 16)
        | (uint32_t(buffer[1]) <<  8)
        | (uint32_t(buffer[0])      );

    c = &cpp;

    MP4Free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::fetchString( const string& code, string& cpp, const char*& c )
{
    cpp.clear();
    c = NULL;

    uint8_t* buffer;
    uint32_t size;
    if( fetchData( code, buffer, size ))
        return;

    cpp.append( reinterpret_cast<char*>(buffer), size );
    c = cpp.c_str();

    MP4Free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::setInteger( const uint8_t* value, uint8_t& cpp, const uint8_t*& c )
{
    if( !value ) {
        cpp = 0;
        c = NULL;
    }
    else {
        cpp = *value;
        c = &cpp;
    }
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::setInteger( const uint32_t* value, uint32_t& cpp, const uint32_t*& c )
{
    if( !value ) {
        cpp = 0;
        c = NULL;
    }
    else {
        cpp = *value;
        c = &cpp;
    }
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::setString( const char* value, string& cpp, const char*& c )
{
    if( !value ) {
        cpp.clear();
        c = NULL;
    }
    else {
        cpp = value;
        c = cpp.c_str();
    }
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::storeInteger( const string& code, uint8_t cpp, const uint8_t* c )
{
    if( c )
        file.SetMetadataUint8( code.c_str(), cpp );
    else
        file.DeleteMetadataAtom( code.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::storeInteger( const string& code, uint32_t cpp, const uint32_t* c )
{
    if( c )
        file.SetMetadataUint32( code.c_str(), cpp );
    else
        file.DeleteMetadataAtom( code.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void
Metadata::storeString( const string& code, const string& cpp, const char* c )
{
    if( c )
        file.SetMetadataString( code.c_str(), cpp.c_str() );
    else
        file.DeleteMetadataAtom( code.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

const string Metadata::CODE_ALBUMNAME = "\xa9" "alb";
const string Metadata::CODE_ARTIST    = "\xa9" "ART";

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::itmf
