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
//      Kona Blend, kona8lend@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MP4V2_IMPL_ITMF_TAGS_H
#define MP4V2_IMPL_ITMF_TAGS_H

namespace mp4v2 { namespace impl { namespace itmf {

///////////////////////////////////////////////////////////////////////////////

class Tags
{
public:
    static const string CODE_NAME;
    static const string CODE_ARTIST;
    static const string CODE_ALBUMARTIST;
    static const string CODE_ALBUM;
    static const string CODE_GROUPING;
    static const string CODE_COMPOSER;
    static const string CODE_COMMENTS;
    static const string CODE_RELEASEDATE;
    static const string CODE_BEATSPERMINUTE;
    static const string CODE_COMPILATION;
        
    static const string CODE_TVSHOW;
    static const string CODE_TVNETWORK;
    static const string CODE_TVEPISODEID;
    static const string CODE_TVSEASON;
    static const string CODE_TVEPISODE;

    static const string CODE_DESCRIPTION;
    static const string CODE_LONGDESCRIPTION;
    static const string CODE_LYRICS;
    
    static const string CODE_SORTNAME;
    static const string CODE_SORTARTIST;
    static const string CODE_SORTALBUMARTIST;
    static const string CODE_SORTALBUM;
    static const string CODE_SORTCOMPOSER;
    static const string CODE_SORTTVSHOW;
       
    static const string CODE_COPYRIGHT;
    static const string CODE_ENCODINGTOOL;   
    static const string CODE_ENCODEDBY;
    static const string CODE_PURCHASEDATE;

    static const string CODE_CATEGORY;
    static const string CODE_KEYWORDS;
    static const string CODE_PODCAST;
    
    static const string CODE_HDVIDEO;
    static const string CODE_MEDIATYPE;
    static const string CODE_CONTENTRATING;
    static const string CODE_GAPLESS;
    
    static const string CODE_ITUNESACCOUNT;
    static const string CODE_ITUNESACCOUNTTYPE;
    static const string CODE_CNID;
    static const string CODE_ITUNESCOUNTRY;

public:
    string    name;
    string    artist;
    string    albumArtist;
    string    album;
    string    grouping;
    string    composer;
    string    comments;
    string    genre;
    string    releaseDate;
    uint16_t  beatsPerMinute;
    uint8_t   compilation;
    
    string    tvShow;
    string    tvNetwork;
    string    tvEpisodeID;
    uint32_t  tvSeason;
    uint32_t  tvEpisode;

    string    description;
    string    longDescription;
    string    lyrics;
    
    string    sortName;
    string    sortArtist;
    string    sortAlbumArtist;
    string    sortAlbum;
    string    sortComposer;
    string    sortTVShow;

    CoverArtBox::ItemList artwork;

    string    copyright;
    string    encodingTool;  
    string    encodedBy;
    string    purchaseDate;
    
    string    category;
    string    keywords;
    uint8_t   podcast;

    uint8_t   hdVideo;
    uint8_t   mediaType;
    uint8_t   contentRating;
    uint8_t   gapless;
    
    string    iTunesAccount;
    uint8_t   iTunesAccountType;
    uint32_t  cnID;
    uint32_t  iTunesCountry;

public:
    Tags();
    ~Tags();

    void c_alloc ( MP4Tags*& );
    void c_fetch ( MP4Tags*&, MP4FileHandle );
    void c_store ( MP4Tags*&, MP4FileHandle );
    void c_free  ( MP4Tags*& );

    void c_addArtwork    ( MP4Tags*&, MP4TagArtwork& );
    void c_setArtwork    ( MP4Tags*&, uint32_t, MP4TagArtwork& );
    void c_removeArtwork ( MP4Tags*&, uint32_t );

    void c_setString  ( const char*, string&, const char*& );
    void c_setInteger ( const uint8_t*, uint8_t&, const uint8_t*& );
    void c_setInteger ( const uint32_t*, uint32_t&, const uint32_t*& );

private:
    void fetchString  ( MP4File&, const string&, string&, const char*& );
    void fetchInteger ( MP4File&, const string&, uint8_t&, const uint8_t*& );
    void fetchInteger ( MP4File&, const string&, uint16_t&, const uint16_t*& );
    void fetchInteger ( MP4File&, const string&, uint32_t&, const uint32_t*& );
    void fetchGenre   ( MP4File&, string&, const char*& );

    bool fetchData ( MP4File&, const string&, uint8_t*&, uint32_t& );

    void storeGenre  ( MP4File&, const string&, const char* );
    void storeString  ( MP4File&, const string&, const string&, const char* );
    void storeInteger ( MP4File&, const string&, uint8_t, const uint8_t* );
    void storeInteger ( MP4File&, const string&, uint32_t, const uint32_t* );

    void updateArtworkShadow( MP4Tags*& );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::itmf

#endif // MP4V2_IMPL_ITMF_TAGS_H
