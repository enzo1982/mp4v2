#include "util/impl.h"

using namespace mp4v2::util;

// TODO-TDB: add POSIX UUID support, see http://trac.xapian.org/browser/trunk/xapian-core/common/safeuuid.h
#include <rpc.h>

///////////////////////////////////////////////////////////////////////////////

/* One-letter options -- if you want to rearrange these, change them
   here, immediately below in OPT_STRING, and in the help text. */
#define OPT_HELP         0x01ff
#define OPT_VERSION      0x02ff
#define OPT_LIST         'l'
#define OPT_SET          's'
#define OPT_STRING       'S'
#define OPT_INT          'i'
#define OPT_TIME         't'
#define OPT_GUID         'g'
#define OPT_REMOVE       'r'

static const char* const MP4_XTRA_WMPTAGS[] = {
    "WM/EncodingTime",
    "WM/MediaClassSecondaryID",
    "WM/MediaClassPrimaryID",
    "WM/Provider",
    "WM/WMContentID",
    "WM/WMCollectionID",
    "WM/WMCollectionGroupID",
    "WM/Publisher",
    "WM/UniqueFileIdentifier",
    "WM/ProviderStyle",
    "AverageLevel",
    "PeakValue",
    NULL, // must be last
};

#define OPT_STR  "l:s:S:i:t:g:r:"

#define ELEMENT_OF(x,i) x[int(i)]

static const char* const help_text =
    "OPTION... FILE...\n"
    "Lists, adds or modifies WMP-specific tags on MP4 files (Xtra atom).\n"
    "\n"
    "      -help            Display this help text and exit\n"
    "      -version         Display version information and exit\n"
    "  -l, -list            List all WMP-specific tags in the file (default)\n"
    "  -s, -set         STR  Set the value of the specified WMP tag\n"
    "  -S, -string      STR  String value to set\n"
    "  -i, -int         NUM  Integer value to set\n"
    "  -t, -time        TIME Filetime value to set (YYYY-MM-DD hh:mm:ss)\n"
    "  -g, -guid        GUID GUID value to set (excluding brackets)\n"
    "  -r, -remove      STR  Remove the specified WMP tag\n";

