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
    bool utility_process();
    bool utility_job( JobContext& );

private:
    enum PseudoLong {
        PL_UNDEFINED = 0xf000000, // serves as safe starting value
        PL_VERBOSE,
        PL_HELP,
        PL_VERSION,
    };

    enum Action {
        ACTION_UNDEFINED,
        ACTION_ADD,
        ACTION_REMOVE,
        ACTION_LIST,
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

    bool actionAdd     ( JobContext& );
    bool actionRemove  ( JobContext& );
    bool actionExtract ( JobContext& );

    Action _action;
    string _artSource;
};

///////////////////////////////////////////////////////////////////////////////

ArtUtility::ArtUtility( int argc, char** argv )
    : Utility   ( "mp4art", argc, argv )
    , _action   ( ACTION_UNDEFINED )
{
    _usage = "[OPTION]... ACTION file...";
    _help =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\n"
        "\nFor each mp4 (m4a) file specified, perform the specified ACTION. An action"
        "\nmust be specified. Some options are not applicable for some actions."
        "\n"
        "\nACTIONS"
        "\n  -a, --add FILE     add cover-art from FILE to mp4 file"
        "\n  -r, --remove       remove cover-art from mp4 file"
        "\n  -x, --extract      extract cover-art from mp4 file"
        "\n"
        "\nOPTIONS"
        "\n  -c, --compat MODE  specify compatibility mode"
        "\n  -s, --strict       enable strict compat; treat deprecations as errors"
//      "\n  -y, --dryrun       do not actually create or modify any files"
        "\n  -k, --keepgoing    continue batch processing even after errors"
        "\n  -f, --force        force overwriting even if file is read-only"
        "\n  -o, --overwrite    overwrite existing image files"
        "\n  -q, --quiet        equivalent to --verbose 0"
        "\n  -z, --optimize     optimize mp4 file after modification"
        "\n  -v, --verbose NUM  increase verbosity or long-option to set level NUM"
        "\n  -h, --help         print help or long-option for extended help"
        "\n      --version      output version information and exit";

    const prog::Option options[] = {
        { "add",       prog::Option::REQUIRED_ARG, 0, 'a' },
        { "remove",    prog::Option::NO_ARG,       0, 'r' },
        { "extract",   prog::Option::NO_ARG,       0, 'x' },

        { "compat",    prog::Option::REQUIRED_ARG, 0, 'c' },
        { "strict",    prog::Option::NO_ARG      , 0, 's' },
        { "dryrun",    prog::Option::NO_ARG,       0, 'y' },
        { "keepgoing", prog::Option::NO_ARG,       0, 'k' },
        { "force",     prog::Option::NO_ARG,       0, 'f' },
        { "overwrite", prog::Option::NO_ARG,       0, 'o' },
        { "quiet",     prog::Option::NO_ARG,       0, 'q' },
        { "optimize",  prog::Option::NO_ARG,       0, 'z' },
        { "verbose",   prog::Option::REQUIRED_ARG, 0, PL_VERBOSE },

        { "help",      prog::Option::NO_ARG,       0, PL_HELP },
        { "version",   prog::Option::NO_ARG,       0, PL_VERSION },

        { 0, prog::Option::NO_ARG, 0, 0 }
    };
    _options = options;

    _helpCompatibility = true;
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

    job.fileHandle = MP4Modify( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    verbose1f( "adding cover-art from %s to %s\n", _artSource.c_str(), job.file.c_str() );

    if( !MP4SetMetadataCoverArt( job.fileHandle, (uint8_t*)data, size ))
        return herrf( "unable to add cover-art: %s\n", job.file.c_str() );

    // mark post processing for optimization
    job.optimize = true;
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
    if( artc == 0 ) {
        verbose2f( "cover-art not found\n" );
        return SUCCESS;
    }

    uint8_t* data;
    uint32_t size;
    if( !MP4GetMetadataCoverArt( job.fileHandle, &data, &size ))
        return herrf( "cover-art missing\n" );

    ArtType type;
    identifyArtType( data, size, type );
    verbose2f( "found %s cover-art: %d bytes\n", type.name.c_str(), size );

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

    verbose1f( "extracting cover-art to %s\n", out_name.c_str() );

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
ArtUtility::actionRemove( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    if( MP4GetMetadataCoverArtCount( job.fileHandle ) == 0)
        return SUCCESS;

    verbose1f( "removing cover-art in %s\n", job.file.c_str() );

    if( !MP4DeleteMetadataCoverArt( job.fileHandle ))
        return herrf( "unable to remove cover-art: %s\n", job.file.c_str() );

    // mark post processing for optimization
    job.optimize = true;
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
    type.cwarns.resize( 0 );
    type.cerror.resize( 0 );

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

    if( _compatibilityJob == COMPAT_ITUNES ) {
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
ArtUtility::utility_process()
{
    for ( ;; ) {
        const int c = prog::getOption( _argc, _argv, "a:rxc:sykfoqzvh", _options, NULL );
        if( c == -1 )
            break;

        switch ( c ) {
            case 'a':
                _action = ACTION_ADD;
                _artSource = prog::optarg;
                break;

            case 'r':
                _action = ACTION_REMOVE;
                break;

            case 'x':
                _action = ACTION_EXTRACT;
                break;

            case 'c':
            {
                const string s = prog::optarg;
                if( !s.compare( "none" ))
                    _compatibility = COMPAT_NONE;
                else if( !s.compare( "dynamic" ))
                    _compatibility = COMPAT_DYNAMIC;
                else if( !s.compare( "itunes" ))
                    _compatibility = COMPAT_ITUNES;
                else
                    return herrf( "invalid compatibility mode: %s\n", s.c_str() );
                verbose2f( "compatibility mode: %s\n", toString( _compatibility ).c_str() ); 
                break;
            }

            case 's':
                _strict = true;
                break;

            case 'y':
                _dryrun = true;
                break;

            case 'k':
                _keepgoing = true;
                break;

            case 'f':
                _force = true;
                break;

            case 'o':
                _overwrite = true;
                break;

            case 'q':
                _verbosity = 0;
                break;

            case 'z':
                _optimize = true;
                break;

            case 'v':
                _verbosity++;
                break;

            case 'h':
                printHelp( false );
                return SUCCESS;

            case PL_VERBOSE:
            {
                const uint32_t level = std::strtoul( prog::optarg, NULL, 0 );
                _verbosity = ( level < 4 ) ? level : 3;
                break;
            }

            case PL_HELP:
                printHelp( true );
                return SUCCESS;

            case PL_VERSION:
                printVersion();
                return SUCCESS;

            default:
                printUsage( true );
                return FAILURE;
        }
    }

    if( !(prog::optind < _argc) ) {
        printUsage( true );
        return FAILURE;
    }

    switch( _action ) {
        case ACTION_ADD:
            if( _artSource.empty() )
                return herrf( "invalid cover-art file: empty-string\n" );
            break;

        case ACTION_REMOVE:
        case ACTION_LIST:
        case ACTION_EXTRACT:
            break;

        case ACTION_UNDEFINED:
        default:
            return herrf( "no action specified\n" );
    }

    return batch( prog::optind );
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
