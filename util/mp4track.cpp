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

class TrackUtility : public Utility
{
private:
    enum TrackLongAction {
        LC_TRACK_ANY = _LC_MAX,
        LC_TRACK_ID,
        LC_TRACK_INDEX,
        LC_SAMPLE_ANY,
        LC_SAMPLE_ID,
        LC_SAMPLE_INDEX,
        LC_LIST,
    };

public:
    TrackUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );

private:
    bool actionList( JobContext& );

private:
    Group _actionGroup;
    Group _parmGroup;

    bool (TrackUtility::*_action)( JobContext& );
    string _listLastFile;
};

///////////////////////////////////////////////////////////////////////////////

string toStringTrackType( string );

///////////////////////////////////////////////////////////////////////////////

TrackUtility::TrackUtility( int argc, char** argv )
    : Utility      ( "mp4track", argc, argv )
    , _actionGroup ( "ACTIONS" )
    , _parmGroup   ( "PARAMETERS" )
    , _action      ( NULL )
{
    // add standard options which make sense for this utility
    _group.add( STD_OPTIMIZE );
    _group.add( STD_DRYRUN );
    _group.add( STD_KEEPGOING );
    _group.add( STD_OVERWRITE );
    _group.add( STD_FORCE );
    _group.add( STD_QUIET );
    _group.add( STD_DEBUG );
    _group.add( STD_VERBOSE );
    _group.add( STD_HELP );
    _group.add( STD_VERSION );
    _group.add( STD_VERSIONX );

    _parmGroup.add( "track-any",    false, LC_TRACK_ANY,    "any track" );
    _parmGroup.add( "track-id",     true,  LC_TRACK_ID,     "track by id" );
    _parmGroup.add( "track-index",  true,  LC_TRACK_INDEX,  "track by index" );
    _parmGroup.add( "sample-any",   false, LC_SAMPLE_ANY,   "any sample" );
    _parmGroup.add( "sample-id",    true,  LC_SAMPLE_ID,    "sample by id" );
    _parmGroup.add( "sample-index", true,  LC_SAMPLE_INDEX, "sample by index" );
    _groups.push_back( &_parmGroup );

    _actionGroup.add( "list", false, LC_LIST, "list tracks in mp4" );
    _groups.push_back( &_actionGroup );

    _usage = "[OPTION]... [PARAMETERS]... ACTION file...";
    _description =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nFor each mp4 file specified, perform the specified ACTION. An action must be"
        "\nspecified. Some options are not applicable to some actions.";
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionList( JobContext& job )
{
    ostringstream report;

    const int widx = 3;
    const int wid = 3;
    const int wtype = 8;
    const int wflags = 6;
    const int wnsamp = 6;
    const string sep = "  ";

    if( _jobCount == 0 ) {
        report << setw(widx) << right << "IDX" << left
               << ' '
               << sep << setw(wid) << right << "ID" << left
               << sep << setw(wtype) << "TYPE"
               << sep << setw(wflags) << "FLAGS"
               << sep << setw(wnsamp) << "#SAMPL"
               << sep << setw(0) << "FILE"
               << '\n';

        report << setfill('-') << setw(70) << "" << setfill(' ') << '\n';
    }

    job.fileHandle = MP4Read( job.file.c_str() );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    const uint32_t trackc = MP4GetNumberOfTracks( job.fileHandle );
    for( uint32_t i = 0; i < trackc; i++ ) {
        ostringstream oss;
        oss << right << setw(widx) << i << left;

        MP4TrackId id = MP4FindTrackId( job.fileHandle, i );
        if( id == MP4_INVALID_TRACK_ID ) {
            report << oss.str() << " [invalid]\n";
            continue;
        }

        uint64_t flags;
        if( !MP4GetTrackIntegerProperty( job.fileHandle, id, "tkhd.flags", &flags )) {
            report << oss.str() << " [invalid]\n";
            continue;
        }

        uint64_t version;
        if( !MP4GetTrackIntegerProperty( job.fileHandle, id, "tkhd.version", &version )) {
            report << oss.str() << " [invalid]\n";
            continue;
        }

        const char* type = MP4GetTrackType( job.fileHandle, id );
        if( !type) {
            report << oss.str() << " [invalid]\n";
            continue;
        }

        MP4SampleId nsamp = MP4GetTrackNumberOfSamples( job.fileHandle, id );

        oss << (version == 0 ? ' ' : '*')
            << sep << setw(wid) << right << id << left
            << sep << setw(wtype) << toStringTrackType( type )
            << sep << setw(wflags) << setfill('0') << right << hex << (flags & 0x00ffffff) << left << dec
            << sep << setw(wnsamp) << nsamp;

        if( job.file != _listLastFile ) {
            _listLastFile = job.file;
            oss << sep << job.file;
        }

        report << oss.str() << '\n';
    }

    verbose1f( "%s", report.str().c_str() );
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::utility_job( JobContext& job )
{
    if( !_action )
        return herrf( "no action specified\n" );

    return (this->*_action)( job );
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::utility_option( int code, bool& handled )
{
    handled = true;

    switch( code ) {
        case LC_LIST:
            _action = &TrackUtility::actionList;
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
    TrackUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

string
toStringTrackType( string code )
{
    if( !code.compare( "vide" ))    // 14496-12
        return "video";
    if( !code.compare( "soun" ))    // 14496-12
        return "audio";
    if( !code.compare( "hint" ))    // 14496-12
        return "hint";

    if( !code.compare( "text" ))    // QTFF
        return "text";
    if( !code.compare( "tmcd" ))    // QTFF
        return "timecode";

    return string( "(" ) + code + ")";
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
