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

#include "libutil/util.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

class FileUtility : public Utility
{
private:
    enum Action {
        ACTION_UNDEFINED,
        ACTION_LIST,
        ACTION_OPTIMIZE,
        ACTION_DUMP,
    };

public:
    FileUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );

private:
    bool actionList     ( JobContext& );
    bool actionOptimize ( JobContext& );
    bool actionDump     ( JobContext& );

private:
    Group  _actionGroup;
    Action _action;
};

///////////////////////////////////////////////////////////////////////////////

FileUtility::FileUtility( int argc, char** argv )
    : Utility      ( "mp4file", argc, argv )
    , _actionGroup ( "ACTIONS" )
    , _action      ( ACTION_UNDEFINED )
{
    _group.remove( "optimize" );
    _group.remove( "compat" );
    _group.remove( "strict" );
    _group.remove( "overwrite" );
    _group.remove( "force" );

    _actionGroup.add( 'l', false, "list",     false, LC_NONE, "list (summary information)" );
    _actionGroup.add( 'z', false, "optimize", false, LC_NONE, "optimize mp4 structure" );
    _actionGroup.add( 'p', false, "dump",     false, LC_NONE, "dump mp4 structure in human-readable format" );
    _groups.push_back( &_actionGroup );

    _usage = "[OPTION]... ACTION file...";
    _description =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nFor each mp4 file specified, perform the specified ACTION. An action must be"
        "\nspecified. Some options are not applicable to some actions.";
}

///////////////////////////////////////////////////////////////////////////////

bool
FileUtility::actionDump( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    if( !MP4Dump( job.fileHandle, stdout, _debugImplicits ))
        return herrf( "dump failed: %s\n", job.file.c_str() );

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileUtility::actionList( JobContext& job )
{
    ostringstream report;

    const int wbrand = 5;
    const int wcompat = 18;
    const int wsizing = 6;
    const string sep = "  ";

    if( _jobCount == 0 ) {
        report << setw(wbrand) << left << "BRAND" 
               << sep << setw(wcompat) << left << "COMPAT" 
               << sep << setw(wsizing) << left << "SIZING" 
               << sep << setw(0) << "FILE"
               << '\n';

        report << setfill('-') << setw(70) << "" << setfill(' ') << '\n';
    }

    job.fileHandle = MP4Read( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    FileSummaryInfo info;
    if( fetchFileSummaryInfo( job.fileHandle, info ))
        return herrf( "unable to fetch file summary info" );

    string compat;
    {
        const FileSummaryInfo::BrandSet::iterator end = info.compatible_brands.end();
        int count = 0;
        for( FileSummaryInfo::BrandSet::iterator it = info.compatible_brands.begin(); it != end; it++, count++ ) {
            if( count > 0 )
                compat += ',';
            compat += *it;
        }
    }

    const bool sizing = info.nlargesize | info.nversion1 | info.nspecial;

    report << setw(wbrand) << left << info.major_brand
           << sep << setw(wcompat) << left << compat
           << sep << setw(wsizing) << left << (sizing ? "64-bit" : "32-bit")
           << sep << job.file
           << '\n';

    verbose1f( "%s", report.str().c_str() );
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileUtility::actionOptimize( JobContext& job )
{
    verbose1f( "optimizing %s\n", job.file.c_str() );

    if( dryrunAbort() )
        return SUCCESS;

    if( !MP4Optimize( job.file.c_str(), NULL ))
        return herrf( "optimize failed: %s\n", job.file.c_str() );

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileUtility::utility_job( JobContext& job )
{
    bool result = FAILURE;

    switch( _action ) {
        case ACTION_UNDEFINED:
            return herrf( "no action specified\n" );

        case ACTION_LIST:
            result = actionList( job );
            break;

        case ACTION_OPTIMIZE:
            result = actionOptimize( job );
            break;

        case ACTION_DUMP:
            result = actionDump( job );
            break;

        default:
            return herrf( "unknown action(%d)\n", _action );
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool
FileUtility::utility_option( int code, bool& handled )
{
    handled = true;

    switch( code ) {
        case 'l':
            _action = ACTION_LIST;
            break;

        case 'z':
            _action = ACTION_OPTIMIZE;
            break;

        case 'p':
            _action = ACTION_DUMP;
            break;

        default:
            handled = false;
            break;
    }

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    FileUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
