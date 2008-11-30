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

class TrackUtility : public Utility
{
public:
    TrackUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );
};

///////////////////////////////////////////////////////////////////////////////

TrackUtility::TrackUtility( int argc, char** argv )
    : Utility( "mp4track", argc, argv )
{
    _usage = "[OPTION]... file...";
    _description =
        "\n<TODO-KB>";
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::utility_job( JobContext& job )
{
    verbose1f( "optimizing %s\n", job.file.c_str() );
    if( !MP4Optimize( job.file.c_str(), NULL ))
        return herrf( "optimize failed: %s\n", job.file.c_str() );

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::utility_option( int code, bool& handled )
{
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    TrackUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
