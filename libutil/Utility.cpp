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

Utility::Utility( string name_, int argc_, char** argv_ )
    : _longOptions      ( NULL )
    , _name             ( name_ )
    , _argc             ( argc_ )
    , _argv             ( argv_ )
    , _optimize         ( false )
    , _compatibility    ( COMPAT_AUTO )
    , _strict           ( false )
    , _dryrun           ( false )
    , _keepgoing        ( false )
    , _overwrite        ( false )
    , _force            ( false )
    , _debug            ( 0 )
    , _verbosity        ( 1 )
    , _jobCompatibility ( COMPAT_NONE )
    , _jobCount         ( 0 )
    , _debugVerbosity   ( 0 )
    , _debugImplicits   ( false )
    , _group            ( "OPTIONS" )
{
    debugUpdate( 1 );

    // populate map
    _compatibilityByExtension["m4a"] = COMPAT_ITUNES;
    _compatibilityByExtension["m4p"] = COMPAT_ITUNES;
    _compatibilityByExtension["m4v"] = COMPAT_ITUNES;

    _xhelpCompatibility =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nCOMPATIBILITY MODES"
        "\n  none      disable compaitbility checks"
        "\n  auto      automatic compatibility mode (default)"
        "\n  itunes    force itunes compatibility";

    _xhelpDebug =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nDEBUG LEVELS (for raw mp4 file I/O)"
        "\n  0  supressed"
        "\n  1  add warnings and errors (default)"
        "\n  2  add table details"
        "\n  3  add implicits"
        "\n  4  everything";

    _xhelpVerbosity =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nVERBOSITY LEVELS"
        "\n  0  warnings and errors"
        "\n  1  normal informative messages (default)"
        "\n  2  more informative messages"
        "\n  3  everything";

    // populate standard options
    _group.add( 'z', false, "optimize",  false, LC_NONE, "optimize mp4 file after modification" );
    _group.add( 'c', true,  "compat",    true,  LC_NONE, "specify compatibility MODE", "MODE",
        _xhelpCompatibility );
    _group.add( 's', false, "strict",    false, LC_NONE, "enable strict compat; treat deprecations as errors" );
    _group.add( 'y', false, "dryrun",    false, LC_NONE, "do not actually create or modify any files" );
    _group.add( 'k', false, "keepgoing", false, LC_NONE, "continue batch processing even after errors" );
    _group.add( 'o', false, "overwrite", false, LC_NONE, "overwrite existing files when creating" );
    _group.add( 'f', false, "force",     false, LC_NONE, "force overwrite even if file is read-only" );
    _group.add( 'q', false, "quiet",     false, LC_NONE, "equivalent to --verbose 0" );
    _group.add( 'd', false, "debug",     true,  LC_DEBUG, "increase debug or long-option to set NUM", "NUM",
        _xhelpDebug );
    _group.add( 'v', false, "verbose",   true,  LC_VERBOSE, "increase verbosity or long-option to set NUM", "NUM",
        _xhelpVerbosity );
    _group.add( 'h', false, "help",      false, LC_HELP, "print help or long-option for extended help" );
    _group.add(             "version",   false, LC_VERSION, "print version information and exit" );

    _usage = "<UNDEFINED>";
    _description = "<UNDEFINED>";
    _groups.push_back( &_group );
}

///////////////////////////////////////////////////////////////////////////////

