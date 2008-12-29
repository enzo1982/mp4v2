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
        LC_TRACK_WILDCARD = _LC_MAX,
        LC_TRACK_ID,
        LC_TRACK_INDEX,

        LC_SAMPLE_WILDCARD,
        LC_SAMPLE_ID,
        LC_SAMPLE_INDEX,

        LC_LIST,

        LC_COLR_PARMS,
        LC_COLR_PARM_HD,
        LC_COLR_PARM_SD,

        LC_COLR_LIST,
        LC_COLR_ADD,
        LC_COLR_SET,
        LC_COLR_REMOVE,

        LC_PASP_PARMS,

        LC_PASP_LIST,
        LC_PASP_ADD,
        LC_PASP_SET,
        LC_PASP_REMOVE,
    };

public:
    TrackUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );

private:
    bool actionList( JobContext& );

    bool actionColorParameterList   ( JobContext& );
    bool actionColorParameterAdd    ( JobContext& );
    bool actionColorParameterSet    ( JobContext& );
    bool actionColorParameterRemove ( JobContext& );

    bool actionPictureAspectRatioList   ( JobContext& );
    bool actionPictureAspectRatioAdd    ( JobContext& );
    bool actionPictureAspectRatioSet    ( JobContext& );
    bool actionPictureAspectRatioRemove ( JobContext& );

private:
    enum TrackMode {
        TM_INDEX,
        TM_ID,
        TM_WILDCARD,
    };

    enum SampleMode {
        SM_INDEX,
        SM_ID,
        SM_WILDCARD,
    };

    Group _actionGroup;
    Group _parmGroup;

    bool (TrackUtility::*_action)( JobContext& );

    TrackMode _trackMode;
    uint16_t  _trackIndex;
    uint32_t  _trackId;

    SampleMode _sampleMode;
    uint16_t   _sampleIndex;
    uint32_t   _sampleId;

    qtff::ColorParameterBox::Item     _colorParameterItem;
    qtff::PictureAspectRatioBox::Item _pictureAspectRatioItem;
};

///////////////////////////////////////////////////////////////////////////////

string toStringTrackType( string );

///////////////////////////////////////////////////////////////////////////////

