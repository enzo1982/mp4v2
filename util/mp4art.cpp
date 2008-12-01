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

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

class ArtUtility : public Utility
{
public:
    ArtUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );

private:
    enum Action {
        ACTION_UNDEFINED,
        ACTION_LIST,
        ACTION_ADD,
        ACTION_REMOVE,
        ACTION_EXTRACT,
    };

private:
    struct ArtType {
        string         name;
        string         ext;
        vector<string> cwarns; // compatibility warnings
        string         cerror; // compatibility error
    };

    void identifyArtType( uint8_t*, uint32_t, ArtType& );

    bool actionList    ( JobContext& );
    bool actionAdd     ( JobContext& );
    bool actionRemove  ( JobContext& );
    bool actionExtract ( JobContext& );

    Group  _artOptions;
    Action _action;
    string _artSource;
};

///////////////////////////////////////////////////////////////////////////////

ArtUtility::ArtUtility( int argc, char** argv )
    : Utility     ( "mp4art", argc, argv )
    , _artOptions ( "ACTIONS" )
    , _action     ( ACTION_UNDEFINED )
{
    _artOptions.add( 'l', false, "list",    false, LC_NONE, "list cover-art in mp4" );
    _artOptions.add( 'a', true,  "add",     true,  LC_NONE, "add cover-art from FILE to mp4", "FILE" );
    _artOptions.add( 'r', false, "remove",  false, LC_NONE, "remove cover-art from mp4" );
    _artOptions.add( 'x', false, "extract", false, LC_NONE, "extract cover-art from mp4" );
    _groups.push_back( &_artOptions );

    _usage = "[OPTION]... ACTION file...";
    _description =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nFor each mp4 (m4a) file specified, perform the specified ACTION. An action"
        "\nmust be specified. Some options are not applicable for some actions.";
}

///////////////////////////////////////////////////////////////////////////////

