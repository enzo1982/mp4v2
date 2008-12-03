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
//      Edward Groenendaal, egroenen@@cisco.com
//
///////////////////////////////////////////////////////////////////////////////

#include "libutil/util.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

class SubtitleUtility : public Utility
{
private:
    enum Action {
        ACTION_UNDEFINED,
        ACTION_LIST,
        ACTION_EXTRACT,
        ACTION_MODIFY,
        ACTION_REMOVE,
    };

public:
    SubtitleUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );

private:
    bool actionList    ( JobContext& );
    bool actionExtract ( JobContext& );
    bool actionModify  ( JobContext& );
    bool actionRemove  ( JobContext& );

private:
    Group  _actionGroup;
    Action _action;
    string _argtxt;
};

///////////////////////////////////////////////////////////////////////////////

SubtitleUtility::SubtitleUtility( int argc, char** argv )
    : Utility      ( "mp4subtitle", argc, argv )
    , _actionGroup ( "ACTIONS" )
    , _action      ( ACTION_UNDEFINED )
{
    _group.remove( "optimize" );
    _group.remove( "compat" );
    _group.remove( "strict" );

    _actionGroup.add( 'l', false, "list",    false, LC_NONE, "list available subtitles" );
    _actionGroup.add( 'x', true,  "extract", true,  LC_NONE, "extract subtitles to TXT", "TXT" );
    _actionGroup.add( 'm', true,  "modify",  true,  LC_NONE, "set subtitles from TXT", "TXT" );
    _actionGroup.add( 'r', false, "remove",  false, LC_NONE, "remove all subtitles" );
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
SubtitleUtility::actionExtract( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    verbose1f( "NOT IMPLEMENTED\n" );
    return FAILURE;
}

///////////////////////////////////////////////////////////////////////////////

bool
SubtitleUtility::actionList( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    verbose1f( "NOT IMPLEMENTED\n" );
    return FAILURE;
}

///////////////////////////////////////////////////////////////////////////////

bool
SubtitleUtility::actionModify( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    verbose1f( "NOT IMPLEMENTED\n" );
    return FAILURE;
}

///////////////////////////////////////////////////////////////////////////////

bool
SubtitleUtility::actionRemove( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    verbose1f( "NOT IMPLEMENTED" );
    return FAILURE;
}

///////////////////////////////////////////////////////////////////////////////

bool
SubtitleUtility::utility_job( JobContext& job )
{
    bool result = FAILURE;

    switch( _action ) {
        case ACTION_UNDEFINED:
            return herrf( "no action specified\n" );

        case ACTION_LIST:
            result = actionList( job );
            break;

        case ACTION_EXTRACT:
            result = actionExtract( job );
            break;

        case ACTION_MODIFY:
            result = actionModify( job );
            break;

        case ACTION_REMOVE:
            result = actionRemove( job );
            break;

        default:
            return herrf( "unknown action(%d)\n", _action );
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////

bool
SubtitleUtility::utility_option( int code, bool& handled )
{
    handled = true;

    switch( code ) {
        case 'l':
            _action = ACTION_LIST;
            break;

        case 'x':
            _action = ACTION_EXTRACT;
            _argtxt = prog::optarg;
            if( _argtxt.empty() )
                return herrf( "invalid TXT file: empty-string\n" );
            break;

        case 'm':
            _action = ACTION_MODIFY;
            _argtxt = prog::optarg;
            if( _argtxt.empty() )
                return herrf( "invalid TXT file: empty-string\n" );
            break;

        case 'r':
            _action = ACTION_REMOVE;
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
    SubtitleUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
