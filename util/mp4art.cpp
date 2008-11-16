/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is MPEG4IP.
 *
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2004.  All Rights Reserved.
 *
 * Contributor(s):
 *      Bill May wmay@cisco.com (from mp4info.cpp)
 */

#include "impl.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

static uint8_t png_hdr[] = {
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a
};

static uint8_t jpg_hdr[] = {
    0xff, 0xd8, 0xff, 0xe0
};

static const char* check_image_header( uint8_t *header )
{
    if ( memcmp( header, png_hdr, 8 ) == 0 )
        return ".png"; /* PNG */
    else if ( memcmp( header, jpg_hdr, 4 ) == 0 )
        return ".jpg"; /* JPEG */

    fprintf( stderr, "Picture was an unknown type. Picture will lack a file extension.\n" );

    return NULL;
}

void PrintUsage( char *programName )
{
    fprintf( stderr, "Usage: %s <m4a-file-name> [/output/path/to/picture/basename] \n", programName );
    fprintf( stderr, "Note: a picture suffix (i.e: \".jpg\") is not necessary, \n" );
    fprintf( stderr, "it will be determined from the picture header itself. \n" );
}

static void strip_filename ( const char *name, char *buffer )
{
    const char *suffix, *slash;
    if ( name != NULL ) {
        suffix = strrchr( name, '.' );
        slash = strrchr( name, '/' );
        if ( slash == NULL )
            slash = name;
        else
            slash++;
        if ( suffix == NULL )
            suffix = slash + strlen( slash );
        memcpy( buffer, slash, suffix - slash );
        buffer[suffix - slash] = '\0';
    }
    else {
        strcpy( buffer, "out" );
    }
}

extern "C" int main( int argc, char** argv )
{
    /* begin processing command line */
    char* ProgName = argv[0];
    while ( true ) {
        int c = -1;
        int option_index = 0;
        static const prog::Option long_options[] = {
            { "version", prog::Option::NO_ARG, 0, 'V' },
            { NULL, prog::Option::NO_ARG, 0, 0 }
        };

        c = prog::getOptionSingle( argc, argv, "V", long_options, &option_index );

        if ( c == -1 )
            break;

        switch ( c ) {
            case '?':
                PrintUsage( ProgName );
                exit( 0 );
            case 'V':
                fprintf( stderr, "%s - %s\n", ProgName, MP4V2_PROJECT_name_formal );
                exit( 0 );
            default:
                PrintUsage( ProgName );
                fprintf( stderr, "%s: unknown option specified, ignoring: %c\n",
                         ProgName, c );
        }
    }

    /* check that we have at least one non-option argument */
    if ( ( argc - prog::optind ) < 1 ) {
        PrintUsage( ProgName );
        exit( 1 );
    }

    /* end processing of command line */
    printf( "%s version %s\n", ProgName, MP4V2_PROJECT_version );

    while ( prog::optind < argc ) {
        char *mp4FileName = argv[prog::optind];
        MP4FileHandle mp4file = MP4Read( mp4FileName );
        if ( mp4file != MP4_INVALID_FILE_HANDLE ) {
            uint8_t *art;
            uint32_t art_size;

            if ( MP4GetMetadataCoverArt( mp4file, &art, &art_size ) ) {
                //extract the image from the mp4file
                char filename[MP4V2_PATH_MAX];
                //the user-supplied path with no extension (it will get determined from the header)
                const char* ending = check_image_header( art );
                if ( argc != prog::optind + 1 ) {
                    strcpy( filename, argv[prog::optind + 1] );
                    prog::optind++;
                }
                else {
                    strip_filename( mp4FileName, filename );
                }

                if ( ending != NULL )
                    strcat( filename, ending );

                if ( io::FileSystem::exists( filename ) ) {
                    fprintf( stderr, "Error: file %s already exists\n", filename );
                    exit( 0 );
                }
                else {
                    // TODO-KB: test io::StdioFile
                    io::StdioFile out( filename );
                    if( !out.open( "wb" )) {
                        io::StdioFile::Size nout;
                        out.write( art, art_size, nout );
                        out.close();
                        printf( "created file %s\n", filename );
                        free( art );
                        MP4Close( mp4file );
                        return( 0 );
                    }
                    else {
                        fprintf( stderr, "couldn't create file %s\n", filename );
                    }
                }
            }
            else {
                fprintf( stderr, "art not available for %s\n", mp4FileName );
            }
            prog::optind++;
        }
    }

    return( 0 );
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
