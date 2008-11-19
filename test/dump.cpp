#include "impl.h"

namespace mp4v2 { namespace test {

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    const char* const progname = "test_dump";
    const char* const usage = "[OPTION]... infile";
    const char* const help =
           "Test: dump MP4 file contents as ASCII."
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

    uint32_t in_verbosity = MP4_DETAILS_ERROR;
    bool     in_implicits = false;

    for ( ;; ) {
        const int c = prog::getOption( argc, argv, "v", long_options, NULL );
        if( c == -1 )
            break;

        switch ( c ) {
            case 'v':
                if( prog::optarg ) {
                    switch ( std::strtoul( prog::optarg, NULL, 0 )) {
                        case 0:
                            in_verbosity = 0;
                            in_implicits = false;
                            break;
                        case 1:
                            in_verbosity = MP4_DETAILS_ERROR;
                            in_implicits = false;
                            break;
                        case 2:
                            in_verbosity = MP4_DETAILS_TABLE;
                            in_implicits = false;
                            break;
                        case 3:
                            in_verbosity = MP4_DETAILS_TABLE;
                            in_implicits = true;
                            break;
                        case 4:
                        default:
                            in_verbosity = MP4_DETAILS_ALL;
                            in_implicits = true;
                            break;
                    }
                }
                else {
                    in_verbosity = MP4_DETAILS_TABLE;
                    in_implicits = false;
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

    // expect exactly 1 remaining argument
    if( (argc - prog::optind) != 1 ) {
        fprintf( stderr, "Usage: %s %s\n", progname, usage );
        return 1;
    }

    const char* const in_name = argv[prog::optind];

    MP4FileHandle in = MP4Read( in_name, in_verbosity );
    if( in == MP4_INVALID_FILE_HANDLE ) {
        fprintf( stdout, "MP4Read failed: %s\n", sys::getLastErrorStr() );
        return 1;
    }

    MP4Dump( in, stdout, in_implicits );

    MP4Close( in );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::test
