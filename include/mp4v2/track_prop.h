#ifndef MP4V2_TRACK_PROP_H
#define MP4V2_TRACK_PROP_H

/**************************************************************************//**
 *
 *  @defgroup mp4_track_prop MP4v2 Track Property
 *  @{
 *
 *****************************************************************************/

/* specific track properties */

MP4V2_EXPORT
bool MP4HaveTrackAtom(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   atomname );

MP4V2_EXPORT
const char* MP4GetTrackType(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
const char* MP4GetTrackMediaDataName(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

/*
 * MP4GetTrackMediaDataOriginalFormat is to be used to get the original
 * MediaDataName if a track has been encrypted.
 */

MP4V2_EXPORT
bool MP4GetTrackMediaDataOriginalFormat(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    char*         originalFormat,
    uint32_t      buflen );

MP4V2_EXPORT
MP4Duration MP4GetTrackDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
uint32_t MP4GetTrackTimeScale(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
void MP4SetTrackTimeScale(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint32_t      value );

MP4V2_EXPORT
uint8_t MP4GetTrackAudioMpeg4Type(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
uint8_t MP4GetTrackEsdsObjectTypeId(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

/* returns MP4_INVALID_DURATION if track samples do not have a fixed duration */
MP4V2_EXPORT
MP4Duration MP4GetTrackFixedSampleDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
uint32_t MP4GetTrackBitRate(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
bool MP4GetTrackVideoMetadata(
    MP4FileHandle hFile,
    MP4TrackId trackId,
    uint8_t**  ppConfig,
    uint32_t*  pConfigSize );

MP4V2_EXPORT
bool MP4GetTrackESConfiguration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint8_t**     ppConfig,
    uint32_t*     pConfigSize );

MP4V2_EXPORT
bool MP4SetTrackESConfiguration(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const uint8_t* pConfig,
    uint32_t       configSize );

/* h264 information routines */
MP4V2_EXPORT
bool MP4GetTrackH264ProfileLevel(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint8_t*      pProfile,
    uint8_t*      pLevel );

MP4V2_EXPORT
void MP4GetTrackH264SeqPictHeaders(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint8_t***    pSeqHeaders,
    uint32_t**    pSeqHeaderSize,
    uint8_t***    pPictHeader,
    uint32_t**    pPictHeaderSize );

MP4V2_EXPORT
bool MP4GetTrackH264LengthSize(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint32_t*     pLength );

MP4V2_EXPORT
MP4SampleId MP4GetTrackNumberOfSamples(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
uint16_t MP4GetTrackVideoWidth(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
uint16_t MP4GetTrackVideoHeight(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
double MP4GetTrackVideoFrameRate(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
int MP4GetTrackAudioChannels(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
bool MP4IsIsmaCrypMediaTrack(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

/* generic track properties */

MP4V2_EXPORT
bool MP4HaveTrackAtom(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   atomName );

MP4V2_EXPORT
bool MP4GetTrackIntegerProperty(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   propName,
    uint64_t*     retvalue );

MP4V2_EXPORT
bool MP4GetTrackFloatProperty(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   propName,
    float*        ret_value );

MP4V2_EXPORT
bool MP4GetTrackStringProperty(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   propName,
    const char**  retvalue );

MP4V2_EXPORT
bool MP4GetTrackBytesProperty(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   propName,
    uint8_t**     ppValue,
    uint32_t*     pValueSize );

MP4V2_EXPORT
bool MP4SetTrackIntegerProperty(
    MP4FileHandle hFile,
    MP4TrackId   trackId,
    const char*  propName,
    int64_t      value );

MP4V2_EXPORT
bool MP4SetTrackFloatProperty(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   propName,
    float         value );

MP4V2_EXPORT
bool MP4SetTrackStringProperty(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    const char*   propName,
    const char*   value );

MP4V2_EXPORT
bool MP4SetTrackBytesProperty(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const char*    propName,
    const uint8_t* pValue,
    uint32_t       valueSize);

/** @} ***********************************************************************/

#endif /* MP4V2_TRACK_PROP_H */
