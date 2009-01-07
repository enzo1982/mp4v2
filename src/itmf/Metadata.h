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
    static const string CODE_ALBUMNAME;
    static const string CODE_ARTIST;

public:
    MP4File& file;

    string    albumName;
    string    artist;
    string    userComment;
    string    copyright;
    string    releaseDate;
    string    encodedBy;
    GenreType predefinedGenre;
    string    userGenre;
    string    songName;
    string    trackSubtitle;
    string    encodingTool;
    string    composer;
    string    albumArtist;
    string    grouping;
    uint8_t   contentRating;
    uint32_t  beatsPerMinute;
    string    soundCheck;
    uint32_t  toolInfo;
    string    cddb_id1;
    string    cddb_id2;
    string    cddb_id3;

    // extended tags
    string artDirector;
    string arranger;
    string lyricist;
    string copyrightAcks;
    string conductor;
    string songDescription;
    string director;
    string equalizationPresetName;
    string linerNotes;
    string recordCompany;
    string originalArtist;
    string phonogramRights;
    string producer;
    string performer;
    string publisher;
    string soundEngineer;
    string soloist;
    string credits;
    string thanks;
    string onlineExtras;
    string executiveProducer;

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
