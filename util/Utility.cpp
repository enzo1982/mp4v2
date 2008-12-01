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
    , _options          ( "OPTIONS" )
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
    _options.add( 'z', false, "optimize",  false, LC_NONE, "optimize mp4 file after modification" );
    _options.add( 'c', true,  "compat",    true,  LC_NONE, "specify compatibility MODE", "MODE",
        _xhelpCompatibility );
    _options.add( 's', false, "strict",    false, LC_NONE, "enable strict compat; treat deprecations as errors" );
    _options.add( 'y', false, "dryrun",    false, LC_NONE, "do not actually create or modify any files" );
    _options.add( 'k', false, "keepgoing", false, LC_NONE, "continue batch processing even after errors" );
    _options.add( 'o', false, "overwrite", false, LC_NONE, "overwrite existing files when creating" );
    _options.add( 'f', false, "force",     false, LC_NONE, "force overwrite even if file is read-only" );
    _options.add( 'q', false, "quiet",     false, LC_NONE, "equivalent to --verbose 0" );
    _options.add( 'd', false, "debug",     true,  LC_DEBUG, "increase debug or long-option to set NUM", "NUM",
        _xhelpDebug );
    _options.add( 'v', false, "verbose",   true,  LC_VERBOSE, "increase verbosity or long-option to set NUM", "NUM",
        _xhelpVerbosity );
    _options.add( 'h', false, "help",      false, LC_HELP, "print help or long-option for extended help" );
    _options.add(             "version",   false, LC_VERSION, "print version information and exit" );

    _usage = "<UNDEFINED>";
    _description = "<UNDEFINED>";
    _groups.push_back( &_options );
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
    const Group::List::const_iterator end = _options.options.end();
    for( Group::List::const_iterator it = _options.options.begin(); it != end; it++ ) {
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

uint32_t
crc32( const unsigned char* data, uint32_t size )
{
    /*
     * Copyright (c) 1988, 1992, 1993
     *  The Regents of the University of California.  All rights reserved.
     *
     * Redistribution and use in source and binary forms, with or without
     * modification, are permitted provided that the following conditions
     * are met:
     * 1. Redistributions of source code must retain the above copyright
     *    notice, this list of conditions and the following disclaimer.
     * 2. Redistributions in binary form must reproduce the above copyright
     *    notice, this list of conditions and the following disclaimer in the
     *    documentation and/or other materials provided with the distribution.
     * 3. Neither the name of the University nor the names of its contributors
     *    may be used to endorse or promote products derived from this software
     *    without specific prior written permission.
     *
     * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
     * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
     * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
     * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
     * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
     * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
     * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
     * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
     * SUCH DAMAGE.
     */
    static const uint32_t __crctab[256] = {
        0x0,
        0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
        0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6,
        0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
        0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac,
        0x5bd4b01b, 0x569796c2, 0x52568b75, 0x6a1936c8, 0x6ed82b7f,
        0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a,
        0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
        0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58,
        0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033,
        0xa4ad16ea, 0xa06c0b5d, 0xd4326d90, 0xd0f37027, 0xddb056fe,
        0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
        0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4,
        0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
        0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5,
        0x2ac12072, 0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
        0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca, 0x7897ab07,
        0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c,
        0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1,
        0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
        0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b,
        0xbb60adfc, 0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698,
        0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d,
        0x94ea7b2a, 0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
        0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2, 0xc6bcf05f,
        0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
        0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80,
        0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
        0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a,
        0x58c1663d, 0x558240e4, 0x51435d53, 0x251d3b9e, 0x21dc2629,
        0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c,
        0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
        0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e,
        0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65,
        0xeba91bbc, 0xef68060b, 0xd727bbb6, 0xd3e6a601, 0xdea580d8,
        0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
        0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2,
        0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
        0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74,
        0x857130c3, 0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
        0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c, 0x7b827d21,
        0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a,
        0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e, 0x18197087,
        0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
        0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d,
        0x2056cd3a, 0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce,
        0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb,
        0xdbee767c, 0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
        0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4, 0x89b8fd09,
        0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
        0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf,
        0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4,
    };

#define COMPUTE(var,ch) (var) = (var) << 8 ^ __crctab[(var) >> 24 ^ (ch)]

    uint32_t crc = 0;
    const unsigned char* const max = data + size;

    for (const unsigned char* p = data; p < max; p++)
        COMPUTE( crc, *p );

    for( ; size != 0; size >>= 8 )
        COMPUTE( crc, size & 0xff );

    return ~crc;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
