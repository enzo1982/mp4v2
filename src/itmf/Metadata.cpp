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

    fetchString( CODE_NAME, name, c.name );
    fetchString( CODE_ARTIST,    artist,    c.artist );
    fetchString( CODE_ALBUMARTIST,    albumArtist,    c.albumArtist );
    fetchString( CODE_ALBUM, album, c.album );
    fetchString( CODE_GROUPING,    grouping,    c.grouping );
    fetchString( CODE_COMPOSER,    composer,    c.composer );
    fetchString( CODE_COMMENTS,    comments,    c.comments );
    fetchString( CODE_RELEASEDATE,    releaseDate,    c.releaseDate );
    fetchInteger( CODE_COMPILATION,    compilation,    c.compilation );

    fetchString( CODE_TVSHOW,    tvShow,    c.tvShow );
    fetchString( CODE_TVNETWORK,    tvNetwork,    c.tvNetwork );
    fetchString( CODE_TVEPISODEID,    tvEpisodeID,    c.tvEpisodeID );
    fetchInteger( CODE_TVSEASON,    tvSeason,    c.tvSeason );
    fetchInteger( CODE_TVEPISODE,    tvEpisode,    c.tvEpisode );

    fetchString( CODE_SORTNAME, sortName, c.sortName );
    fetchString( CODE_SORTARTIST,    sortArtist,    c.sortArtist );
    fetchString( CODE_SORTALBUMARTIST,    sortAlbumArtist,    c.sortAlbumArtist );
    fetchString( CODE_SORTALBUM, sortAlbum, c.sortAlbum );
    fetchString( CODE_SORTCOMPOSER,    sortComposer,    c.sortComposer );
    fetchString( CODE_SORTTVSHOW,    sortTVShow,    c.sortTVShow );
    
    fetchString( CODE_DESCRIPTION,    description,    c.description );
    fetchString( CODE_LONGDESCRIPTION,    longDescription,    c.longDescription );
    fetchString( CODE_LYRICS,    lyrics,    c.lyrics );

    fetchString( CODE_COPYRIGHT,    copyright,    c.copyright );
    fetchString( CODE_ENCODINGTOOL,    encodingTool,    c.encodingTool ); 
    fetchString( CODE_ENCODEDBY,    encodedBy,    c.encodedBy );
    fetchString( CODE_PURCHASEDATE,    purchaseDate,    c.purchaseDate );
        
    fetchString( CODE_CATEGORY,    category,    c.category );
    fetchString( CODE_KEYWORDS,    keywords,    c.keywords );
    fetchInteger( CODE_PODCAST,    podcast,    c.podcast );
    
    fetchInteger( CODE_HDVIDEO,    hdVideo,    c.hdVideo );
    fetchInteger( CODE_MEDIATYPE,    mediaType,    c.mediaType );
    fetchInteger( CODE_CONTENTRATING,    contentRating,    c.contentRating );
    fetchInteger( CODE_GAPLESS,    gapless,    c.gapless );

    fetchString( CODE_ITUNESACCOUNT,    iTunesAccount,    c.iTunesAccount );
    fetchInteger( CODE_CNID,    cnID,    c.cnID );
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
   
    storeString( CODE_NAME, name, c.name );
    storeString( CODE_ARTIST,    artist,    c.artist );
    storeString( CODE_ALBUMARTIST,    albumArtist,    c.albumArtist );
    storeString( CODE_ALBUM, album, c.album );
    storeString( CODE_GROUPING,    grouping,    c.grouping );
    storeString( CODE_COMPOSER,    composer,    c.composer );
    storeString( CODE_COMMENTS,    comments,    c.comments );
    storeString( CODE_RELEASEDATE,    releaseDate,    c.releaseDate );
    
    storeString( CODE_DESCRIPTION,    description,    c.description );

    storeString( CODE_COPYRIGHT,    copyright,    c.copyright );
    storeString( CODE_ENCODINGTOOL,    encodingTool,    c.encodingTool );
    storeString( CODE_ENCODEDBY,    encodedBy,    c.encodedBy );
    
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

    cpp = (uint32_t(buffer[0]) << 24)
        | (uint32_t(buffer[1]) << 16)
        | (uint32_t(buffer[2]) <<  8)
        | (uint32_t(buffer[3])      );

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

const string Metadata::CODE_NAME            = "\xa9" "nam";
const string Metadata::CODE_ARTIST          = "\xa9" "ART";
const string Metadata::CODE_ALBUMARTIST     = "aART";
const string Metadata::CODE_ALBUM           = "\xa9" "alb";
const string Metadata::CODE_GROUPING        = "\xa9" "grp";
const string Metadata::CODE_COMPOSER        = "\xa9" "wrt";
const string Metadata::CODE_COMMENTS        = "\xa9" "cmt";
const string Metadata::CODE_RELEASEDATE     = "\xa9" "day";
const string Metadata::CODE_COMPILATION     = "cpil";

const string Metadata::CODE_TVSHOW          = "tvsh";
const string Metadata::CODE_TVNETWORK       = "tvnn";
const string Metadata::CODE_TVEPISODEID     = "tven";
const string Metadata::CODE_TVSEASON        = "tvsn";
const string Metadata::CODE_TVEPISODE       = "tves";

const string Metadata::CODE_DESCRIPTION     = "desc";
const string Metadata::CODE_LONGDESCRIPTION = "ldes";
const string Metadata::CODE_LYRICS          = "\xa9" "lyr";

const string Metadata::CODE_SORTNAME        = "sonm";
const string Metadata::CODE_SORTARTIST      = "soar";
const string Metadata::CODE_SORTALBUMARTIST = "soaa";
const string Metadata::CODE_SORTALBUM       = "soal";
const string Metadata::CODE_SORTCOMPOSER    = "soco";
const string Metadata::CODE_SORTTVSHOW      = "sosn";

const string Metadata::CODE_COPYRIGHT       = "cprt";
const string Metadata::CODE_ENCODINGTOOL    = "\xa9" "too";
const string Metadata::CODE_ENCODEDBY       = "\xa9" "enc";
const string Metadata::CODE_PURCHASEDATE    = "purd";

const string Metadata::CODE_CATEGORY        = "catg";
const string Metadata::CODE_KEYWORDS        = "keyw";
const string Metadata::CODE_PODCAST         = "pcst";

const string Metadata::CODE_HDVIDEO         = "hdvd";
const string Metadata::CODE_MEDIATYPE       = "stik";
const string Metadata::CODE_CONTENTRATING   = "rtng";
const string Metadata::CODE_GAPLESS         = "pgap";

const string Metadata::CODE_ITUNESACCOUNT   = "apID";
const string Metadata::CODE_CNID            = "cnID";

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::itmf
