#include "impl.h"

namespace mp4v2 { namespace unit {

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    const char* const progname = "unitdump";
    const char* const usage = "[OPTION]... outfile";
    const char* const help =
           "Unit test: dump MP4 files."
        "\n"
        "\n  -v, --verbose    add diagnostic information to output"
        "\n      --help       output this help and exit"
        "\n      --version    output version information and exit";

    enum PseudoLong {
        PL_HELP = 0x00010000,
        PL_VERSION,
    };

    const prog::Option long_options[] = {
        { "verbose", prog::Option::NO_ARG, 0, 'v' },
        { "help",    prog::Option::NO_ARG, 0, PL_HELP },
        { "version", prog::Option::NO_ARG, 0, PL_VERSION },
        { 0, prog::Option::NO_ARG, 0, 0 }
    };

    uint32_t in_verbosity = MP4_DETAILS_ERROR;

    for ( ;; ) {
        const int c = prog::getOption( argc, argv, "v", long_options, NULL );
        if( c == -1 )
            break;

        switch ( c ) {
            case 'v':
                in_verbosity = MP4_DETAILS_ALL;
                break;

            case PL_HELP:
                fprintf( stdout, "Usage: %s %s\n\n%s\n", progname, usage, help );
                return 0;

            case PL_VERSION:
                fprintf( stderr, "%s - %s\n", progname, MP4V2_PROJECT_name_formal );
                return 0;

            default:
                fprintf( stderr, "Usage: %s %s\n", progname, usage );
                return 1;
        }
    }

    // we expect exactly 1 remaining argument
    if( (argc - prog::optind) != 1 ) {
        fprintf( stdout, "Usage: %s %s\n\n%s\n", progname, usage, help );
        return 1;
    }

    const char* const in_name = argv[prog::optind];

    MP4FileHandle in = MP4Read( in_name, in_verbosity );
    if( in == MP4_INVALID_FILE_HANDLE ) {
        fprintf( stdout, "MP4Read failed: %s\n", sys::getLastErrorStr() );
        return 1;
    }

    MP4Close( in );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::unit