Utility::~Utility()
{
    delete[] _longOptions;
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::batch( int argi )
{
    // nothing to be done
    if( !(argi < _argc) )
        return SUCCESS;

    bool result = FAILURE;
    for( int i = argi; i < _argc; i++ ) {
        if( job( _argv[i] )) {
            if( !_keepgoing )
                return FAILURE;
        }
        else {
            result = SUCCESS;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::debugUpdate( uint32_t debug )
{
    _debug = debug;
    verbose2f( "debug level: %u\n", _debug );

    switch( _debug ) {
        case 0:
            _debugVerbosity = 0;
            _debugImplicits = false;
            break;

        case 1:
            _debugVerbosity = MP4_DETAILS_ERROR;
            _debugImplicits = false;
            break;

        case 2:
            _debugVerbosity = MP4_DETAILS_ERROR | MP4_DETAILS_TABLE;
            _debugImplicits = false;
            break;

        case 3:
            _debugVerbosity = MP4_DETAILS_ERROR | MP4_DETAILS_TABLE;
            _debugImplicits = true;
            break;

        case 4:
        default:
            _debugVerbosity = MP4_DETAILS_ALL;
            _debugImplicits = true;
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::dryrunAbort()
{
    if( !_dryrun )
        return false;

    verbose2f( "skipping: dry-run mode enabled\n" );
    return true;
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::errf( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    vfprintf( stderr, format, ap );
    va_end( ap );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::formatGroups()
{
    // determine longest long-option [+space +argname]
    int longMax = 0;
    list<Group*>::reverse_iterator end = _groups.rend();
    for( list<Group*>::reverse_iterator it = _groups.rbegin(); it != end; it++ ) {
        Group& group = **it;
        const Group::List::const_iterator endo = group.options.end();
        for( Group::List::const_iterator ito = group.options.begin(); ito != endo; ito++ ) {
            Option& option = **ito;
            int len = option.lname.length();
            if( option.lhasarg )
                len += 1 + option.argname.length();
            if( len > longMax )
                longMax = len;
        }
    }

    // format help output (no line-wrapping yet)
    ostringstream oss;

    int groupCount = 0;
    int optionCount = 0;
    end = _groups.rend();
    for( list<Group*>::reverse_iterator it = _groups.rbegin(); it != end; it++, groupCount++ ) {
        if( groupCount )
            oss << '\n';
        Group& group = **it;
        oss << '\n' << group.name;
        const Group::List::const_iterator endo = group.options.end();
        for( Group::List::const_iterator ito = group.options.begin(); ito != endo; ito++, optionCount++ ) {
            Option& option = **ito;
            oss << "\n ";

            if( option.scode == 0 )
                oss << "    --";
            else
                oss << '-' << option.scode << ", --";

            if( option.lhasarg ) {
                oss << option.lname << ' ' << option.argname;
                oss << setw( longMax - option.lname.length() - 1 - option.argname.length() ) << "";
            }
            else {
                oss << setw( longMax ) << left << option.lname;
            }

            oss << "  ";

            const string::size_type imax = option.descr.length();
            for( string::size_type i = 0; i < imax; i++ )
                oss << option.descr[i];
        }
    }

    _help = oss.str();

    // allocate and populate C-style options
    _longOptions = new prog::Option[optionCount];
    _shortOptions.clear();

    int optionIndex = 0;
    end = _groups.rend();
    for( list<Group*>::reverse_iterator it = _groups.rbegin(); it != end; it++ ) {
        Group& group = **it;
        const Group::List::const_iterator endo = group.options.end();
        for( Group::List::const_iterator ito = group.options.begin(); ito != endo; ito++, optionIndex++ ) {
            Option& a = **ito;
            prog::Option& b = _longOptions[optionIndex];

            b.name = const_cast<char*>(a.lname.c_str());
            b.type = a.lhasarg ? prog::Option::REQUIRED_ARG : prog::Option::NO_ARG;
            b.flag = 0;
            b.val  = (a.lcode == LC_NONE) ? a.scode : a.lcode;

            if( a.scode != 0 ) {
                _shortOptions += a.scode;
                if( a.shasarg )
                    _shortOptions += ':';
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::job( string arg )
{
    verbose2f( "job begin: %s\n", arg.c_str() );

    // setup job compatibility
    if( _compatibility == COMPAT_AUTO ) {
        string ext = arg;
        io::FileSystem::pathnameOnlyExtension( ext );

        const CompatibilityMap::iterator found = _compatibilityByExtension.find( ext );
        if( found != _compatibilityByExtension.end() )
            _jobCompatibility = found->second;
        else
            _jobCompatibility = COMPAT_NONE;

        verbose2f( "compatibility mode: %s\n", toString( _jobCompatibility ).c_str() );
    }
    else {
        _jobCompatibility = _compatibility;
    }

    // perform job
    JobContext job( arg );
    const bool result = utility_job( job );

    // free data flagged with job
    list<void*>::iterator end = job.tofree.end();
    for( list<void*>::iterator it = job.tofree.begin(); it != end; it++ )
        free( *it );

    // close file handle flagged with job
    if( job.fileHandle != MP4_INVALID_FILE_HANDLE )
        MP4Close( job.fileHandle );

    // invoke optimize if flagged
    if( _optimize && job.optimizeApplicable ) {
        verbose1f( "optimizing %s\n", job.file.c_str() );
        if( !MP4Optimize( job.file.c_str(), NULL ))
            hwarnf( "optimize failed: %s\n", job.file.c_str() );    
    }

    verbose2f( "job end\n" );
    _jobCount++;
    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::herrf( const char* format, ... )
{
    fprintf( stdout, (_keepgoing ? "WARNING: " : "ERROR: ") );
    va_list ap;
    va_start( ap, format );
    vfprintf( stdout, format, ap );
    va_end( ap );
    return FAILURE;
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::hwarnf( const char* format, ... )
{
    fprintf( stdout, "WARNING: " );
    va_list ap;
    va_start( ap, format );
    vfprintf( stdout, format, ap );
    va_end( ap );
    return FAILURE;
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::outf( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    vfprintf( stdout, format, ap );
    va_end( ap );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::printHelp( bool extended, bool toerr )
{
    ostringstream oss;
    oss << "Usage: " << _name << " " << _usage << '\n' << _description << '\n' << _help;

    if( extended ) {
        const list<Group*>::reverse_iterator end = _groups.rend();
        for( list<Group*>::reverse_iterator it = _groups.rbegin(); it != end; it++ ) {
            Group& group = **it;
            const Group::List::const_iterator endo = group.options.end();
            for( Group::List::const_iterator ito = group.options.begin(); ito != endo; ito++ ) {
                Option& option = **ito;
                if( option.help.empty() )
                    continue;

                oss << '\n' << option.help;
            }
        }
    }

    if( toerr )
        errf( "%s\n\n", oss.str().c_str() );
    else
        outf( "%s\n\n", oss.str().c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::printUsage( bool toerr )
{
    ostringstream oss;
    oss <<   "Usage: " << _name << " " << _usage
        << "\nTry -h or --help for more information";
 
    if( toerr )
        errf( "%s\n", oss.str().c_str() );
    else
        outf( "%s\n", oss.str().c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::printVersion()
{
    ostringstream oss;
    oss << _name << " - " << MP4V2_PROJECT_name_formal;

    outf( "%s\n", oss.str().c_str() );
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::process()
{
    formatGroups();

    // populate code lookup set
    set<int> codes;
    const Group::List::const_iterator end = _group.options.end();
    for( Group::List::const_iterator it = _group.options.begin(); it != end; it++ ) {
        Option& option = **it;
        if( option.scode != 0 )
            codes.insert( option.scode );
        if( option.lcode != LC_NONE )
            codes.insert( option.lcode );
    }

    for( ;; ) {
        const int code = prog::getOption( _argc, _argv, _shortOptions.c_str(), _longOptions, NULL );
        if( code == -1 )
            break;

        bool handled = false;
        if( utility_option( code, handled ))
            break;
        if( handled )
            continue;

        if( codes.find( code ) == codes.end() )
            continue;

        switch( code ) {
            case 'z':
                _optimize = true;
                break;

            case 'c':
            {
                const string s = prog::optarg;
                if( !s.compare( "none" ))
                    _compatibility = COMPAT_NONE;
                else if( !s.compare( "auto" ))
                    _compatibility = COMPAT_AUTO;
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

            case 'o':
                _overwrite = true;
                break;

            case 'f':
                _force = true;
                break;

            case 'q':
                _verbosity = 0;
                debugUpdate( 0 );
                break;

            case 'v':
                _verbosity++;
                break;

            case 'd':
                debugUpdate( _debug + 1 );
                break;

            case 'h':
                printHelp( false );
                return SUCCESS;

            case LC_VERBOSE:
            {
                const uint32_t level = std::strtoul( prog::optarg, NULL, 0 );
                _verbosity = ( level < 4 ) ? level : 3;
                break;
            }

            case LC_DEBUG:
                debugUpdate( std::strtoul( prog::optarg, NULL, 0 ) );
                break;

            case LC_HELP:
                printHelp( true );
                return SUCCESS;

            case LC_VERSION:
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

    const bool result = batch( prog::optind );
    verbose2f( "exit code %d\n", result );
    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::openFileForWriting( io::StdioFile& file )
{
    static const char* const mode = "wb";

    // simple case is file does not exist
    if( !io::FileSystem::exists( file.name )) {
        if( !file.open( mode ))
            return SUCCESS;

        return herrf( "unable to open %s for write: %s\n", file.name.c_str(), sys::getLastErrorStr() );
    }

    // fail if overwrite is not enabled
    if( !_overwrite )
        return herrf( "file already exists: %s\n", file.name.c_str() );

    // only overwrite if it is a file
    if( !io::FileSystem::isFile( file.name ))
        return herrf( "cannot overwrite non-file: %s\n", file.name.c_str() );

    // first attemp to re-open/truncate so as to keep any file perms
    if( !file.open( mode ))
        return SUCCESS;

    // fail if force is not enabled
    if( !_force )
        return herrf( "unable to overwrite file: %s\n", file.name.c_str() );

    // first attempt to open, truncating file
    if( !file.open( mode ))
        return SUCCESS;

    // nuke file
    if( ::remove( file.name.c_str() ))
        return herrf( "unable to remove %s: %s\n", file.name.c_str(), sys::getLastErrorStr() );

    // final effort
    if( !file.open( mode ))
        return SUCCESS;

    return herrf( "unable to open %s for write: %s\n", file.name.c_str(), sys::getLastErrorStr() );
}

///////////////////////////////////////////////////////////////////////////////

string
Utility::toString( CompatibilityMode mode )
{
    string result;
    return toString( mode, result );
}

///////////////////////////////////////////////////////////////////////////////

string&
Utility::toString( CompatibilityMode mode, string& out )
{
    switch( mode ) {
        case COMPAT_NONE:   out = "none";   return out;
        case COMPAT_AUTO:   out = "auto";   return out;
        case COMPAT_ITUNES: out = "itunes"; return out;

        default:
            break;
    }

    ostringstream oss;
    oss << "UNDEFINED(" << mode << ")";
    out = oss.str();
    return out;
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::verbose( uint32_t level, const char* format, va_list ap )
{
    if( level > _verbosity )
        return;
    vfprintf( stdout, format, ap );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::verbose1f( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    verbose( 1, format, ap );
    va_end( ap );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::verbose2f( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    verbose( 2, format, ap );
    va_end( ap );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::verbose3f( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    verbose( 3, format, ap );
    va_end( ap );
}

///////////////////////////////////////////////////////////////////////////////

const bool Utility::SUCCESS = false;
const bool Utility::FAILURE = true;

///////////////////////////////////////////////////////////////////////////////

Utility::Group::Group( string name_ )
    : name    ( name_ )
    , options ( _options )
{
}

///////////////////////////////////////////////////////////////////////////////

Utility::Group::~Group()
{
    const List::iterator end = _options.end();
    for( List::iterator it = _options.begin(); it != end; it++ )
        delete *it;
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::Group::add(
    char     scode,
    bool     shasarg,
    string   lname,
    bool     lhasarg,
    LongCode lcode,
    string   descr,
    string   argname,
    string   help )
{
    _options.push_back( new Option( scode, shasarg, lname, lhasarg, lcode, descr, argname, help ));
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::Group::add( 
    string   lname,
    bool     lhasarg,
    LongCode lcode,
    string   descr,
    string   argname,
    string   help )
{
    add( 0, false, lname, lhasarg, lcode, descr, argname, help );
}

///////////////////////////////////////////////////////////////////////////////

void
Utility::Group::remove( string name )
{
    const List::iterator end = _options.end();
    for( List::iterator it = _options.begin(); it != end; it++ ) {
        Option& option = **it;
        if( option.lname.compare( name ))
            continue;

        _options.erase( it );
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////

Utility::Option::Option(
    char     scode_,
    bool     shasarg_,
    string   lname_,
    bool     lhasarg_,
    LongCode lcode_,
    string   descr_,
    string   argname_,
    string   help_ )
        : scode   ( scode_ )
        , shasarg ( shasarg_ )
        , lname   ( lname_ )
        , lhasarg ( lhasarg_ )
        , lcode   ( lcode_ )
        , descr   ( descr_ )
        , argname ( argname_ )
        , help    ( help_ )
{
}

///////////////////////////////////////////////////////////////////////////////

Utility::JobContext::JobContext( string file_ )
    : file               ( file_ )
    , fileHandle         ( MP4_INVALID_FILE_HANDLE )
    , optimizeApplicable ( false )
{
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
