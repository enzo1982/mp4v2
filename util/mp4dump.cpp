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

class DumpUtility : public Utility
{
public:
    DumpUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );
};

///////////////////////////////////////////////////////////////////////////////

DumpUtility::DumpUtility( int argc, char** argv )
    : Utility    ( "mp4dump", argc, argv )
{
    _options.remove( "optimize" );
    _options.remove( "compat" );
    _options.remove( "strict" );
    _options.remove( "dryrun" );
    _options.remove( "overwrite" );
    _options.remove( "force" );
    _options.remove( "quiet" );

    _usage = "[OPTION]... file...";
    _description =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nFor each mp4 file specified, dump the file contents in a human-readable"
        "\ntext format, to stdout.";
}

///////////////////////////////////////////////////////////////////////////////

bool
DumpUtility::utility_job( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    verbose1f( "dumping %s\n", job.file.c_str() );

    if( !MP4Dump( job.fileHandle, stdout, _debugImplicits ))
        return herrf( "dump failed: %s\n", job.file.c_str() );

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
DumpUtility::utility_option( int code, bool& handled )
{
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    DumpUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
