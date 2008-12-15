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
//  The Initial Developer of the Original Code is Ullrich Pollaehne.
//  Portions created by Kona Blend are Copyright (C) 2008.
//  All Rights Reserved.
//
//  Contributors:
//      Kona Blend, kona8lend@@gmail.com
//      Ullrich Pollaehne, u.pollaehne@@gmail.com
//
///////////////////////////////////////////////////////////////////////////////
#include "libutil/util.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

class ChapterUtility : public Utility
{
public:
    ChapterUtility( int, char** );

protected:
    // delegates implementation
    bool utility_option( int, bool& );
    bool utility_job( JobContext& );

private:
    bool actionList    ( JobContext& );
    bool actionConvert ( JobContext& );
    bool actionEvery   ( JobContext& );
    bool actionExport  ( JobContext& );
    bool actionImport  ( JobContext& );
    bool actionRemove  ( JobContext& );

private:
    Group  _actionGroup;
    Group  _parmGroup;

    bool       (ChapterUtility::*_action)( JobContext& );
    void       fixQtScale(MP4FileHandle );
    MP4TrackId getReferencingTrack( MP4FileHandle );
    string     getChapterTypeName( MP4ChapterType ) const;
    bool       readChapterFile( const char*, char**, io::StdioFile::Size& );

    MP4ChapterType _ChapterType;
    uint32_t       _ChaptersEvery;

    string _stTextFile;
};

///////////////////////////////////////////////////////////////////////////////

