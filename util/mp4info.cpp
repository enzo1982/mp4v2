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
 * Copyright (C) Cisco Systems Inc. 2001-2002.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie     dmackie@cisco.com
 */

#include "libutil/util.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

extern "C" int main( int argc, char** argv )
{
    const char* const usageString =
        "<file-name>";

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
                fprintf( stderr, "usage: %s %s\n", ProgName, usageString );
                exit( 0 );
            case 'V':
                fprintf( stderr, "%s - %s\n", ProgName, MP4V2_PROJECT_name_formal );
                exit( 0 );
            default:
                fprintf( stderr, "%s: unknown option specified, ignoring: %c\n",
                         ProgName, c );
        }
    }

    /* check that we have at least one non-option argument */
    if ( ( argc - prog::optind ) < 1 ) {
        fprintf( stderr, "usage: %s %s\n", ProgName, usageString );
        exit( 1 );
    }

    /* end processing of command line */
    printf( "%s version %s\n", ProgName, MP4V2_PROJECT_version );

    while ( prog::optind < argc ) {
        char *mp4FileName = argv[prog::optind++];

        printf( "%s:\n", mp4FileName );

        char* info = MP4FileInfo( mp4FileName );

        if ( !info ) {
            fprintf( stderr,
                     "%s: can't open %s\n",
                     ProgName, mp4FileName );
            continue;
        }

        fputs( info, stdout );
        MP4FileHandle mp4file = MP4Read( mp4FileName ); //, MP4_DETAILS_ERROR);
        if ( mp4file != MP4_INVALID_FILE_HANDLE ) {
            char *value;
            uint16_t numvalue, numvalue2;
            uint8_t bytevalue;
            if ( MP4GetMetadataName( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Name: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataArtist( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Artist: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataComposer( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Composer: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataTool( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Tool: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataReleaseDate( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Release Date: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataAlbum( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Album: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataTrack( mp4file, &numvalue, &numvalue2 ) ) {
                fprintf( stdout, " Track: %u of %u\n", numvalue,
                         numvalue2 );
            }
            if ( MP4GetMetadataDisk( mp4file, &numvalue, &numvalue2 ) ) {
                fprintf( stdout, " Disk: %u of %u\n", numvalue,
                         numvalue2 );
            }
            if ( MP4GetMetadataGenre( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Genre: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataGrouping( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Grouping: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataBPM( mp4file, &numvalue ) ) {
                fprintf( stdout, " BPM: %u\n", numvalue );
            }
            if ( MP4GetMetadataComment( mp4file, &value ) &&
                    value != NULL ) {
                fprintf( stdout, " Comment: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataCompilation( mp4file, &bytevalue ) ) {
                fprintf( stdout, " Part of Compilation: %s\n",
                         bytevalue ? "yes" : "no" );
            }
            if ( MP4GetMetadataPartOfGaplessAlbum( mp4file, &bytevalue ) ) {
                fprintf( stdout, " Part of Gapless Album: %s\n",
                         bytevalue ? "yes" : "no" );
            }
            uint32_t artcount = MP4GetMetadataCoverArtCount( mp4file );
            if ( artcount > 0 ) {
                fprintf( stdout, " Cover Art pieces: %u\n",
                         artcount );
            }
            if ( MP4GetMetadataAlbumArtist( mp4file, &value ) &&
                    value != NULL ) {
                fprintf( stdout, " Album Artist: %s\n",
                         value );
                free( value );
            }
            MP4Close( mp4file );
        }
        free( info );
    }

    return( 0 );
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
