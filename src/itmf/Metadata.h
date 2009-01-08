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

#ifndef MP4V2_IMPL_ITMF_METADATA_H
#define MP4V2_IMPL_ITMF_METADATA_H

namespace mp4v2 { namespace impl { namespace itmf {

///////////////////////////////////////////////////////////////////////////////

class Metadata
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
        
    static const string CODE_TVSHOW;
    static const string CODE_TVNETWORK;
    static const string CODE_TVEPISODEID;

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
public:
    MP4File& file;


    string    name;
    string    artist;
    string    albumArtist;
    string    album;
    string    grouping;
    string    composer;
    string    comments;
    string    releaseDate;
    
    string    tvShow;
    string    tvNetwork;
    string    tvEpisodeID;

    string    description;
    string    longDescription;
    string    lyrics;
    
    string    sortName;
    string    sortArtist;
    string    sortAlbumArtist;
    string    sortAlbum;
    string    sortComposer;
    string    sortTVShow;

    string    copyright;
    string    encodingTool;  
    string    encodedBy;
    string    purchaseDate;
    
    string    category;
    string    keywords;


    GenreType predefinedGenre;
    string    userGenre;
    string    trackSubtitle;
    uint8_t   contentRating;
    uint32_t  beatsPerMinute;
public:
    Metadata( MP4File& );
    Metadata( MP4FileHandle );
    ~Metadata();

    void c_alloc ( MP4Metadata*& );
    void c_fetch ( MP4Metadata*& );
    void c_store ( MP4Metadata*& );
    void c_free  ( MP4Metadata*& );

    void setString  ( const char*, string&, const char*& );
    void setInteger ( const uint8_t*, uint8_t&, const uint8_t*& );
    void setInteger ( const uint32_t*, uint32_t&, const uint32_t*& );

private:
    void fetchString  ( const string&, string&, const char*& );
    void fetchInteger ( const string&, uint8_t&, const uint8_t*& );
    void fetchInteger ( const string&, uint32_t&, const uint32_t*& );

    bool fetchData( const string&, uint8_t*&, uint32_t& );

    void storeString  ( const string&, const string&, const char* );
    void storeInteger ( const string&, uint8_t, const uint8_t* );
    void storeInteger ( const string&, uint32_t, const uint32_t* );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::itmf

#endif // MP4V2_IMPL_ITMF_METADATA_H
