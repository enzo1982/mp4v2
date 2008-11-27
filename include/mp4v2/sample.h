#ifndef MP4V2_SAMPLE_H
#define MP4V2_SAMPLE_H

/**************************************************************************//**
 *
 *  @defgroup mp4_sample MP4v2 Sample
 *  @{
 *
 *****************************************************************************/

MP4V2_EXPORT
bool MP4ReadSample(
    /* input parameters */
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId,
    /* input/output parameters */
    uint8_t** ppBytes,
    uint32_t* pNumBytes,
    /* output parameters */
    MP4Timestamp* pStartTime DEFAULT(NULL),
    MP4Duration*  pDuration DEFAULT(NULL),
    MP4Duration*  pRenderingOffset DEFAULT(NULL),
    bool*         pIsSyncSample DEFAULT(NULL) );

/* uses (unedited) time to specify sample instead of sample id */
MP4V2_EXPORT
bool MP4ReadSampleFromTime(
    /* input parameters */
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  when,
    /* input/output parameters */
    uint8_t** ppBytes,
    uint32_t* pNumBytes,
    /* output parameters */
    MP4Timestamp* pStartTime DEFAULT(NULL),
    MP4Duration*  pDuration DEFAULT(NULL),
    MP4Duration*  pRenderingOffset DEFAULT(NULL),
    bool*         pIsSyncSample DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4WriteSample(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const uint8_t* pBytes,
    uint32_t       numBytes,
    MP4Duration    duration DEFAULT(MP4_INVALID_DURATION),
    MP4Duration    renderingOffset DEFAULT(0),
    bool           isSyncSample DEFAULT(true) );

MP4V2_EXPORT
bool MP4CopySample(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4SampleId   srcSampleId,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    MP4TrackId    dstTrackId DEFAULT(MP4_INVALID_TRACK_ID),
    MP4Duration   dstSampleDuration DEFAULT(MP4_INVALID_DURATION) );

MP4V2_EXPORT
bool MP4EncAndCopySample(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4SampleId   srcSampleId,
    encryptFunc_t encfcnp,
    uint32_t      encfcnparam1,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    MP4TrackId    dstTrackId DEFAULT(MP4_INVALID_TRACK_ID),
    MP4Duration   dstSampleDuration DEFAULT(MP4_INVALID_DURATION) );

/* Note this function is not yet implemented */
MP4V2_EXPORT
bool MP4ReferenceSample(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4SampleId   srcSampleId,
    MP4FileHandle dstFile,
    MP4TrackId    dstTrackId,
    MP4Duration   dstSampleDuration DEFAULT(MP4_INVALID_DURATION) );

MP4V2_EXPORT
uint32_t MP4GetSampleSize(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId);

MP4V2_EXPORT
uint32_t MP4GetTrackMaxSampleSize(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
MP4SampleId MP4GetSampleIdFromTime(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  when,
    bool          wantSyncSample DEFAULT(false) );

MP4V2_EXPORT
MP4Timestamp MP4GetSampleTime(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

MP4V2_EXPORT
MP4Duration MP4GetSampleDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

MP4V2_EXPORT
MP4Duration MP4GetSampleRenderingOffset(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

MP4V2_EXPORT
bool MP4SetSampleRenderingOffset(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId,
    MP4Duration   renderingOffset );

MP4V2_EXPORT
int8_t MP4GetSampleSync(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4SampleId   sampleId );

/* @} ***********************************************************************/
/** @} */

#endif /* MP4V2_SAMPLE_H */