TrackUtility::TrackUtility( int argc, char** argv )
    : Utility      ( "mp4track", argc, argv )
    , _actionGroup ( "ACTIONS" )
    , _parmGroup   ( "PARAMETERS" )
    , _action      ( NULL )
    , _trackMode   ( TM_WILDCARD )
    , _trackIndex  ( 0 )
    , _trackId     ( MP4_INVALID_TRACK_ID )
    , _sampleMode  ( SM_INDEX )
    , _sampleIndex ( 0 )
    , _sampleId    ( MP4_INVALID_SAMPLE_ID )
{
    // add standard options which make sense for this utility
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

    _parmGroup.add( "track-any",    false, LC_TRACK_WILDCARD,  "act on any track (default)" );
    _parmGroup.add( "track-index",  true,  LC_TRACK_INDEX,     "act on track index IDX", "IDX" );
    _parmGroup.add( "track-id",     true,  LC_TRACK_ID,        "act on track id ID", "ID" );
/*
    _parmGroup.add( "sample-any",   false, LC_SAMPLE_WILDCARD, "act on any sample (default)" );
    _parmGroup.add( "sample-index", true,  LC_SAMPLE_INDEX,    "act on sample index IDX" );
    _parmGroup.add( "sample-id",    true,  LC_SAMPLE_ID,       "act on sample id ID" );
*/
    _parmGroup.add( "colr-parms",   true,  LC_COLR_PARMS,      "where CSV is IDX1,IDX2,IDX3", "CSV" );
    _parmGroup.add( "colr-parm-hd", false, LC_COLR_PARM_HD,    "equivalent to --colr-parms=1,1,1" );
    _parmGroup.add( "colr-parm-sd", false, LC_COLR_PARM_SD,    "equivalent to --colr-parms=6,1,6" );
    _parmGroup.add( "pasp-parms",   true,  LC_PASP_PARMS,      "where CSV is hSPACING,vSPACING", "CSV" );
    _groups.push_back( &_parmGroup );

    _actionGroup.add( "list",        false, LC_LIST,        "list all tracks in mp4" );
    _actionGroup.add( "colr-list",   false, LC_COLR_LIST,   "list all colr-boxes in mp4" );
    _actionGroup.add( "colr-add",    false, LC_COLR_ADD,    "add colr-box to a video track" );
    _actionGroup.add( "colr-set",    false, LC_COLR_SET,    "set colr-box parms" );
    _actionGroup.add( "colr-remove", false, LC_COLR_REMOVE, "remove colr-box from track" );
    _actionGroup.add( "pasp-list",   false, LC_PASP_LIST,   "list all pasp-boxes in mp4" );
    _actionGroup.add( "pasp-add",    false, LC_PASP_ADD,    "add pasp-box to a video track" );
    _actionGroup.add( "pasp-set",    false, LC_PASP_SET,    "set pasp-box parms" );
    _actionGroup.add( "pasp-remove", false, LC_PASP_REMOVE, "remove pasp-box from track" );
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
TrackUtility::actionColorParameterAdd( JobContext& job )
{
    ostringstream oss;
    oss << "adding colr-box(" << _colorParameterItem.convertToCSV() << ") -> " << job.file;

    switch( _trackMode ) {
        case TM_INDEX:
            oss << " (track index=" << _trackIndex << ')';
            break;

        case TM_ID:
            oss << " (track id=" << _trackId << ')';
            break;

        default:
        case TM_WILDCARD:
            return herrf( "track not specified\n" );
    }

    verbose1f( "%s\n", oss.str().c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    switch( _trackMode ) {
        default:
        case TM_INDEX:
            if( qtff::ColorParameterBox::add( job.fileHandle, _trackIndex, _colorParameterItem ))
                return herrf( "unable to add colr-box\n" );
            break;

        case TM_ID:
            if( qtff::ColorParameterBox::add( job.fileHandle, _trackId, _colorParameterItem ))
                return herrf( "unable to add colr-box\n" );
            break;
    }

    job.fileWasModified = true;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionColorParameterList( JobContext& job )
{
    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    ostringstream report;

    const int widx = 3;
    const int wid = 3;
    const int wtype = 8;
    const int wparm = 6;
    const string sep = "  ";

    if( _jobCount == 0 ) {
        report << setw(widx) << right << "IDX"
               << sep << setw(wid) << "ID"
               << sep << setw(wtype) << left << "TYPE"
               << sep << setw(wparm) << right << "PRIMRY"
               << sep << setw(wparm) << right << "XFERFN"
               << sep << setw(wparm) << right << "MATRIX"
               << sep << setw(0) << "FILE"
               << '\n';

        report << setfill('-') << setw(70) << "" << setfill(' ') << '\n';
    }

    qtff::ColorParameterBox::ItemList itemList;
    if( qtff::ColorParameterBox::list( job.fileHandle, itemList ))
        return herrf( "unable to fetch list of colr-boxes" );

    const qtff::ColorParameterBox::ItemList::size_type max = itemList.size();
    for( qtff::ColorParameterBox::ItemList::size_type i = 0; i < max; i++ ) {
        const qtff::ColorParameterBox::IndexedItem& xitem = itemList[i];

        const char* type = MP4GetTrackType( job.fileHandle, xitem.trackId );
        if( !type)
            type = "unknown";

        report << right << setw(widx) << xitem.trackIndex
               << sep << setw(wid) << xitem.trackId
               << sep << setw(wtype) << left << toStringTrackType( type )
               << sep << setw(wparm) << right << xitem.item.primariesIndex
               << sep << setw(wparm) << right << xitem.item.transferFunctionIndex
               << sep << setw(wparm) << right << xitem.item.matrixIndex;

        if( i == 0 )
            report << sep << setw(0) << job.file;

        report << '\n';
    }

    verbose1f( "%s", report.str().c_str() );
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionColorParameterRemove( JobContext& job )
{
    ostringstream oss;
    oss << "removing colr-box from " << job.file;

    switch( _trackMode ) {
        case TM_INDEX:
            oss << " (track index=" << _trackIndex << ')';
            break;

        case TM_ID:
            oss << " (track id=" << _trackId << ')';
            break;

        default:
        case TM_WILDCARD:
            oss << " (all tracks)";
    }

    verbose1f( "%s\n", oss.str().c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    switch( _trackMode ) {
        case TM_INDEX:
            if( qtff::ColorParameterBox::remove( job.fileHandle, _trackIndex ))
                return herrf( "unable to remove colr-box\n" );
            break;

        case TM_ID:
            if( qtff::ColorParameterBox::remove( job.fileHandle, _trackId ))
                return herrf( "unable to remove colr-box\n" );
            break;

        default:
        case TM_WILDCARD:
        {
            qtff::ColorParameterBox::ItemList itemList;
            if( qtff::ColorParameterBox::list( job.fileHandle, itemList ))
                return herrf( "unable to fetch list of colr-boxes" );

            _trackMode = TM_INDEX;
            const qtff::ColorParameterBox::ItemList::size_type max = itemList.size();
            for( qtff::ColorParameterBox::ItemList::size_type i = 0; i < max; i++ ) {
                const qtff::ColorParameterBox::IndexedItem& xitem = itemList[i];
                _trackIndex = xitem.trackIndex;
                actionColorParameterRemove( job );
            }
            break;
        }
    }

    job.fileWasModified = true;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionColorParameterSet( JobContext& job )
{
    ostringstream oss;
    oss << "setting colr-box(" << _colorParameterItem.convertToCSV() << ") -> " << job.file;

    switch( _trackMode ) {
        case TM_INDEX:
            oss << " (track index=" << _trackIndex << ')';
            break;

        case TM_ID:
            oss << " (track id=" << _trackId << ')';
            break;

        default:
        case TM_WILDCARD:
            return herrf( "track not specified\n" );
    }

    verbose1f( "%s\n", oss.str().c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    switch( _trackMode ) {
        default:
        case TM_INDEX:
            if( qtff::ColorParameterBox::set( job.fileHandle, _trackIndex, _colorParameterItem ))
                return herrf( "unable to set colr-box\n" );
            break;

        case TM_ID:
            if( qtff::ColorParameterBox::set( job.fileHandle, _trackId, _colorParameterItem ))
                return herrf( "unable to set colr-box\n" );
            break;
    }

    job.fileWasModified = true;
    return SUCCESS;
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

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
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

        if( i == 0 )
            oss << sep << setw(0) << job.file;

        report << oss.str() << '\n';
    }

    verbose1f( "%s", report.str().c_str() );
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionPictureAspectRatioAdd( JobContext& job )
{
    ostringstream oss;
    oss << "adding pasp-box(" << _pictureAspectRatioItem.convertToCSV() << ") -> " << job.file;

    switch( _trackMode ) {
        case TM_INDEX:
            oss << " (track index=" << _trackIndex << ')';
            break;

        case TM_ID:
            oss << " (track id=" << _trackId << ')';
            break;

        default:
        case TM_WILDCARD:
            return herrf( "track not specified\n" );
    }

    verbose1f( "%s\n", oss.str().c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    switch( _trackMode ) {
        default:
        case TM_INDEX:
            if( qtff::PictureAspectRatioBox::add( job.fileHandle, _trackIndex, _pictureAspectRatioItem ))
                return herrf( "unable to add pasp-box\n" );
            break;

        case TM_ID:
            if( qtff::PictureAspectRatioBox::add( job.fileHandle, _trackId, _pictureAspectRatioItem ))
                return herrf( "unable to add pasp-box\n" );
            break;
    }

    job.fileWasModified = true;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionPictureAspectRatioList( JobContext& job )
{
    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for read: %s\n", job.file.c_str() );

    ostringstream report;

    const int widx = 3;
    const int wid = 3;
    const int wtype = 8;
    const int wparm = 6;
    const string sep = "  ";

    if( _jobCount == 0 ) {
        report << setw(widx) << right << "IDX"
               << sep << setw(wid) << "ID"
               << sep << setw(wtype) << left << "TYPE"
               << sep << setw(wparm) << right << "hSPACE"
               << sep << setw(wparm) << right << "vSPACE"
               << sep << setw(0) << "FILE"
               << '\n';

        report << setfill('-') << setw(70) << "" << setfill(' ') << '\n';
    }

    qtff::PictureAspectRatioBox::ItemList itemList;
    if( qtff::PictureAspectRatioBox::list( job.fileHandle, itemList ))
        return herrf( "unable to fetch list of pasp-boxes" );

    const qtff::PictureAspectRatioBox::ItemList::size_type max = itemList.size();
    for( qtff::PictureAspectRatioBox::ItemList::size_type i = 0; i < max; i++ ) {
        const qtff::PictureAspectRatioBox::IndexedItem& xitem = itemList[i];

        const char* type = MP4GetTrackType( job.fileHandle, xitem.trackId );
        if( !type)
            type = "unknown";

        report << right << setw(widx) << xitem.trackIndex
               << sep << setw(wid) << xitem.trackId
               << sep << setw(wtype) << left << toStringTrackType( type )
               << sep << setw(wparm) << right << xitem.item.hSpacing
               << sep << setw(wparm) << right << xitem.item.vSpacing;

        if( i == 0 )
            report << sep << setw(0) << job.file;

        report << '\n';
    }

    verbose1f( "%s", report.str().c_str() );
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionPictureAspectRatioRemove( JobContext& job )
{
    ostringstream oss;
    oss << "removing pasp-box from " << job.file;

    switch( _trackMode ) {
        case TM_INDEX:
            oss << " (track index=" << _trackIndex << ')';
            break;

        case TM_ID:
            oss << " (track id=" << _trackId << ')';
            break;

        default:
        case TM_WILDCARD:
            oss << " (all tracks)";
    }

    verbose1f( "%s\n", oss.str().c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    switch( _trackMode ) {
        case TM_INDEX:
            if( qtff::PictureAspectRatioBox::remove( job.fileHandle, _trackIndex ))
                return herrf( "unable to remove pasp-box\n" );
            break;

        case TM_ID:
            if( qtff::PictureAspectRatioBox::remove( job.fileHandle, _trackId ))
                return herrf( "unable to remove pasp-box\n" );
            break;

        default:
        case TM_WILDCARD:
        {
            qtff::PictureAspectRatioBox::ItemList itemList;
            if( qtff::PictureAspectRatioBox::list( job.fileHandle, itemList ))
                return herrf( "unable to fetch list of pasp-boxes" );

            _trackMode = TM_INDEX;
            const qtff::PictureAspectRatioBox::ItemList::size_type max = itemList.size();
            for( qtff::PictureAspectRatioBox::ItemList::size_type i = 0; i < max; i++ ) {
                const qtff::PictureAspectRatioBox::IndexedItem& xitem = itemList[i];
                _trackIndex = xitem.trackIndex;
                actionPictureAspectRatioRemove( job );
            }
            break;
        }
    }

    job.fileWasModified = true;
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
TrackUtility::actionPictureAspectRatioSet( JobContext& job )
{
    ostringstream oss;
    oss << "setting pasp-box(" << _pictureAspectRatioItem.convertToCSV() << ") -> " << job.file;

    switch( _trackMode ) {
        case TM_INDEX:
            oss << " (track index=" << _trackIndex << ')';
            break;

        case TM_ID:
            oss << " (track id=" << _trackId << ')';
            break;

        default:
        case TM_WILDCARD:
            return herrf( "track not specified\n" );
    }

    verbose1f( "%s\n", oss.str().c_str() );
    if( dryrunAbort() )
        return SUCCESS;

    job.fileHandle = MP4ReadCopy( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
        return herrf( "unable to open for write: %s\n", job.file.c_str() );

    switch( _trackMode ) {
        default:
        case TM_INDEX:
            if( qtff::PictureAspectRatioBox::set( job.fileHandle, _trackIndex, _pictureAspectRatioItem ))
                return herrf( "unable to set pasp-box\n" );
            break;

        case TM_ID:
            if( qtff::PictureAspectRatioBox::set( job.fileHandle, _trackId, _pictureAspectRatioItem ))
                return herrf( "unable to set pasp-box\n" );
            break;
    }

    job.fileWasModified = true;
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
        case LC_TRACK_WILDCARD:
            _trackMode = TM_WILDCARD;
            break;

        case LC_TRACK_INDEX:
        {
            _trackMode = TM_INDEX;
            istringstream iss( prog::optarg );
            iss >> _trackIndex;
            if( iss.rdstate() != ios::eofbit )
                return herrf( "invalid track index: %s\n", prog::optarg );
            break;
        }

        case LC_TRACK_ID:
        {
            _trackMode = TM_ID;
            istringstream iss( prog::optarg );
            iss >> _trackId;
            if( iss.rdstate() != ios::eofbit )
                return herrf( "invalid track id: %s\n", prog::optarg );
            break;
        }

        case LC_LIST:
            _action = &TrackUtility::actionList;
            break;

        case LC_COLR_PARMS:
            _colorParameterItem.convertFromCSV( prog::optarg );
            break;

        case LC_COLR_PARM_HD:
            _colorParameterItem.primariesIndex        = 1;
            _colorParameterItem.transferFunctionIndex = 1;
            _colorParameterItem.matrixIndex           = 1;
            break;

        case LC_COLR_PARM_SD:
            _colorParameterItem.primariesIndex        = 6;
            _colorParameterItem.transferFunctionIndex = 1;
            _colorParameterItem.matrixIndex           = 6;
            break;

        case LC_COLR_LIST:
            _action = &TrackUtility::actionColorParameterList;
            break;

        case LC_COLR_ADD:
            _action = &TrackUtility::actionColorParameterAdd;
            break;

        case LC_COLR_SET:
            _action = &TrackUtility::actionColorParameterSet;
            break;

        case LC_COLR_REMOVE:
            _action = &TrackUtility::actionColorParameterRemove;
            break;

        case LC_PASP_PARMS:
            _pictureAspectRatioItem.convertFromCSV( prog::optarg );
            break;

        case LC_PASP_LIST:
            _action = &TrackUtility::actionPictureAspectRatioList;
            break;

        case LC_PASP_ADD:
            _action = &TrackUtility::actionPictureAspectRatioAdd;
            break;

        case LC_PASP_SET:
            _action = &TrackUtility::actionPictureAspectRatioSet;
            break;

        case LC_PASP_REMOVE:
            _action = &TrackUtility::actionPictureAspectRatioRemove;
            break;

        default:
            handled = false;
            break;
    }

    return SUCCESS;
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

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    mp4v2::util::TrackUtility util( argc, argv );
    return util.process();
}
