#ifndef MP4V2_TRACK_H
#define MP4V2_TRACK_H

/**************************************************************************//**
 *
 *  @defgroup mp4_track MP4v2 Track
 *  @{
 *
 *****************************************************************************/

MP4V2_EXPORT
MP4TrackId MP4AddTrack(
    MP4FileHandle hFile,
    const char*   type );

MP4V2_EXPORT
MP4TrackId MP4AddSystemsTrack(
    MP4FileHandle hFile,
    const char*   type );

MP4V2_EXPORT
MP4TrackId MP4AddODTrack(
    MP4FileHandle hFile );

MP4V2_EXPORT
MP4TrackId MP4AddSceneTrack(
    MP4FileHandle hFile );

/** Add audio track to mp4 file.
 *
 *  MP4AddAudioTrack adds an audio track to the mp4 file. MP4WriteSample()
 *  can then be used to add the desired audio samples.
 *
 *  It is recommended that the time scale be set to the sampling frequency
 *  (eg. 44100 Hz) of the audio so as to preserve the timing information
 *  accurately.
 *
 *  If the audio encoding uses a fixed duration for each sample that should
 *  be specified here. If not then the value #MP4_INVALID_SAMPLE_DURATION
 *  should be given for the sampleDuration argument.
 *
 *  @param hFile handle of file for operation.
 *  @param timeScale the time scale in ticks per second of the track.
 *  @param sampleDuration the fixed  duration for all track samples.
 *      Caveat: the value should be in track-timescale units.
 *  @param audioType the audio encoding type.
 *      See MP4GetTrackEsdsObjectTypeId() for known values.
 *
 *  @return On success, the track-id of the new track.
 *      On error, #MP4_INVALID_TRACK_ID.
 */
MP4V2_EXPORT
MP4TrackId MP4AddAudioTrack(
    MP4FileHandle hFile,
    uint32_t      timeScale,
    MP4Duration   sampleDuration,
    uint8_t       audioType DEFAULT(MP4_MPEG4_AUDIO_TYPE) );

MP4V2_EXPORT
MP4TrackId MP4AddAC3AudioTrack(
    MP4FileHandle hFile,
    uint32_t      samplingRate,
    uint8_t       fscod,
    uint8_t       bsid,
    uint8_t       bsmod,
    uint8_t       acmod,
    uint8_t       lfeon,
    uint8_t       bit_rate_code );

MP4V2_EXPORT
MP4TrackId MP4AddAmrAudioTrack(
    MP4FileHandle hFile,
    uint32_t      timeScale,
    uint16_t      modeSet,
    uint8_t       modeChangePeriod,
    uint8_t       framesPerSample,
    bool          isAmrWB );

MP4V2_EXPORT
void MP4SetAmrVendor(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint32_t      vendor );

MP4V2_EXPORT
void MP4SetAmrDecoderVersion(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint8_t       decoderVersion );

MP4V2_EXPORT
void MP4SetAmrModeSet(
    MP4FileHandle hFile,
    MP4TrackId    trakId,
    uint16_t      modeSet );

MP4V2_EXPORT
uint16_t MP4GetAmrModeSet(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
MP4TrackId MP4AddHrefTrack(
    MP4FileHandle hFile,
    uint32_t      timeScale,
    MP4Duration   sampleDuration,
    const char*   base_url DEFAULT(NULL) );

MP4V2_EXPORT
const char* MP4GetHrefTrackBaseUrl(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
MP4TrackId MP4AddVideoTrack(
    MP4FileHandle hFile,
    uint32_t      timeScale,
    MP4Duration   sampleDuration,
    uint16_t      width,
    uint16_t      height,
    uint8_t       videoType DEFAULT(MP4_MPEG4_VIDEO_TYPE) );

MP4V2_EXPORT
MP4TrackId MP4AddH264VideoTrack(
    MP4FileHandle hFile,
    uint32_t      timeScale,
    MP4Duration   sampleDuration,
    uint16_t      width,
    uint16_t      height,
    uint8_t       AVCProfileIndication,
    uint8_t       profile_compat,
    uint8_t       AVCLevelIndication,
    uint8_t       sampleLenFieldSizeMinusOne );

MP4V2_EXPORT
void MP4AddH264SequenceParameterSet(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const uint8_t* pSequence,
    uint16_t       sequenceLen );

MP4V2_EXPORT
void MP4AddH264PictureParameterSet(
    MP4FileHandle  hFile,
    MP4TrackId     trackId,
    const uint8_t* pPict,
    uint16_t       pictLen );

MP4V2_EXPORT
void MP4SetH263Vendor(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint32_t      vendor );

MP4V2_EXPORT
void MP4SetH263DecoderVersion(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint8_t       decoderVersion );

MP4V2_EXPORT
void MP4SetH263Bitrates(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint32_t      avgBitrate,
    uint32_t      maxBitrate );

MP4V2_EXPORT
MP4TrackId MP4AddH263VideoTrack(
    MP4FileHandle hFile,
    uint32_t      timeScale,
    MP4Duration   sampleDuration,
    uint16_t      width,
    uint16_t      height,
    uint8_t       h263Level,
    uint8_t       h263Profile,
    uint32_t      avgBitrate,
    uint32_t      maxBitrate );

MP4V2_EXPORT
MP4TrackId MP4AddHintTrack(
    MP4FileHandle hFile,
    MP4TrackId    refTrackId );

MP4V2_EXPORT
MP4TrackId MP4AddTextTrack(
    MP4FileHandle hFile,
    MP4TrackId    refTrackId );

MP4V2_EXPORT
MP4TrackId MP4AddSubtitleTrack(
    MP4FileHandle hFile,
    MP4TrackId    refTrackId );

MP4V2_EXPORT
MP4TrackId MP4AddPixelAspectRatio(
    MP4FileHandle hFile,
    MP4TrackId    refTrackId,
    uint32_t      hSpacing,
    uint32_t      vSpacing );

MP4V2_EXPORT
MP4TrackId MP4AddColr(
    MP4FileHandle hFile,
    MP4TrackId    refTrackId,
    uint16_t      primary,
    uint16_t      transfer,
    uint16_t      matrix );

MP4V2_EXPORT
MP4TrackId MP4CloneTrack(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    MP4TrackId    dstHintTrackReferenceTrack DEFAULT(MP4_INVALID_TRACK_ID) );

MP4V2_EXPORT
MP4TrackId MP4CopyTrack(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    bool          applyEdits DEFAULT(false),
    MP4TrackId    dstHintTrackReferenceTrack DEFAULT(MP4_INVALID_TRACK_ID) );

MP4V2_EXPORT
void MP4DeleteTrack(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
uint32_t MP4GetNumberOfTracks(
    MP4FileHandle hFile,
    const char*   type DEFAULT(NULL),
    uint8_t       subType DEFAULT(0) );

MP4V2_EXPORT
MP4TrackId MP4FindTrackId(
    MP4FileHandle hFile,
    uint16_t      index,
    const char*   type DEFAULT(NULL),
    uint8_t       subType DEFAULT(0) );

MP4V2_EXPORT
uint16_t MP4FindTrackIndex(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

/** @} ***********************************************************************/

#endif /* MP4V2_TRACK_H */