bool
ArtUtility::actionAdd( JobContext& job )
{
    io::StdioFile in( _artSource );
    if( in.open( "rb" ))
        return herrf( "unable to open %s for read: %s\n", _artSource.c_str(), sys::getLastErrorStr() );

    io::File::Size size;
    if( in.getSize( size ))
        return herrf( "unable to get %s size: %s\n", _artSource.c_str(), sys::getLastErrorStr() );

    void* const data = malloc( size );
    job.tofree.push_back( data );

    io::File::Size nin;
    if( in.read( data, size, nin ))
        return herrf( "read failed: %s\n", _artSource.c_str() );

    in.close();

    verbose1f( "adding %s -> %s\n", _artSource.c_str(), job.file.c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4Modify( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    if( !MP4SetMetadataCoverArt( job.fileHandle, (uint8_t*)data, size ))
        return herrf( "unable to add cover-art: %s\n", job.file.c_str() );

    job.optimizeApplicable = true;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ArtUtility::actionExtract( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    const uint32_t artc = MP4GetMetadataCoverArtCount( job.fileHandle );
    verbose2f( "cover-art %s\n", (artc == 0) ? "not found" : "found" );
    if( artc == 0 ) {
        verbose1f( "skipped: %s\n", job.file.c_str() );
        return SUCCESS;
    }

    uint8_t* data;
    uint32_t size;
    if( !MP4GetMetadataCoverArt( job.fileHandle, &data, &size ))
        return herrf( "cover-art missing\n" );

    ArtType type;
    identifyArtType( data, size, type );
    verbose2f( "cover-art identified: %d bytes\n", size );

    const vector<string>::iterator end = type.cwarns.end();
    for (vector<string>::iterator it = type.cwarns.begin(); it != end; it++ )
        hwarnf( "%s\n", it->c_str() );

    if( !type.cerror.empty() )
        return herrf( "%s\n", type.cerror.c_str() );

    string out_name = job.file;
    io::FileSystem::pathnameStripExtension( out_name );

    // add art extension
    out_name += '.';
    out_name += type.ext;
    io::StdioFile out( out_name );

    verbose1f( "extracting %s -> %s\n", job.file.c_str(), out_name.c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    if( openFileForWriting( out ))
        return FAILURE;

    io::File::Size nout;
    if( out.write( data, size, nout ))
        return herrf( "write failed: %s\n", out_name.c_str() );

    out.close();
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ArtUtility::actionList( JobContext& job )
{
    ostringstream report;

    const int wid = 3;
    const int wsize = 8;
    const int wext = 4;
    const int wtype = 9;
    const string sep = "  ";

    if( _jobCount == 0 ) {
        report << setw(wid) << right << "ID" << left
               << sep << setw(wsize) << right << "SIZE" << left
               << sep << setw(8) << "CRC32"
               << sep << setw(wext) << "EXT"
               << sep << setw(wtype) << "TYPE"
               << sep << setw(0) << "FILE"
               << '\n';

        report << setfill('-') << setw(70) << "" << setfill(' ') << '\n';
    }

    job.fileHandle = MP4Read( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    const uint32_t artc = MP4GetMetadataCoverArtCount( job.fileHandle );
    for( uint32_t i = 0; i < artc; i++ ) {
        report << right << setw(wid) << i << left;

        uint8_t* data;
        uint32_t size;
        if( !MP4GetMetadataCoverArt( job.fileHandle, &data, &size )) {
            report << " [missing]\n";
            continue;
        }

        const uint32_t crc = crc32( data, size );

        ArtType type;
        identifyArtType( data, size, type );

        report << sep << setw(wsize) << right << size << left
               << sep << setw(8) << setfill('0') << hex << right << crc << setfill(' ') << dec << left
               << sep << setw(wext) << type.ext
               << sep << setw(wtype) << type.name
               << sep << setw(0) << job.file
               << '\n';
    }

    verbose1f( "%s", report.str().c_str() );
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ArtUtility::actionRemove( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    const uint32_t artc = MP4GetMetadataCoverArtCount( job.fileHandle );
    verbose2f( "cover-art %s\n", (artc == 0) ? "not found" : "found" );
    if( artc == 0 ) {
        verbose1f( "skipped: %s\n", job.file.c_str() );
        return SUCCESS;
    }

    verbose1f( "removing cover-art from %s\n", job.file.c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    if( !MP4DeleteMetadataCoverArt( job.fileHandle ))
        return herrf( "unable to remove cover-art: %s\n", job.file.c_str() );

    job.optimizeApplicable = true;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

void
ArtUtility::identifyArtType( uint8_t* art, uint32_t size, ArtType& type )
{
    // compatibility flags
    // reference: iTMF (iTunes Metadata Format) Specification 2008-04-16
    static const uint32_t CF_ITMF  = 0x00000001; // iTMF compatible
    static const uint32_t CF_ITMFD = 0x00000002; // iTMF deprecated

    struct Header {
        enum Type {
            UNKNOWN,
            BMP,
            GIF87A,
            GIF89A,
            JPG,
            PNG,
            TIFFL,
            TIFFB,
        } type;

        string   ext;    // suitable lower-case file extension
        string   name;   // short string describing name of type
        string   data;   // header-bytes to match
        uint32_t cflags; // compatibility flags
    };

    // types which may be detected by first-bytes only
    static Header headers[] = {
        { Header::BMP,    "BMP",  "bmp", "\x4d\x42", CF_ITMF },
        { Header::GIF87A, "gif",  "GIF (87a)", "GIF87a", CF_ITMFD },
        { Header::GIF89A, "gif",  "GIF (89a)", "GIF89a", CF_ITMFD },
        { Header::JPG,    "jpg",  "JPEG", "\xff\xd8\xff\xe0", CF_ITMF },
        { Header::PNG,    "png",  "PNG", "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a", CF_ITMF },
        { Header::TIFFL,  "tiff", "TIFF (little-endian)", "II42", 0 },
        { Header::TIFFB,  "tiff", "TIFF (big-endian)", "MM42", 0 },
        { Header::UNKNOWN } // must be last
    };

    type.name = "unknown";
    type.ext  = "dat";
    type.cwarns.clear();
    type.cerror.clear();

    Header* found = NULL;
    for( Header* p = headers; p->type != Header::UNKNOWN; p++ ) {
        Header& h = *p;

        if( size < h.data.size() )
            continue;

        if( memcmp(h.data.data(), art, h.data.size()) == 0 ) {
            found = &h;
            type.name = h.name;
            type.ext  = h.ext;
            break;
        }
    }

    // populate compatibility warnings
    if( !found) {
        ostringstream oss;
        oss << "invalid cover-art type: " << type.name;
        type.cerror = oss.str();
        return;
    }

    if( _jobCompatibility == COMPAT_ITUNES ) {
        if( found->cflags & CF_ITMFD ) {
            ostringstream oss;
            oss << "deprecated cover-art type: " << type.name;
            if( _strict ) {
                type.cerror = oss.str();
                return;
            }
            type.cwarns.push_back( oss.str() );
        }
        else if( !(found->cflags & CF_ITMF) ) {
            ostringstream oss;
            oss << "invalid cover-art type: " << type.name;
            type.cerror = oss.str();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

bool
ArtUtility::utility_job( JobContext& job )
{
    bool result = FAILURE;

    switch( _action ) {
        case ACTION_UNDEFINED:
            return herrf( "no action specified\n" );

        case ACTION_LIST:
            result = actionList( job );
            break;

        case ACTION_ADD:
            result = actionAdd( job );
            break;

        case ACTION_REMOVE:
            result = actionRemove( job );
            break;

        case ACTION_EXTRACT:
            result = actionExtract( job );
            break;

        default:
            return herrf( "unknown action(%d)\n", _action );
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool
ArtUtility::utility_option( int code, bool& handled )
{
    handled = true;
    switch( code ) {
        case 'l':
            _action = ACTION_LIST;
            break;

        case 'a':
            _action = ACTION_ADD;
            _artSource = prog::optarg;
            if( _artSource.empty() )
                return herrf( "invalid cover-art file: empty-string\n" );
            break;

        case 'r':
            _action = ACTION_REMOVE;
            break;

        case 'x':
            _action = ACTION_EXTRACT;
            break;

        default:
            handled = false;
            break;
    }

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    ArtUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