const time_t FileTime_to_POSIX_time( const MP4FILETIME ft )
{
    uint64_t date = ((uint64_t)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
    // 100-nanoseconds = milliseconds * 10000
    uint64_t adjust = 11644473600000 * 10000;
    // removes the diff between 1970 and 1601
    date -= adjust;
    // converts back from 100-nanoseconds to seconds
    return date / 10000000;
}

void POSIX_time_to_FileTime( const time_t time, MP4FILETIME *ft )
{
    // converts from seconds to 100-nanoseconds
    uint64_t date = time * 10000000;
    // 100-nanoseconds = milliseconds * 10000
    uint64_t adjust = 11644473600000 * 10000;
    // adds the diff between 1970 and 1601
    date += adjust;
    ft->dwLowDateTime = (unsigned long)date;
    ft->dwHighDateTime = (unsigned long)(date >> 32);
}

/// The size of a UUID in bytes.
const size_t UUID_SIZE = 16;

/// The size of a UUID string in bytes (not including trailing '\0').
const size_t UUID_STRING_SIZE = 36;

// Unfortunately Windows defines uuid_t as GUID, so we redefine it to match the
// Unix definition.
typedef unsigned char uuid_t_[16];

int uuid_parse( const char * in, uuid_t_ uu )
{
    UUID uuid;
    if( UuidFromStringA( (unsigned char*)in, &uuid ) != RPC_S_OK )
        return -1;
    memcpy( (void *)uu, &uuid, UUID_SIZE );
    return 0;
}

void uuid_unparse_lower( const uuid_t_ uu, char * out )
{
    UUID uuid;
    char *uuidstr;
    memcpy( (void *)&uuid, (const void *)uu, UUID_SIZE );
    if( (UuidToStringA( &uuid, (unsigned char **)(&uuidstr) ) != RPC_S_OK) ) {
        // The only documented (or really conceivable) error code is
        // RPC_S_OUT_OF_MEMORY.
        throw std::bad_alloc();
    }
    memcpy( out, strlwr( uuidstr ), UUID_STRING_SIZE );
    out[UUID_STRING_SIZE] = '\0';
    RpcStringFreeA( (unsigned char**)(&uuidstr) );
}

extern "C" int main( int argc, char** argv )
{
    const prog::Option long_options[] = {
        { "help",        prog::Option::NO_ARG,       0, OPT_HELP         },
        { "version",     prog::Option::NO_ARG,       0, OPT_VERSION      },
        { "list",        prog::Option::NO_ARG,       0, OPT_LIST         },
        { "set",         prog::Option::REQUIRED_ARG, 0, OPT_SET          },
        { "string",      prog::Option::REQUIRED_ARG, 0, OPT_STRING       },
        { "int",         prog::Option::REQUIRED_ARG, 0, OPT_INT          },
        { "time",        prog::Option::REQUIRED_ARG, 0, OPT_TIME         },
        { "guid",        prog::Option::REQUIRED_ARG, 0, OPT_GUID         },
        { "remove",      prog::Option::REQUIRED_ARG, 0, OPT_REMOVE       },
        { NULL, prog::Option::NO_ARG, 0, 0 }
    };

    const char *mods[UCHAR_MAX];

    memset( mods, 0, sizeof( mods ) );

    /* Any modifications requested? */
    int nummods = 0;

    /* Number of tag data types specified */
    int numtypes = 0;

    /* Integer value to set */
    uint64_t intvalue;

    /* Filetime value to set */
    time_t timevalue;

    /* GUID value to set */
    uuid_t_ guidvalue = {0};

    /* Option-processing loop. */
    tm time = {0};
    int c = prog::getOptionSingle( argc, argv, OPT_STR, long_options, NULL );
    while ( c != -1 ) {
        int r = 2;
        switch ( c ) {
                /* getopt() returns '?' if there was an error.  It already
                   printed the error message, so just return. */
            case '?':
                return 1;

                /* Help and version requests handled here. */

            case OPT_HELP:
                fprintf( stderr, "usage %s %s\n", argv[0], help_text );
                return 0;
            case OPT_VERSION:
                fprintf( stderr, "%s - %s\n", argv[0], MP4V2_PROJECT_name_formal );
                return 0;

                /* Integer arguments: convert them using sscanf(). */
            case OPT_INT:
                r = sscanf( prog::optarg, "%llu", &intvalue );
                if ( r < 1 ) {
                    fprintf( stderr, "%s: option requires integer argument -- %c\n",
                             argv[0], c );
                    return 2;
                }

                mods[c] = prog::optarg;
                nummods++;
                numtypes++;
                break;

                /* Time arguments: convert them using sscanf()/mktime(). */
            case OPT_TIME:
                r = sscanf( prog::optarg, "%04d-%02d-%02d %02d:%02d:%02d",
                    &time.tm_year, &time.tm_mon, &time.tm_mday,
                    &time.tm_hour, &time.tm_min, &time.tm_sec );
                if ( r < 1 ) {
                    fprintf( stderr, "%s: option requires a time argument -- %c\n",
                             argv[0], c );
                    return 3;
                }
                else {
                    time.tm_year -= 1900;
                    time.tm_mon -= 1;
                    timevalue = mktime( &time );
                }

                mods[c] = prog::optarg;
                nummods++;
                numtypes++;
                break;

                /* GUID arguments: convert them using uuid_parse(). */
            case OPT_GUID:
                if ( uuid_parse( prog::optarg, guidvalue ) == -1) {
                    fprintf( stderr, "%s: option requires a GUID argument -- %c\n",
                             argv[0], c );
                    return 4;
                }

                mods[c] = prog::optarg;
                nummods++;
                numtypes++;
                break;

            case OPT_STRING:

                mods[c] = prog::optarg;
                nummods++;
                numtypes++;
                break;

            default:
                mods[c] = prog::optarg;
                nummods++;
        } /* end switch */

        c = prog::getOptionSingle( argc, argv, OPT_STR, long_options, NULL );
    } /* end while */

    /* Check that at most one tag set is requested */
    if ( numtypes > 1 ) {
        fprintf( stderr,
                 "%s: Only one tag data type (string/int/time/GUID) is allowed at a time.\n",
                 argv[0] );
        fprintf( stderr, "usage %s %s\n", argv[0], help_text );
        return 5;
    }
    else if ( numtypes == 1 ) {
        /* A value to set was specified; check that we have a -set argument */
        if ( !mods[OPT_SET] ) {
            fprintf( stderr,
                     "%s: You must specify a WMP tag to set.\n",
                     argv[0] );
            fprintf( stderr, "usage %s %s\n", argv[0], help_text );
            return 6;
        }
    }
    
    /* If -set was specified, check that we have a value to set */
    if ( mods[OPT_SET] && numtypes == 0 ) {
        fprintf( stderr,
                 "%s: You must specify a value to set.\n",
                 argv[0] );
        fprintf( stderr, "usage %s %s\n", argv[0], help_text );
        return 7;
    }

    /* Check that we have at least one non-option argument */
    if ( ( argc - prog::optind ) < 1 ) {
        fprintf( stderr,
                 "%s: You must specify at least one MP4 file.\n",
                 argv[0] );
        fprintf( stderr, "usage %s %s\n", argv[0], help_text );
        return 8;
    }

    /* Check that we have at least one requested modification.  List all tags
       if no modifications are requested */
    if ( !nummods ) {
        mods[OPT_LIST] = "";
    }

    /* Loop through the non-option arguments, and modify the tags as
       requested. */
    while ( prog::optind < argc ) {
        char *mp4 = argv[prog::optind++];

        MP4FileHandle h = MP4Modify( mp4 );
        if ( h == MP4_INVALID_FILE_HANDLE ) {
            fprintf( stderr, "Could not open '%s'... aborting\n", mp4 );
            return 9;
        }

        /* List tags */
        const MP4XtraTag *tag;
        char *uuidstr[UUID_STRING_SIZE + 1];
        if ( ELEMENT_OF(mods,OPT_LIST) ) {
            for( const char* const* p = MP4_XTRA_WMPTAGS; *p; p++ ) {
                tag = MP4XtraTagsGet( h, *p );
                if( tag ) {
                    printf( "%s type=%d\n", tag->name, tag->type );

                    switch( tag->type ) {

                        case MP4_XTRA_BT_UNICODE:
                            printf( "    (Unicode) %ls\n", tag->pwstr );
                            break;

                        case MP4_XTRA_BT_INT64:
                            printf( "    (Int64) %ld\n", tag->number );
                            break;

                        case MP4_XTRA_BT_GUID:
                            uuid_unparse_lower( (const unsigned char*)&tag->guid, (char *)uuidstr );
                            printf( "    (GUID) {%s}\n", uuidstr );
                            break;

                        case MP4_XTRA_BT_FILETIME:
                            time_t time = FileTime_to_POSIX_time( tag->fileTime );
                            tm *t = localtime( &time ); 
                            if( t )
                                printf( "    (Filetime) %04d-%02d-%02d %02d:%02d:%02d\n",
                                    t->tm_year+1900,t->tm_mon+1,t->tm_mday,
                                    t->tm_hour,t->tm_min,t->tm_sec );
                            break;

                    }

                    MP4XtraTagFree( tag );
                }
            }
        }

        if ( ELEMENT_OF(mods, OPT_STRING) ) {
            /* No Unicode support needed */
            std::string value( mods[OPT_STRING] );
            std::wstring widevalue( value.begin(), value.end() );
            MP4XtraTagsSetString( h, mods[OPT_SET], widevalue.c_str() );
        }
        else if ( ELEMENT_OF(mods, OPT_INT) )
            MP4XtraTagsSetInt( h, mods[OPT_SET], &intvalue );
        else if ( ELEMENT_OF(mods, OPT_TIME) ) {
            MP4FILETIME ft;
            POSIX_time_to_FileTime( timevalue, &ft );
            MP4XtraTagsSetFileTime( h, mods[OPT_SET], &ft );
        }
        else if ( ELEMENT_OF(mods, OPT_GUID) )
            MP4XtraTagsSetGUID( h, mods[OPT_SET], (const MP4GUID *)&guidvalue );

        if ( ELEMENT_OF(mods, OPT_REMOVE) )
            MP4XtraTagsRemove( h, mods[OPT_REMOVE] );

        MP4Close( h );
    } /* end while optind < argc */
    return 0;
}

