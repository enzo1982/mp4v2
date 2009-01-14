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
Tags::c_addArtwork( MP4Tags*& tags, MP4TagArtwork& c_artwork )
{
    artwork.resize( artwork.size() + 1 );
    c_setArtwork( tags, artwork.size() - 1, c_artwork );
    updateArtworkShadow( tags );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_alloc( MP4Tags*& tags )
{
    tags = new MP4Tags();
    memset( tags, sizeof(Tags), 0 ); // safe: pure C-struct
    tags->__handle = this;
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_fetch( MP4Tags*& tags, MP4FileHandle hFile )
{
    MP4Tags& c = *tags;
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

    // fetch full list and overwrite our copy, otherwise clear
    {
        CoverArtBox::ItemList items;
        if( CoverArtBox::list( hFile, items ))
            artwork.clear();
        else
            artwork = items;

        updateArtworkShadow( tags );
    }
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_free( MP4Tags*& tags )
{
    MP4Tags* c = const_cast<MP4Tags*>(tags);

    //delete[] c->artwork;
    delete c;

    tags = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_removeArtwork( MP4Tags*& tags, uint32_t index ) 
{
    if( !(index < artwork.size()) )
        return;

    artwork.erase( artwork.begin() + index );
    updateArtworkShadow( tags );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_setArtwork( MP4Tags*& tags, uint32_t index, MP4TagArtwork& c_artwork )
{
    if( !(index < artwork.size()) )
        return;

    CoverArtBox::Item& item = artwork[index];

    switch( c_artwork.type ) {
        case MP4_ART_BMP:
            item.type = BT_BMP;
            break;

        case MP4_ART_GIF:
            item.type = BT_GIF;
            break;

        case MP4_ART_JPEG:
            item.type = BT_JPEG;
            break;

        case MP4_ART_PNG:
            item.type = BT_PNG;
            break;

        case MP4_ART_UNDEFINED:
        default:
            item.type = computeBasicType( c_artwork.data, c_artwork.size );
            break;
    }

    item.buffer   = (uint8_t*)malloc( c_artwork.size );
    item.size     = c_artwork.size;
    item.autofree = true;

    memcpy( item.buffer, c_artwork.data, c_artwork.size );
    updateArtworkShadow( tags );
}

///////////////////////////////////////////////////////////////////////////////

void
Tags::c_setInteger( const uint8_t* value, uint8_t& cpp, const uint8_t*& c )
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
Tags::c_setInteger( const uint32_t* value, uint32_t& cpp, const uint32_t*& c )
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
Tags::c_setString( const char* value, string& cpp, const char*& c )
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
Tags::c_store( MP4Tags*& tags, MP4FileHandle hFile )
{
    MP4Tags& c = *tags;
    MP4File& file = *static_cast<MP4File*>(hFile);
   
    storeString( file, CODE_NAME,         name,         c.name );
    storeString( file, CODE_ARTIST,       artist,       c.artist );
    storeString( file, CODE_ALBUMARTIST,  albumArtist,  c.albumArtist );
    storeString( file, CODE_ALBUM,        album,        c.album );
    storeString( file, CODE_GROUPING,     grouping,     c.grouping );
    storeString( file, CODE_COMPOSER,     composer,     c.composer );
    storeString( file, CODE_COMMENTS,     comments,     c.comments );
    storeGenre( file, genre, c.genre );
    storeString( file, CODE_RELEASEDATE,  releaseDate,  c.releaseDate );
    
    storeString( file, CODE_DESCRIPTION, description, c.description );

    storeString( file, CODE_COPYRIGHT,     copyright,     c.copyright );
    storeString( file, CODE_ENCODINGTOOL,  encodingTool,  c.encodingTool );
    storeString( file, CODE_ENCODEDBY,     encodedBy,     c.encodedBy );

    // destroy all cover-art then add each
    {
        CoverArtBox::remove( hFile );
        const CoverArtBox::ItemList::size_type max = artwork.size();
        for( CoverArtBox::ItemList::size_type i = 0; i < max; i++ )
            CoverArtBox::add( hFile, artwork[i] );
    }
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
    try {
        file.GetMetadataGenre( &value );
    }
    catch( MP4Error* e ) {
        delete e;
        value = NULL;
    }

    if ( value != NULL ) { 
       cpp = value;
       c = cpp.c_str();    
    } else {
        cpp.clear();
        c = NULL;
    }

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
Tags::storeGenre( MP4File& file, const string& cpp, const char* c )
{
    if( c )
        file.SetMetadataGenre( cpp.c_str() );
    else
        file.DeleteMetadataGenre();
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

void
Tags::updateArtworkShadow( MP4Tags*& tags )
{
    if( tags->artwork ) {
        delete[] tags->artwork;
        tags->artwork = NULL;
        tags->artworkCount = 0;
    }

    if( artwork.empty() )
        return;

    MP4TagArtwork* const cartwork = new MP4TagArtwork[ artwork.size() ];
    uint32_t max = artwork.size();

    for( uint32_t i = 0; i < max; i++ ) {
        MP4TagArtwork& a = cartwork[i];
        CoverArtBox::Item& item = artwork[i];

        a.data = item.buffer;
        a.size = item.size;

        switch( item.type ) {
            case BT_BMP:
                a.type = MP4_ART_BMP;
                break;

            case BT_GIF:
                a.type = MP4_ART_GIF;
                break;

            case BT_JPEG:
                a.type = MP4_ART_JPEG;
                break;

            case BT_PNG:
                a.type = MP4_ART_PNG;
                break;

            default:
                a.type = MP4_ART_UNDEFINED;
                break;
        }
    }

    tags->artwork      = cartwork;
    tags->artworkCount = max;
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
