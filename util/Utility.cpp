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
    : _name              ( name_ )
    , _argc              ( argc_ )
    , _argv              ( argv_ )
    , _usage             ( "<UNDEFINED>" )
    , _help              ( "<UNDEFINED>" )
    , _helpCompatibility ( false )
    , _options           ( NULL )
    , _dryrun            ( false )
    , _force             ( false )
    , _keepgoing         ( false )
    , _overwrite         ( false )
    , _verbosity         ( 1 )
    , _optimize          ( false )
    , _compatibility     ( COMPAT_DYNAMIC )
    , _compatibilityJob  ( COMPAT_NONE )
    , _strict            ( false )
{
    // populate map
    _compatibilityByExtension["m4a"] = COMPAT_ITUNES;
    _compatibilityByExtension["m4p"] = COMPAT_ITUNES;
    _compatibilityByExtension["m4v"] = COMPAT_ITUNES;
}

///////////////////////////////////////////////////////////////////////////////

Utility::~Utility()
{
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
Utility::errf( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    vfprintf( stderr, format, ap );
    va_end( ap );
}

///////////////////////////////////////////////////////////////////////////////

bool
Utility::job( string arg )
{
    verbose2f( "starting job: %s\n", arg.c_str() );

    // setup job compatibility
    if( _compatibility == COMPAT_DYNAMIC ) {
        string ext = arg;
        io::FileSystem::pathnameOnlyExtension( ext );

        const CompatibilityMap::iterator found = _compatibilityByExtension.find( ext );
        if( found != _compatibilityByExtension.end() )
            _compatibilityJob = found->second;
        else
            _compatibilityJob = COMPAT_NONE;

        verbose2f( "compatibility mode: %s\n", toString( _compatibilityJob ).c_str() );
    }
    else {
        _compatibilityJob = _compatibility;
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
    if( _optimize && job.optimize ) {
        verbose1f( "optimizing %s\n", job.file.c_str() );
        if( !MP4Optimize( job.file.c_str(), NULL ))
            hwarnf( "optimize failed: %s\n", job.file.c_str() );    
    }

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
    oss << "Usage: " << _name << " " << _usage << _help;

    if( extended ) {
        utility_printHelp( oss );

        if( _helpCompatibility ) {
            oss <<
                // 79-cols, inclusive, max desired width
                // |----------------------------------------------------------------------------|
                "\n"
                "\nCOMPATIBILITY MODES"
                "\n  none     disable compaitbility checks"
                "\n  dynamic  determine compatibility based on filename extension (default)"
                "\n  itunes   force itunes compatibility";
        }

        oss <<
            // 79-cols, inclusive, max desired width
            // |----------------------------------------------------------------------------|
            "\n"
            "\nVERBOSITY LEVELS"
            "\n  0  warnings and errors"
            "\n  1  normal informative messages"
            "\n  2  more informative messages"
            "\n  3  everything";
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
    const bool result = utility_process();
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
        case COMPAT_NONE:    out = "none";    return out;
        case COMPAT_DYNAMIC: out = "dynamic"; return out;
        case COMPAT_ITUNES:  out = "itunes";  return out;

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

Utility::JobContext::JobContext( string file_ )
    : file       ( file_ )
    , fileHandle ( MP4_INVALID_FILE_HANDLE )
    , optimize   ( false )
{
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
