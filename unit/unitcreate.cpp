#include "impl.h"

namespace mp4v2 { namespace unit {

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    const char* const progname = "unitcreate";
    const char* const usage = "[OPTION]... outfile";
    const char* const help =
           "Unit test: create MP4 files."
        "\n"
        "\n  -d, --64data     use 64-bit data atoms"
        "\n  -t, --64time     use 64-bit time atoms"
        "\n  -v, --verbose    add diagnostic information to output"
        "\n      --help       output this help and exit"
        "\n      --version    output version information and exit";

    enum PseudoLong {
        PL_HELP = 0x00010000,
        PL_VERSION,
    };

    const prog::Option long_options[] = {
        { "64data",  prog::Option::NO_ARG, 0, 'd' },
        { "64time",  prog::Option::NO_ARG, 0, 't' },
        { "verbose", prog::Option::NO_ARG, 0, 'v' },
        { "help",    prog::Option::NO_ARG, 0, PL_HELP },
        { "version", prog::Option::NO_ARG, 0, PL_VERSION },
        { 0, prog::Option::NO_ARG, 0, 0 }
    };

    uint32_t out_verbosity = MP4_DETAILS_ERROR;
    uint32_t out_flags = 0;

    for ( ;; ) {
        const int c = prog::getOption( argc, argv, "dtv", long_options, NULL );
        if( c == -1 )
            break;

        switch ( c ) {
            case 'd':
                out_flags |= MP4_CREATE_64BIT_DATA;
                break;

            case 't':
                out_flags |= MP4_CREATE_64BIT_TIME;
                break;

            case 'v':
                out_verbosity = MP4_DETAILS_ALL;
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

    const char* const out_name = argv[prog::optind];

    MP4FileHandle out = MP4Create( out_name, out_verbosity, out_flags );
    if( out == MP4_INVALID_FILE_HANDLE ) {
        fprintf( stdout, "MP4Create failed: %s\n", sys::getLastErrorStr() );
        return 1;
    }

    MP4Close( out );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::unit
