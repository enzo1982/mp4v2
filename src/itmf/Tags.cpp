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

Tags::Tags()
{
}

///////////////////////////////////////////////////////////////////////////////

Tags::~Tags()
{
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_alloc( MP4Tags*& mdata )
{
    mdata = new MP4Tags();
    memset( mdata, sizeof(Tags), 0 ); // safe: pure C-struct
    mdata->__handle = this;
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_fetch( MP4Tags*& mdata, MP4FileHandle hFile )
{
    MP4Tags& c = *mdata;
    MP4File& file = *static_cast<MP4File*>(hFile);

    fetchString( file, CODE_NAME,         name,         c.name );
    fetchString( file, CODE_ARTIST,       artist,       c.artist );
    fetchString( file, CODE_ALBUMARTIST,  albumArtist,  c.albumArtist );
    fetchString( file, CODE_ALBUM,        album,        c.album );
    fetchString( file, CODE_GROUPING,     grouping,     c.grouping );
    fetchString( file, CODE_COMPOSER,     composer,     c.composer );
    fetchString( file, CODE_COMMENTS,     comments,     c.comments );

    fetchGenre( file, genre, c.genre );

    fetchString  ( file, CODE_RELEASEDATE,     releaseDate,     c.releaseDate );
    fetchInteger ( file, CODE_BEATSPERMINUTE,  beatsPerMinute,  c.beatsPerMinute );
    fetchInteger ( file, CODE_COMPILATION,     compilation,     c.compilation );

    fetchString  ( file, CODE_TVSHOW,       tvShow,       c.tvShow );
    fetchString  ( file, CODE_TVNETWORK,    tvNetwork,    c.tvNetwork );
    fetchString  ( file, CODE_TVEPISODEID,  tvEpisodeID,  c.tvEpisodeID );
    fetchInteger ( file, CODE_TVSEASON,     tvSeason,     c.tvSeason );
    fetchInteger ( file, CODE_TVEPISODE,    tvEpisode,    c.tvEpisode );

    fetchString( file, CODE_SORTNAME,         sortName,         c.sortName );
    fetchString( file, CODE_SORTARTIST,       sortArtist,       c.sortArtist );
    fetchString( file, CODE_SORTALBUMARTIST,  sortAlbumArtist,  c.sortAlbumArtist );
    fetchString( file, CODE_SORTALBUM,        sortAlbum,        c.sortAlbum );
    fetchString( file, CODE_SORTCOMPOSER,     sortComposer,     c.sortComposer );
    fetchString( file, CODE_SORTTVSHOW,       sortTVShow,       c.sortTVShow );

    fetchString( file, CODE_DESCRIPTION,      description,      c.description );
    fetchString( file, CODE_LONGDESCRIPTION,  longDescription,  c.longDescription );
    fetchString( file, CODE_LYRICS,           lyrics,           c.lyrics );

    fetchString( file, CODE_COPYRIGHT,     copyright,     c.copyright );
    fetchString( file, CODE_ENCODINGTOOL,  encodingTool,  c.encodingTool ); 
    fetchString( file, CODE_ENCODEDBY,     encodedBy,     c.encodedBy );
    fetchString( file, CODE_PURCHASEDATE,  purchaseDate,  c.purchaseDate );

    fetchString  ( file, CODE_CATEGORY,  category,  c.category );
    fetchString  ( file, CODE_KEYWORDS,  keywords,  c.keywords );
    fetchInteger ( file, CODE_PODCAST,   podcast,   c.podcast );
    
    fetchInteger( file, CODE_HDVIDEO,        hdVideo,        c.hdVideo );
    fetchInteger( file, CODE_MEDIATYPE,      mediaType,      c.mediaType );
    fetchInteger( file, CODE_CONTENTRATING,  contentRating,  c.contentRating );
    fetchInteger( file, CODE_GAPLESS,        gapless,        c.gapless );

    fetchString  ( file, CODE_ITUNESACCOUNT,  iTunesAccount,  c.iTunesAccount );
    fetchInteger ( file, CODE_CNID,           cnID,           c.cnID );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_free( MP4Tags*& mdata )
{
    MP4Tags* m = const_cast<MP4Tags*>(mdata);
    delete m;
    mdata = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_store( MP4Tags*& mdata, MP4FileHandle hFile )
{
    MP4Tags& c = *mdata;
    MP4File& file = *static_cast<MP4File*>(hFile);
   
    storeString( file, CODE_NAME,         name,         c.name );
    storeString( file, CODE_ARTIST,       artist,       c.artist );
    storeString( file, CODE_ALBUMARTIST,  albumArtist,  c.albumArtist );
    storeString( file, CODE_ALBUM,        album,        c.album );
    storeString( file, CODE_GROUPING,     grouping,     c.grouping );
    storeString( file, CODE_COMPOSER,     composer,     c.composer );
    storeString( file, CODE_COMMENTS,     comments,     c.comments );
    storeString( file, CODE_RELEASEDATE,  releaseDate,  c.releaseDate );
    
    storeString( file, CODE_DESCRIPTION, description, c.description );

    storeString( file, CODE_COPYRIGHT,     copyright,     c.copyright );
    storeString( file, CODE_ENCODINGTOOL,  encodingTool,  c.encodingTool );
    storeString( file, CODE_ENCODEDBY,     encodedBy,     c.encodedBy );
}

///////////////////////////////////////////////////////////////////////////////

bool
Tags::fetchData( MP4File& file, const string& code, uint8_t*& buffer, uint32_t& size )
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
Tags::fetchGenre( MP4File& file, string& cpp, const char*& c )
{
    cpp.clear();
    c = NULL;

    char* value;
    file.GetMetadataGenre( &value );

    cpp = value;
    c = cpp.c_str();

    free( value );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::fetchInteger( MP4File& file, const string& code, uint8_t& cpp, const uint8_t*& c )
{
    cpp = 0;
    c = NULL;

    uint8_t* buffer;
    uint32_t size;
    if( fetchData( file, code, buffer, size ))
        return;

    cpp = buffer[0];
    c = &cpp;

    MP4Free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::fetchInteger( MP4File& file, const string& code, uint16_t& cpp, const uint16_t*& c )
{
    cpp = 0;
    c = NULL;

    uint8_t* buffer; 
    uint32_t size;
    if( fetchData( file, code, buffer, size ))
        return;

    cpp = (uint16_t(buffer[0]) <<  8)
        | (uint16_t(buffer[1])      );

    c = &cpp;

    MP4Free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::fetchInteger( MP4File& file, const string& code, uint32_t& cpp, const uint32_t*& c )
{
    cpp = 0;
    c = NULL;

    uint8_t* buffer; 
    uint32_t size;
    if( fetchData( file, code, buffer, size ))
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
Tags::fetchString( MP4File& file, const string& code, string& cpp, const char*& c )
{
    cpp.clear();
    c = NULL;

    uint8_t* buffer;
    uint32_t size;
    if( fetchData( file, code, buffer, size ))
        return;

    cpp.append( reinterpret_cast<char*>(buffer), size );
    c = cpp.c_str();

    MP4Free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::setInteger( const uint8_t* value, uint8_t& cpp, const uint8_t*& c )
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
Tags::setInteger( const uint32_t* value, uint32_t& cpp, const uint32_t*& c )
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
Tags::setString( const char* value, string& cpp, const char*& c )
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
Tags::storeInteger( MP4File& file, const string& code, uint8_t cpp, const uint8_t* c )
{
    if( c )
        file.SetMetadataUint8( code.c_str(), cpp );
    else
        file.DeleteMetadataAtom( code.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::storeInteger( MP4File& file, const string& code, uint32_t cpp, const uint32_t* c )
{
    if( c )
        file.SetMetadataUint32( code.c_str(), cpp );
    else
        file.DeleteMetadataAtom( code.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::storeString( MP4File& file, const string& code, const string& cpp, const char* c )
{
    if( c )
        file.SetMetadataString( code.c_str(), cpp.c_str() );
    else
        file.DeleteMetadataAtom( code.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

const string Tags::CODE_NAME            = "\xa9" "nam";
const string Tags::CODE_ARTIST          = "\xa9" "ART";
const string Tags::CODE_ALBUMARTIST     = "aART";
const string Tags::CODE_ALBUM           = "\xa9" "alb";
const string Tags::CODE_GROUPING        = "\xa9" "grp";
const string Tags::CODE_COMPOSER        = "\xa9" "wrt";
const string Tags::CODE_COMMENTS        = "\xa9" "cmt";
const string Tags::CODE_RELEASEDATE     = "\xa9" "day";
const string Tags::CODE_BEATSPERMINUTE  = "tmpo";
const string Tags::CODE_COMPILATION     = "cpil";

const string Tags::CODE_TVSHOW          = "tvsh";
const string Tags::CODE_TVNETWORK       = "tvnn";
const string Tags::CODE_TVEPISODEID     = "tven";
const string Tags::CODE_TVSEASON        = "tvsn";
const string Tags::CODE_TVEPISODE       = "tves";

const string Tags::CODE_DESCRIPTION     = "desc";
const string Tags::CODE_LONGDESCRIPTION = "ldes";
const string Tags::CODE_LYRICS          = "\xa9" "lyr";

const string Tags::CODE_SORTNAME        = "sonm";
const string Tags::CODE_SORTARTIST      = "soar";
const string Tags::CODE_SORTALBUMARTIST = "soaa";
const string Tags::CODE_SORTALBUM       = "soal";
const string Tags::CODE_SORTCOMPOSER    = "soco";
const string Tags::CODE_SORTTVSHOW      = "sosn";

const string Tags::CODE_COPYRIGHT       = "cprt";
const string Tags::CODE_ENCODINGTOOL    = "\xa9" "too";
const string Tags::CODE_ENCODEDBY       = "\xa9" "enc";
const string Tags::CODE_PURCHASEDATE    = "purd";

const string Tags::CODE_CATEGORY        = "catg";
const string Tags::CODE_KEYWORDS        = "keyw";
const string Tags::CODE_PODCAST         = "pcst";

const string Tags::CODE_HDVIDEO         = "hdvd";
const string Tags::CODE_MEDIATYPE       = "stik";
const string Tags::CODE_CONTENTRATING   = "rtng";
const string Tags::CODE_GAPLESS         = "pgap";

const string Tags::CODE_ITUNESACCOUNT   = "apID";
const string Tags::CODE_CNID            = "cnID";

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::itmf