ChapterUtility::ChapterUtility( int argc, char** argv )
    : Utility        ( "mp4chaps", argc, argv )
    , _actionGroup   ( "ACTIONS" )
    , _parmGroup     ( "ACTION PARAMETERS" )
    , _action        ( NULL )
    , _ChapterType   ( MP4ChapterTypeAny )
    , _ChaptersEvery ( 0 )
{
    // add standard options which make sense for this utility
    _group.add( STD_KEEPGOING );
    _group.add( STD_OVERWRITE );
    _group.add( STD_FORCE );
    _group.add( STD_QUIET );
    _group.add( STD_DEBUG );
    _group.add( STD_VERBOSE );
    _group.add( STD_HELP );
    _group.add( STD_VERSION );
    _group.add( STD_VERSIONX );

    _parmGroup.add( 'A', false, "chapter-any",  false, LC_NONE, "act on any chapter type (default)" );
    _parmGroup.add( 'Q', false, "chapter-qt",   false, LC_NONE, "act on QuickTime chapters" );
    _parmGroup.add( 'N', false, "chapter-nero", false, LC_NONE, "act on Nero chapters" );
    _groups.push_back( &_parmGroup );

    _actionGroup.add( 'l', false, "list",    false, LC_NONE, "list available chapters" );
    _actionGroup.add( 'c', false, "convert", false, LC_NONE, "convert available chapters" );
    _actionGroup.add( 'e', true,  "every",   true,  LC_NONE, "create chapters every NUM seconds", "NUM" );
    _actionGroup.add( 'x', false, "export",  false, LC_NONE, "export chapters to file.chapters.txt" );
    _actionGroup.add( 'i', false, "import",  false, LC_NONE, "import chapters from file.chapters.txt" );
    _actionGroup.add( 'r', false, "remove",  false, LC_NONE, "remove all chapters" );
    _groups.push_back( &_actionGroup );

    _usage = "[OPTION]... ACTION [ACTION PARAMETERS] file...";
    _description =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\nFor each mp4 file specified, perform the specified ACTION. An action must be"
        "\nspecified. Some options are not applicable to some actions.";
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::actionList( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
    {
        return herrf( "unable to open for read: %s\n", job.file.c_str() );
    }

    MP4Chapter_t * chapters = 0;
    uint32_t chapterCount = 0;

    // get the list of chapters
    MP4ChapterType chtp = MP4GetChapters(job.fileHandle, &chapters, &chapterCount, _ChapterType);
    if (0 == chapterCount)
    {
        verbose1f( "File %s does not contain chapters of type %s\n", job.file.c_str(), getChapterTypeName( chtp ).c_str() );
        return SUCCESS;
    }

    // start output (more or less like mp4box does)
    fprintf( stdout, "%s Chapters:\n", getChapterTypeName( chtp ).c_str() );

    Timecode duration(0, 1000.0);
    for (uint32_t i = 0; i < chapterCount; ++i)
    {
        // print the infos
        fprintf(stdout, "\tChapter #%u - %s - \"%s\"\n", i+1, duration.svalue.c_str(), chapters[i].title);

        // add the duration of this chapter to the sum (is the start time of the next chapter)
        duration += Timecode(chapters[i].duration, 1000.0);
    }

    // free up the memory
    MP4Free(chapters);

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::actionConvert( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
    {
        return herrf( "unable to open for write: %s\n", job.file.c_str() );
    }

    MP4ChapterType chtp = MP4ConvertChapters( job.fileHandle, _ChapterType );
    if( MP4ChapterTypeNone == chtp )
    {
        return herrf( "File %s does not contain chapters of type %s\n", job.file.c_str(), getChapterTypeName( chtp ).c_str() );
    }

    fixQtScale( job.fileHandle );
    job.fileWasModified = true;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::actionEvery( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
    {
        return herrf( "unable to open for write: %s\n", job.file.c_str() );
    }

    MP4TrackId refTrackId = getReferencingTrack( job.fileHandle );
    if( !MP4_IS_VALID_TRACK_ID(refTrackId) )
    {
        return herrf( "unable to find a video or audio track in file %s\n", job.file.c_str() );
    }

    Timecode refTrackDuration( MP4GetTrackDuration( job.fileHandle, refTrackId ), MP4GetTrackTimeScale( job.fileHandle, refTrackId ) );
    refTrackDuration.setScale( 1000.0 );

    Timecode chapterDuration( _ChaptersEvery * 1000, 1000.0 );
    Timecode durationSum( 0, 1000.0 );
    vector<MP4Chapter_t> chapters;

    while( durationSum + chapterDuration < refTrackDuration )
    {
        MP4Chapter_t chap;
        chap.duration = chapterDuration.duration;
        sprintf(chap.title, "Chapter %i", chapters.size()+1);

        chapters.push_back( chap );

        durationSum += chapterDuration;
    }

    if( 0 < chapters.size() )
    {
        chapters.back().duration = (refTrackDuration - (durationSum - chapterDuration)).duration;

        MP4SetChapters(job.fileHandle, &chapters[0], chapters.size(), _ChapterType);
    }

    fixQtScale( job.fileHandle );
    job.fileWasModified = true;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::actionExport( JobContext& job )
{
    job.fileHandle = MP4Read( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
    {
        return herrf( "unable to open for read: %s\n", job.file.c_str() );
    }

    // get the list of chapters
    MP4Chapter_t*  chapters = 0;
    uint32_t       chapterCount = 0;
    MP4ChapterType chtp = MP4GetChapters( job.fileHandle, &chapters, &chapterCount, _ChapterType );
    if (0 == chapterCount)
    {
        return herrf( "File %s does not contain chapters of type %s\n", job.file.c_str(), getChapterTypeName( chtp ).c_str() );
    }

    // build the filename
    // compute out filename
    string outName = job.file;
    io::FileSystem::pathnameStripExtension( outName );
    outName.append( ".chapters.txt" );

    // open the file
    io::StdioFile out( outName );
    if( openFileForWriting( out ) )
    {
        return FAILURE;
    }

    // write the chapters
    char fileLine[2048] = {0};
    io::StdioFile::Size nout;
    bool failure = SUCCESS;
    Timecode duration( 0, 1000.0 );
    for( uint32_t i = 0; i < chapterCount; ++i )
    {
        // print the infos
        sprintf( fileLine, "%s %s\n", duration.svalue.c_str(), chapters[i].title );
        if( out.write(fileLine, strlen(fileLine), nout) )
        {
            failure = herrf( "write to %s failed: %s\n", outName.c_str(), sys::getLastErrorStr() );
            break;
        }

        // add the duration of this chapter to the sum (the start time of the next chapter)
        duration += Timecode(chapters[i].duration, 1000.0);
    }
    out.close();
    if( failure )
    {
        verbose1f( "removing file %s\n", outName.c_str() );
        ::remove( outName.c_str() );
    }

    // free up the memory
    MP4Free(chapters);

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::actionImport( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
    {
        return herrf( "unable to open for write: %s\n", job.file.c_str() );
    }

    MP4TrackId refTrackId = getReferencingTrack( job.fileHandle );
    if( !MP4_IS_VALID_TRACK_ID(refTrackId) )
    {
        return herrf( "unable to find a video or audio track in file %s\n", job.file.c_str() );
    }

    Timecode refTrackDuration( MP4GetTrackDuration( job.fileHandle, refTrackId ), MP4GetTrackTimeScale( job.fileHandle, refTrackId ) );
    refTrackDuration.setScale( 1000.0 );

    // get the content
    char * inBuf;
    io::StdioFile::Size fileSize;
    if( readChapterFile( job.file.c_str(), &inBuf, fileSize ) )
    {
        return FAILURE;
    }

    // separate the text lines
    char* pos = inBuf;
    while (pos < inBuf + fileSize)
    {
        if (*pos == '\n' || *pos == '\r')
        {
            *pos = 0;
            if (pos > inBuf)
            {
                // remove trailing whitespace
                char* tmp = pos-1;
                while ((*tmp == ' ' || *tmp == '\t') && tmp > inBuf)
                {
                    *tmp = 0;
                    tmp--;
                }
            }
        }
        pos++;
    }
    pos = inBuf;

    vector<MP4Chapter_t> chapters;

    // parse the lines
    bool failure = false;
    while (pos < inBuf + fileSize)
    {
        if (*pos == 0 || *pos == ' ' || *pos == '\t')
        {
            // uninteresting chars
            pos++;
            continue;
        }
        else if (*pos == '#')
        {
            // comment line
            pos += strlen(pos);
            continue;
        }
        else if (isdigit(*pos))
        {
            MP4Chapter_t chap;

            char* timeStart = pos;

            // read the title if there is one
            char* titleStart = strchr(timeStart, ' ');
            if (titleStart == NULL)
            {
                titleStart = strchr(timeStart, '\t');
            }

            if (titleStart != NULL)
            {
                *titleStart = 0;
                pos = ++titleStart;

                while (*titleStart == ' ' || *titleStart == '\t')
                {
                    titleStart++;
                }

                int titleLen = min(strlen(titleStart), (size_t)MP4V2_CHAPTER_TITLE_MAX);
                strncpy(chap.title, titleStart, titleLen);
                chap.title[titleLen] = 0;
            }

            Timecode tc( 0, 1000.0 );
            string tm( timeStart );
            if( tc.parse( tm ) )
            {
                failure = true;
                break;
            }

            chap.duration = tc.duration;
            chapters.push_back( chap );
        }

        pos += strlen(pos);
    }
    free( inBuf );
    if( failure )
    {
        return failure;
    }


    // convert start time into duration
    if (0 < chapters.size())
    {
        int chapterIndex = chapters.size();
        // convert 0..n-1
        for (int i = 0; i < chapterIndex-1; ++i)
        {
            chapters[i].duration = chapters[i+1].duration - chapters[i].duration;
        }

        // convert last chapter
        chapters[chapterIndex-1].duration = (refTrackDuration - Timecode( chapters[chapterIndex-1].duration, 1000.0 )).duration;

        // now set the chapters
        MP4SetChapters( job.fileHandle, &chapters[0], chapters.size(), _ChapterType );
    }

    fixQtScale( job.fileHandle );
    job.fileWasModified = true;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::actionRemove( JobContext& job )
{
    job.fileHandle = MP4Modify( job.file.c_str(), _debugVerbosity );
    if( job.fileHandle == MP4_INVALID_FILE_HANDLE )
    {
        return herrf( "unable to open for write: %s\n", job.file.c_str() );
    }

    MP4ChapterType chtp = MP4DeleteChapters( job.fileHandle, _ChapterType );
    if( MP4ChapterTypeNone == chtp )
    {
        return FAILURE;
    }

    fixQtScale( job.fileHandle );
    job.fileWasModified = true;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::utility_job( JobContext& job )
{
    if( !_action )
        return herrf( "no action specified\n" );

    return (this->*_action)( job );
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::utility_option( int code, bool& handled )
{
    handled = true;

    switch( code ) {
        case 'A':
            _ChapterType = MP4ChapterTypeAny;
            break;

        case 'Q':
            _ChapterType = MP4ChapterTypeQt;
            break;

        case 'N':
            _ChapterType = MP4ChapterTypeNero;
            break;

        case 'l':
            _action = &ChapterUtility::actionList;
            break;

        case 'e':
        {
            istringstream iss( prog::optarg );
            iss >> _ChaptersEvery;
            if( iss.rdstate() != ios::eofbit )
                return herrf( "invalid number of seconds: %s\n", prog::optarg );
            _action = &ChapterUtility::actionEvery;
            break;
        }

        case 'x':
            _action = &ChapterUtility::actionExport;
            //_stTextFile = prog::optarg;
            //if( _stTextFile.empty() )
            //{
            //    return herrf( "invalid TXT file: empty-string\n" );
            //}
            break;

        case 'i':
            _action = &ChapterUtility::actionImport;
            //_stTextFile = prog::optarg;
            //if( _stTextFile.empty() )
            //{
            //    return herrf( "invalid TXT file: empty-string\n" );
            //}
            break;

        case 'c':
            _action = &ChapterUtility::actionConvert;
            if( MP4ChapterTypeAny == _ChapterType )
            {
                return herrf( "invalid chapter type: define the chapter type to convert to\n" );
            }
            break;

        case 'r':
            _action = &ChapterUtility::actionRemove;
            break;

        default:
            handled = false;
            break;
    }

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

void
ChapterUtility::fixQtScale(MP4FileHandle file)
{
    // get around a QuickTime/iPod issue with storing the number of samples in a signed 32Bit value
    if( INT_MAX < (MP4GetDuration(file) * MP4GetTimeScale(file)) )
    {
        // timescale too high, lower it
        MP4ChangeMovieTimeScale(file, 1000);
    }
}

///////////////////////////////////////////////////////////////////////////////

MP4TrackId
ChapterUtility::getReferencingTrack( MP4FileHandle file )
{
    uint32_t trackCount = MP4GetNumberOfTracks( file );
    if( 0 == trackCount )
    {
        return MP4_INVALID_TRACK_ID;
    }

    MP4TrackId refTrackId = MP4_INVALID_TRACK_ID;
    for( uint32_t i = 0; i < trackCount; ++i )
    {
        MP4TrackId    id = MP4FindTrackId( file, i );
        const char* type = MP4GetTrackType( file, id );
        if( MP4_IS_VIDEO_TRACK_TYPE( type ) )
        {
            refTrackId = id;
            break;
        }
        else if( MP4_IS_AUDIO_TRACK_TYPE( type ) )
        {
            refTrackId = id;
            break;
        }
    }

    return refTrackId;
}

///////////////////////////////////////////////////////////////////////////////

string
ChapterUtility::getChapterTypeName( MP4ChapterType chapterType) const
{
    if( chapterType == MP4ChapterTypeQt )
    {
        return string( "QuickTime" );
    }
    else if( chapterType == MP4ChapterTypeNero )
    {
        return string( "Nero" );
    }
    else if( chapterType == MP4ChapterTypeAny )
    {
        return string( "QuickTime or Nero" );
    }

    return string( "Unknown" );
}

///////////////////////////////////////////////////////////////////////////////

bool
ChapterUtility::readChapterFile(const char* mp4Filename, char** buffer, io::StdioFile::Size &fileSize)
{
    const string inMode = "rb";

    // create the chapter file name
    string inName = mp4Filename;
    io::FileSystem::pathnameStripExtension( inName );
    inName.append( ".chapters.txt" );


    // open the file
    io::StdioFile in( inName );
    io::StdioFile::Size nin;
    if( in.open( inMode ) )
    {
        return herrf( "opening chapter file '%s' failed: %s\n", inName.c_str(), sys::getLastErrorStr() );
    }

    // get the file size
    fileSize = 0;
    if( in.getSize(fileSize) || 0 >= fileSize )
    {
        in.close();
        return herrf( "getting size of chapter file '%s' failed: %s\n", inName.c_str(), sys::getLastErrorStr() );
    }

    // allocate a buffer for the file and read the content
    char* inBuf = static_cast<char*>( malloc( fileSize+1 ) );
    if( in.read( inBuf, fileSize, nin ) )
    {
        in.close();
        return herrf( "reading chapter file '%s' failed: %s\n", inName.c_str(), sys::getLastErrorStr() );
    }
    in.close();
    inBuf[fileSize] = 0;

    *buffer = inBuf;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    using namespace mp4v2::util;

    sinit(); // libutil static initializer
    ChapterUtility util( argc, argv );
    const bool result = util.process();
    sshutdown(); // libutil static initializer
    return result;
}
