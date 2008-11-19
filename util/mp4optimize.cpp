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
//  The Original Code is MPEG4IP.
// 
//  The Initial Developer of the Original Code is Cisco Systems Inc.
//  Portions created by Cisco Systems Inc. are Copyright (C) 2001.
//  All Rights Reserved.
//
//  Contributors:
//      Kona Blend, kona8lend@@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include "impl.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    const char* const progname = "mp4optimize";
    const char* const usage = "[OPTION]... file [newfile]";
    const char* const help =
          "The file structure of an mp4 file is optimized by moving control"
        "\ninformation to the beginning of the file and interleaving track"
        "\nsamples, eliminating excess seeks during playback. Additionally,"
        "\nfree blocks and unreferenced data chunks are eliminated."
        "\n"
        "\nSpecify newfile to create a new file, otherwise the original file"
        "\nwill be overwritten."
        "\n"
        "\n  -v, --verbose=LEVEL  add diagnostic information to output"
        "\n      --help           output this help and exit"
        "\n      --version        output version information and exit";

    enum PseudoLong {
        PL_HELP = 0x00010000,
        PL_VERSION,
    };

    const prog::Option long_options[] = {
        { "verbose", prog::Option::REQUIRED_ARG, 0, 'v' },
        { "help",    prog::Option::NO_ARG,       0, PL_HELP },
        { "version", prog::Option::NO_ARG,       0, PL_VERSION },
        { 0, prog::Option::NO_ARG, 0, 0 }
    };

    uint32_t verbosity = MP4_DETAILS_ERROR;

    for ( ;; ) {
        const int c = prog::getOption( argc, argv, "v", long_options, NULL );
        if( c == -1 )
            break;

        switch ( c ) {
            case 'v':
                if( prog::optarg ) {
                    switch ( std::strtoul( prog::optarg, NULL, 0 )) {
                        case 0:
                            verbosity = 0;
                            break;
                        case 1:
                            verbosity = MP4_DETAILS_ERROR;
                            break;
                        case 2:
                            verbosity = MP4_DETAILS_TABLE;
                            break;
                        case 3:
                            verbosity = MP4_DETAILS_TABLE;
                            break;
                        case 4:
                        default:
                            verbosity = MP4_DETAILS_ALL;
                            break;
                    }
                }
                else {
                    verbosity = MP4_DETAILS_TABLE;
                }
                break;

            case PL_HELP:
                fprintf( stdout, "Usage: %s %s\n\n%s\n", progname, usage, help );
                return 0;

            case PL_VERSION:
                fprintf( stdout, "%s - %s\n", progname, MP4V2_PROJECT_name_formal );
                return 0;

            default:
                fprintf( stderr, "Usage: %s %s\n", progname, usage );
                return 1;
        }
    }

    // expect 1 or 2 remaining arguments
    const int nremain = argc - prog::optind;
    if( nremain < 1 || nremain > 2 ) {
        fprintf( stderr, "Usage: %s %s\n", progname, usage );
        return 1;
    }

    const char* const in_name = argv[prog::optind++];
    const char* const out_name = nremain == 1 ? NULL : argv[prog::optind++];

    const bool result = MP4Optimize( in_name, out_name, verbosity );
    if( !result ) {
        fprintf( stderr, "MP4Optimize failed: %s\n", sys::getLastErrorStr() );
        return 1;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
