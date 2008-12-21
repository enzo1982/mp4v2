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
            uint32_t longvalue;
            uint16_t numvalue, numvalue2;
            uint8_t bytevalue;
            if ( MP4GetMetadataName( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Name: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataSortName( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Sort Name: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataArtist( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Artist: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataSortArtist( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Sort Artist: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataComposer( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Composer: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataSortComposer( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Sort Composer: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataTool( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Encoded with: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataEncodedBy( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Encoded by: %s\n", value );
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
            if ( MP4GetMetadataSortAlbum( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Sort Album: %s\n", value );
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
            if ( MP4GetMetadataSortAlbumArtist( mp4file, &value ) &&
                    value != NULL ) {
                fprintf( stdout, " Sort Album Artist: %s\n",
                         value );
                free( value );
            }
            if ( MP4GetMetadataCopyright( mp4file, &value ) &&
                    value != NULL ) {
                fprintf( stdout, " Copyright: %s\n",
                         value );
                free( value );
            }
            if ( MP4GetMetadataExplicit( mp4file, &bytevalue ) ) {
                fprintf( stdout, " Explicit Content: %s\n",
                         bytevalue ? "yes" : "no" );
            }
            if ( MP4GetMetadataPurchaserAccount( mp4file, &value ) &&
                    value != NULL ) {
                fprintf( stdout, " Purchaser Account: %s\n",
                         value );
                free( value );
            }
            if ( MP4GetMetadataPurchaseDate( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Purchase Date: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataHDVideo( mp4file, &bytevalue ) ) {
                fprintf( stdout, " HD Video: %s\n",
                         bytevalue ? "yes" : "no" );
            }
            if ( MP4GetMetadataTVShow( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " TV Show: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataSortTVShow( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Sort TV Show: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataTVNetworkName( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " TV Network: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataTVEpisodeNumber( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " TV Episode Number: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataShortDescription( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Short Description: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataLongDescription( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Long Description: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataLyrics( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Lyrics: \n%s\n", value );
                free( value );
            }
            if ( MP4GetMetadataTVEpisode( mp4file, &longvalue ) ) {
                fprintf( stdout, " TV Episode: %i\n",
                         longvalue );
            }
            if ( MP4GetMetadataTVSeason( mp4file, &longvalue ) ) {
                fprintf( stdout, " TV Season: %i\n",
                         longvalue );
            }
            if ( MP4GetMetadataPodcast( mp4file, &bytevalue ) ) {
                fprintf( stdout, " Podcast: %s\n",
                         bytevalue ? "yes" : "no" );
            }
            if ( MP4GetMetadataKeywords( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Kywords: %s\n", value );
                free( value );
            }
            if ( MP4GetMetadataCategory( mp4file, &value ) && value != NULL ) {
                fprintf( stdout, " Category: %s\n", value );
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
