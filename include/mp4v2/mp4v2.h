/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is MPEG4IP.
 *
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001 - 2005.  All Rights Reserved.
 *
 * 3GPP features implementation is based on 3GPP's TS26.234-v5.60,
 * and was contributed by Ximpo Group Ltd.
 *
 * Portions created by Ximpo Group Ltd. are
 * Copyright (C) Ximpo Group Ltd. 2003, 2004.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie                 dmackie@cisco.com
 *      Alix Marchandise-Franquet   alix@cisco.com
 *      Ximpo Group Ltd.            mp4v2@ximpo.com
 *      Bill May                    wmay@cisco.com
 */

#ifndef MP4V2_MP4V2_H
#define MP4V2_MP4V2_H

/*****************************************************************************/

#include <mp4v2/platform.h>
#include <mp4v2/project.h>

/* exploit C++ ability of default values for function parameters */
#ifndef DEFAULT
#   ifdef __cplusplus
#       define DEFAULT(x)  =x
#   else
#       define DEFAULT(x)
#   endif
#endif

/* MP4 API types */
typedef void*       MP4FileHandle;
typedef uint32_t    MP4TrackId;
typedef uint32_t    MP4SampleId;
typedef uint64_t    MP4Timestamp;
typedef uint64_t    MP4Duration;
typedef uint32_t    MP4EditId;

typedef uint64_t (*VIRTUALIO_GETFILELENGTH)(void *user); // return file length in bytes
typedef int (*VIRTUALIO_SETPOSITION)(void *user, uint64_t position); // return 0 on success
typedef int (*VIRTUALIO_GETPOSITION)(void *user, uint64_t *position); // fill position, return 0 on success
typedef size_t (*VIRTUALIO_READ)(void *user, void *buffer, size_t size); // return number of bytes actually read
typedef size_t (*VIRTUALIO_WRITE)(void *user, void *buffer, size_t size); // return number of bytes actually written
typedef int (*VIRTUALIO_ENDOFFILE)(void *user); // return 1 if file hit EOF
typedef int (*VIRTUALIO_CLOSE)(void *user); // return 0 on success

typedef struct Virtual_IO
{
    VIRTUALIO_GETFILELENGTH GetFileLength;
    VIRTUALIO_SETPOSITION SetPosition;
    VIRTUALIO_GETPOSITION GetPosition;
    VIRTUALIO_READ Read;
    VIRTUALIO_WRITE Write;
    VIRTUALIO_ENDOFFILE EndOfFile;
    VIRTUALIO_CLOSE Close;
} Virtual_IO_t;

/*****************************************************************************/

typedef void (*error_msg_func_t)(
    int         loglevel,
    const char* lib,
    const char* fmt,
    va_list     ap );

typedef void (*lib_message_func_t)(
    int         loglevel,
    const char* lib,
    const char* fmt,
    ... );

/*****************************************************************************/

/** @defgroup mp4_general MP4v2 General
  * @{
  */

/** Invalid MP4FileHandle constant. */
#define MP4_INVALID_FILE_HANDLE ((MP4FileHandle)NULL)
/** Invalid MP4TrackId constant. */
#define MP4_INVALID_TRACK_ID ((MP4TrackId)0)
/** Invalid MP4SampleId constant. */
#define MP4_INVALID_SAMPLE_ID ((MP4SampleId)0)
/** Invalid MP4Timestamp constant. */
#define MP4_INVALID_TIMESTAMP ((MP4Timestamp)-1)
/** Invalid MP4Duration constant. */
#define MP4_INVALID_DURATION ((MP4Duration)-1)
/** Invalid MP4EditId constant. */
#define MP4_INVALID_EDIT_ID ((MP4EditId)0)

/* MP4 verbosity levels - e.g. MP4SetVerbosity() */
#define MP4_DETAILS_ALL     0xFFFFFFFF  /**< Verbosity level all. */
#define MP4_DETAILS_ERROR   0x00000001  /**< Verbosity level error. */
#define MP4_DETAILS_WARNING 0x00000002  /**< Verbosity level warning. */
#define MP4_DETAILS_READ    0x00000004  /**< Verbosity level read. */
#define MP4_DETAILS_WRITE   0x00000008  /**< Verbosity level write. */
#define MP4_DETAILS_FIND    0x00000010  /**< Verbosity level find. */
#define MP4_DETAILS_TABLE   0x00000020  /**< Verbosity level table. */
#define MP4_DETAILS_SAMPLE  0x00000040  /**< Verbosity level sample. */
#define MP4_DETAILS_HINT    0x00000080  /**< Verbosity level hint. */
#define MP4_DETAILS_ISMA    0x00000100  /**< Verbosity level isma. */
#define MP4_DETAILS_EDIT    0x00000200  /**< Verbosity level edit. */

/** @} */

/* Macros to test for API type validity */
#define MP4_IS_VALID_FILE_HANDLE(x) ((x) != MP4_INVALID_FILE_HANDLE)
#define MP4_IS_VALID_TRACK_ID(x)    ((x) != MP4_INVALID_TRACK_ID)
#define MP4_IS_VALID_SAMPLE_ID(x)   ((x) != MP4_INVALID_SAMPLE_ID)
#define MP4_IS_VALID_TIMESTAMP(x)   ((x) != MP4_INVALID_TIMESTAMP)
#define MP4_IS_VALID_DURATION(x)    ((x) != MP4_INVALID_DURATION)
#define MP4_IS_VALID_EDIT_ID(x)     ((x) != MP4_INVALID_EDIT_ID)

#define MP4_DETAILS_READ_ALL        \
    (MP4_DETAILS_READ | MP4_DETAILS_TABLE | MP4_DETAILS_SAMPLE)
#define MP4_DETAILS_WRITE_ALL       \
    (MP4_DETAILS_WRITE | MP4_DETAILS_TABLE | MP4_DETAILS_SAMPLE)

/*
 * MP4 Known track type names - e.g. MP4GetNumberOfTracks(type)
 *
 * Note this first group of track types should be created
 * via the MP4Add<Type>Track() functions, and not MP4AddTrack(type)
 */
#define MP4_OD_TRACK_TYPE       "odsm"
#define MP4_SCENE_TRACK_TYPE    "sdsm"
#define MP4_AUDIO_TRACK_TYPE    "soun"
#define MP4_VIDEO_TRACK_TYPE    "vide"
#define MP4_HINT_TRACK_TYPE     "hint"
#define MP4_CNTL_TRACK_TYPE     "cntl"
#define MP4_TEXT_TRACK_TYPE     "text"
/*
 * This second set of track types should be created
 * via MP4AddSystemsTrack(type)
 */
#define MP4_CLOCK_TRACK_TYPE    "crsm"
#define MP4_MPEG7_TRACK_TYPE    "m7sm"
#define MP4_OCI_TRACK_TYPE      "ocsm"
#define MP4_IPMP_TRACK_TYPE     "ipsm"
#define MP4_MPEGJ_TRACK_TYPE    "mjsm"

#define MP4_IS_VIDEO_TRACK_TYPE(type) \
    (!strcasecmp(type, MP4_VIDEO_TRACK_TYPE))

#define MP4_IS_AUDIO_TRACK_TYPE(type) \
    (!strcasecmp(type, MP4_AUDIO_TRACK_TYPE))

#define MP4_IS_CNTL_TRACK_TYPE(type) \
        (!strcasecmp(type, MP4_CNTL_TRACK_TYPE))

#define MP4_IS_OD_TRACK_TYPE(type) \
    (!strcasecmp(type, MP4_OD_TRACK_TYPE))

#define MP4_IS_SCENE_TRACK_TYPE(type) \
    (!strcasecmp(type, MP4_SCENE_TRACK_TYPE))

#define MP4_IS_HINT_TRACK_TYPE(type) \
    (!strcasecmp(type, MP4_HINT_TRACK_TYPE))

#define MP4_IS_SYSTEMS_TRACK_TYPE(type) \
    (!strcasecmp(type, MP4_CLOCK_TRACK_TYPE) \
    || !strcasecmp(type, MP4_MPEG7_TRACK_TYPE) \
    || !strcasecmp(type, MP4_OCI_TRACK_TYPE) \
    || !strcasecmp(type, MP4_IPMP_TRACK_TYPE) \
    || !strcasecmp(type, MP4_MPEGJ_TRACK_TYPE))

/* MP4 Audio track types - see MP4AddAudioTrack()*/
#define MP4_INVALID_AUDIO_TYPE          0x00
#define MP4_MPEG1_AUDIO_TYPE            0x6B
#define MP4_MPEG2_AUDIO_TYPE            0x69
#define MP4_MP3_AUDIO_TYPE              MP4_MPEG2_AUDIO_TYPE
#define MP4_MPEG2_AAC_MAIN_AUDIO_TYPE   0x66
#define MP4_MPEG2_AAC_LC_AUDIO_TYPE     0x67
#define MP4_MPEG2_AAC_SSR_AUDIO_TYPE    0x68
#define MP4_MPEG2_AAC_AUDIO_TYPE        MP4_MPEG2_AAC_MAIN_AUDIO_TYPE
#define MP4_MPEG4_AUDIO_TYPE            0x40
#define MP4_PRIVATE_AUDIO_TYPE          0xC0
#define MP4_PCM16_LITTLE_ENDIAN_AUDIO_TYPE  0xE0 /* a private definition */
#define MP4_VORBIS_AUDIO_TYPE           0xE1 /* a private definition */
#define MP4_AC3_AUDIO_TYPE              0xE2 /* a private definition */
#define MP4_ALAW_AUDIO_TYPE             0xE3 /* a private definition */
#define MP4_ULAW_AUDIO_TYPE             0xE4 /* a private definition */
#define MP4_G723_AUDIO_TYPE             0xE5 /* a private definition */
#define MP4_PCM16_BIG_ENDIAN_AUDIO_TYPE 0xE6 /* a private definition */

/* MP4 MPEG-4 Audio types from 14496-3 Table 1.5.1 */
#define MP4_MPEG4_INVALID_AUDIO_TYPE        0
#define MP4_MPEG4_AAC_MAIN_AUDIO_TYPE       1
#define MP4_MPEG4_AAC_LC_AUDIO_TYPE         2
#define MP4_MPEG4_AAC_SSR_AUDIO_TYPE        3
#define MP4_MPEG4_AAC_LTP_AUDIO_TYPE        4
#define MP4_MPEG4_AAC_HE_AUDIO_TYPE         5
#define MP4_MPEG4_AAC_SCALABLE_AUDIO_TYPE   6
#define MP4_MPEG4_CELP_AUDIO_TYPE           8
#define MP4_MPEG4_HVXC_AUDIO_TYPE           9
#define MP4_MPEG4_TTSI_AUDIO_TYPE           12
#define MP4_MPEG4_MAIN_SYNTHETIC_AUDIO_TYPE 13
#define MP4_MPEG4_WAVETABLE_AUDIO_TYPE      14
#define MP4_MPEG4_MIDI_AUDIO_TYPE           15
#define MP4_MPEG4_ALGORITHMIC_FX_AUDIO_TYPE 16
#define MP4_MPEG4_ALS_AUDIO_TYPE            31
#define MP4_MPEG4_LAYER1_AUDIO_TYPE         32
#define MP4_MPEG4_LAYER2_AUDIO_TYPE         33
#define MP4_MPEG4_LAYER3_AUDIO_TYPE         34
#define MP4_MPEG4_SLS_AUDIO_TYPE            35

/* MP4 Audio type utilities following common usage */
#define MP4_IS_MP3_AUDIO_TYPE(type) \
    ((type) == MP4_MPEG1_AUDIO_TYPE || (type) == MP4_MPEG2_AUDIO_TYPE)

#define MP4_IS_MPEG2_AAC_AUDIO_TYPE(type) \
    (((type) >= MP4_MPEG2_AAC_MAIN_AUDIO_TYPE \
        && (type) <= MP4_MPEG2_AAC_SSR_AUDIO_TYPE))

#define MP4_IS_MPEG4_AAC_AUDIO_TYPE(mpeg4Type) \
    (((mpeg4Type) >= MP4_MPEG4_AAC_MAIN_AUDIO_TYPE \
        && (mpeg4Type) <= MP4_MPEG4_AAC_HE_AUDIO_TYPE) \
      || (mpeg4Type) == MP4_MPEG4_AAC_SCALABLE_AUDIO_TYPE \
          || (mpeg4Type) == 17)

#define MP4_IS_AAC_AUDIO_TYPE(type) \
    (MP4_IS_MPEG2_AAC_AUDIO_TYPE(type) \
    || (type) == MP4_MPEG4_AUDIO_TYPE)

/* MP4 Video track types - see MP4AddVideoTrack() */
#define MP4_INVALID_VIDEO_TYPE          0x00
#define MP4_MPEG1_VIDEO_TYPE            0x6A
#define MP4_MPEG2_SIMPLE_VIDEO_TYPE     0x60
#define MP4_MPEG2_MAIN_VIDEO_TYPE       0x61
#define MP4_MPEG2_SNR_VIDEO_TYPE        0x62
#define MP4_MPEG2_SPATIAL_VIDEO_TYPE    0x63
#define MP4_MPEG2_HIGH_VIDEO_TYPE       0x64
#define MP4_MPEG2_442_VIDEO_TYPE        0x65
#define MP4_MPEG2_VIDEO_TYPE            MP4_MPEG2_MAIN_VIDEO_TYPE
#define MP4_MPEG4_VIDEO_TYPE            0x20
#define MP4_JPEG_VIDEO_TYPE             0x6C
#define MP4_PRIVATE_VIDEO_TYPE          0xD0
#define MP4_YUV12_VIDEO_TYPE            0xF0    /* a private definition */
#define MP4_H263_VIDEO_TYPE             0xF2    /* a private definition */
#define MP4_H261_VIDEO_TYPE             0xF3    /* a private definition */

/* MP4 Video type utilities */
#define MP4_IS_MPEG1_VIDEO_TYPE(type) \
    ((type) == MP4_MPEG1_VIDEO_TYPE)

#define MP4_IS_MPEG2_VIDEO_TYPE(type) \
    (((type) >= MP4_MPEG2_SIMPLE_VIDEO_TYPE \
        && (type) <= MP4_MPEG2_442_VIDEO_TYPE) \
      || MP4_IS_MPEG1_VIDEO_TYPE(type))

#define MP4_IS_MPEG4_VIDEO_TYPE(type) \
    ((type) == MP4_MPEG4_VIDEO_TYPE)

/* Mpeg4 Visual Profile Defines - ISO/IEC 14496-2:2001/Amd.2:2002(E) */
#define MPEG4_SP_L1 (0x1)
#define MPEG4_SP_L2 (0x2)
#define MPEG4_SP_L3 (0x3)
#define MPEG4_SP_L0 (0x8)
#define MPEG4_SSP_L1 (0x11)
#define MPEG4_SSP_L2 (0x12)
#define MPEG4_CP_L1 (0x21)
#define MPEG4_CP_L2 (0x22)
#define MPEG4_MP_L2 (0x32)
#define MPEG4_MP_L3 (0x33)
#define MPEG4_MP_L4 (0x34)
#define MPEG4_NBP_L2 (0x42)
#define MPEG4_STP_L1 (0x51)
#define MPEG4_SFAP_L1 (0x61)
#define MPEG4_SFAP_L2 (0x62)
#define MPEG4_SFBAP_L1 (0x63)
#define MPEG4_SFBAP_L2 (0x64)
#define MPEG4_BATP_L1 (0x71)
#define MPEG4_BATP_L2 (0x72)
#define MPEG4_HP_L1 (0x81)
#define MPEG4_HP_L2 (0x82)
#define MPEG4_ARTSP_L1 (0x91)
#define MPEG4_ARTSP_L2 (0x92)
#define MPEG4_ARTSP_L3 (0x93)
#define MPEG4_ARTSP_L4 (0x94)
#define MPEG4_CSP_L1 (0xa1)
#define MPEG4_CSP_L2 (0xa2)
#define MPEG4_CSP_L3 (0xa3)
#define MPEG4_ACEP_L1 (0xb1)
#define MPEG4_ACEP_L2 (0xb2)
#define MPEG4_ACEP_L3 (0xb3)
#define MPEG4_ACEP_L4 (0xb4)
#define MPEG4_ACP_L1 (0xc1)
#define MPEG4_ACP_L2 (0xc2)
#define MPEG4_AST_L1 (0xd1)
#define MPEG4_AST_L2 (0xd2)
#define MPEG4_AST_L3 (0xd3)
#define MPEG4_S_STUDIO_P_L1 (0xe1)
#define MPEG4_S_STUDIO_P_L2 (0xe2)
#define MPEG4_S_STUDIO_P_L3 (0xe3)
#define MPEG4_S_STUDIO_P_L4 (0xe4)
#define MPEG4_C_STUDIO_P_L1 (0xe5)
#define MPEG4_C_STUDIO_P_L2 (0xe6)
#define MPEG4_C_STUDIO_P_L3 (0xe7)
#define MPEG4_C_STUDIO_P_L4 (0xe8)
#define MPEG4_ASP_L0 (0xF0)
#define MPEG4_ASP_L1 (0xF1)
#define MPEG4_ASP_L2 (0xF2)
#define MPEG4_ASP_L3 (0xF3)
#define MPEG4_ASP_L4 (0xF4)
#define MPEG4_ASP_L5 (0xF5)
#define MPEG4_ASP_L3B (0xF7)
#define MPEG4_FGSP_L0 (0xf8)
#define MPEG4_FGSP_L1 (0xf9)
#define MPEG4_FGSP_L2 (0xfa)
#define MPEG4_FGSP_L3 (0xfb)
#define MPEG4_FGSP_L4 (0xfc)
#define MPEG4_FGSP_L5 (0xfd)

/*! The maximum length of a QuickTime chapter title (in 8-bit chars)
 */
#define MP4V2_CHAPTER_TITLE_MAX 1023

/** Chapter item.
 *  This item defines various attributes for a chapter.
 *  @ingroup mp4_chapter
 */
typedef struct MP4Chapter_s {
    MP4Duration duration; /**< duration of chapter in milliseconds */
    char title[MP4V2_CHAPTER_TITLE_MAX+1]; /**< title of chapter */
} MP4Chapter_t;

/** Known chapter types.
 *  @ingroup mp4_chapter
 */
typedef enum {
    MP4ChapterTypeNone = 0, /**< no chapters found return value */
    MP4ChapterTypeAny  = 1, /**< any or all known chapter types */
    MP4ChapterTypeQt   = 2, /**< QuickTime chapter type */
    MP4ChapterTypeNero = 4  /**< Nero chapter type */
} MP4ChapterType;

/*
 * MP4 API declarations
 * All symbols must be marked for export otherwise they will be hidden
 */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

/** @defgroup mp4_fileops MP4v2 File I/O
  * @{
  */

/** Bit used with MP4Create() to enable 64-bit data-atoms. */
#define MP4_CREATE_64BIT_DATA 0x01
/** Bit used with MP4Create() to enable 64-bit time-atoms.
  * @warning Quicktime-incompatible
  */
#define MP4_CREATE_64BIT_TIME 0x02
/** Convenience bitmask for both data and time atoms.
  * @deprecated scheduled for removal
  */
#define MP4_CREATE_64BIT (MP4_CREATE_64BIT_DATA | MP4_CREATE_64BIT_TIME)
/** Not used.
  * @deprecated scheduled for removal
  */
#define MP4_CREATE_EXTENSIBLE_FORMAT 0x04

/** Close an mp4 file.
 *  MP4Close closes a previously opened mp4 file. If the file was opened
 *  writable with MP4Create() or MP4Modify(), then MP4Close() will write
 *  out all pending information to disk.
 *
 *  @param hFile handle of file to close.
 */
MP4V2_EXPORT
void MP4Close(
    MP4FileHandle hFile );

/** Create a new mp4 file.
 *
 *  MP4Create is the first call that should be used when you want to create
 *  a new, empty mp4 file. It is equivalent to opening a file for writing,
 *  but also involved with creation of necessary mp4 framework structures.
 *  ie. invoking MP4Create() followed by MP4Close() will result in a file
 *  with a non-zero size.
 *
 *  @param fileName pathname of the file to be created.
 *  @param verbosity bitmask of diagnostic details the library
 *      should print to stdout during its functioning.
 *      See MP4SetVerbosity() for values.
 *  @param flags bitmask that allows the user to set 64-bit values for
 *      data or time atoms. Valid bits may be any combination of:
 *          @li #MP4_CREATE_64BIT_DATA
 *          @li #MP4_CREATE_64BIT_TIME
 *
 *  @return On success a handle of the newly created file for use in
 *      subsequent calls to the library.
 *      On error, #MP4_INVALID_FILE_HANDLE is returned.
 */
MP4V2_EXPORT
MP4FileHandle MP4Create(
    const char* fileName,
    uint32_t    verbosity DEFAULT(0),
    uint32_t    flags DEFAULT(0) );

/** Create a new mp4 file with extended options.
 *
 *  MP4CreateEx is an extended version of MP4Create().
 *
 *  @param fileName pathname of the file to be created.
 *  @param verbosity bitmask of diagnostic details the library
 *      should print to stdout during its functioning.
 *      See MP4SetVerbosity() for values.
 *  @param flags bitmask that allows the user to set 64-bit values for
 *      data or time atoms. Valid bits may be any combination of:
 *          @li #MP4_CREATE_64BIT_DATA
 *          @li #MP4_CREATE_64BIT_TIME
 *  @param add_ftyp if true an <b>ftyp</b> atom is automatically created.
 *  @param add_iods if true an <b>iods</b> atom is automatically created.
 *  @param majorBrand <b>ftyp</b> brand identifier.
 *  @param minorVersion <b>ftyp</b> informative integer for the minor version
 *      of the major brand.
 *  @param compatibleBrands <b>ftyp</b> list of compatible brands.
 *  @param compatibleBrandsCount is the count of items specified in
 *      compatibleBrands.
 *
 *  @return On success a handle of the newly created file for use in
 *      subsequent calls to the library.
 *      On error, #MP4_INVALID_FILE_HANDLE is returned.
 */
MP4V2_EXPORT
MP4FileHandle MP4CreateEx(
    const char* fileName,
    uint32_t    verbosity DEFAULT(0),
    uint32_t    flags DEFAULT(0),
    int         add_ftyp DEFAULT(1),
    int         add_iods DEFAULT(1),
    char*       majorBrand DEFAULT(0),
    uint32_t    minorVersion DEFAULT(0),
    char**      compatibleBrands DEFAULT(0),
    uint32_t    compatibleBrandsCount DEFAULT(0) );

/** Dump mp4 file contents as ASCII.
 *
 *  Dump is an invaluable debugging tool in that in can reveal all the details
 *  of the mp4 control structures. However, the output will not make much sense
 *  until you familiarize yourself with the mp4 specification (or the Quicktime
 *  File Format specification).
 *
 *  Note that MP4Dump() will not print the individual values of control tables,
 *  such as the size of each sample, unless the current verbosity value
 *  includes the flag #MP4_DETAILS_TABLE. See MP4SetVerbosity() for how to set
 *  this flag.
 *
 *  @param hFile handle of file to dump.
 *  @param pDumpFile dump destination. If NULL stdout will be used.
 *  @param dumpImplicits prints properties which would not actually be
 *      written to the mp4 file, but still exist in mp4 control structures.
 *      ie. they are implicit given the current values of other controlling
 *      properties.
 *
 *  @return <b>true</b> on success, <b>false</b> on failure.
 */
MP4V2_EXPORT
bool MP4Dump(
    MP4FileHandle hFile,
    FILE*         pDumpFile DEFAULT(NULL),
    bool          dumpImplicits DEFAULT(0) );

/** Return a textual summary of an mp4 file.
 *
 *  MP4FileInfo provides a string that contains a textual summary of the
 *  contents of an mp4 file. This includes the track id's, the track type,
 *  and track specific information. For example, for a video track, media
 *  encoding, image size, frame rate, and bitrate are summarized.
 *
 *  Note that the returned string is malloc'ed, so it is the caller's
 *  responsibility to free() the string. Also note that the returned string
 *  contains newlines and tabs which may or may not be desirable. 
 *
 *  The following is an example of the output of MP4Info():
@verbatim
Track  Type   Info
1      video  MPEG−4 Simple @ L3, 119.625 secs, 1008 kbps, 352x288 @ 24.00 fps
2      audio  MPEG−4, 119.327 secs, 128 kbps, 44100 Hz
3      hint   Payload MP4V−ES for track 1
4      hint   Payload mpeg4−generic for track 2
5      od     Object Descriptors
6      scene  BIFS
@endverbatim
 *
 *  @param fileName pathname to mp4 file to summarize.
 *  @param trackId specifies track to summarize. If the value is
 *      #MP4_INVALID_TRACK_ID, the summary info is created for all
 *      tracks in the file.
 *
 *  @return On success a malloc'd string containing summary information.
 *      On failre, NULL.
 */
MP4V2_EXPORT
char* MP4FileInfo(
    const char* fileName,
    MP4TrackId  trackId DEFAULT(MP4_INVALID_TRACK_ID) );

/** Return a textual summary of an mp4 file.
 *
 *  MP4FileInfo provides a string that contains a textual summary of the
 *  contents of an mp4 file. This includes the track id's, the track type,
 *  and track specific information. For example, for a video track, media
 *  encoding, image size, frame rate, and bitrate are summarized.
 *
 *  Note that the returned string is malloc'ed, so it is the caller's
 *  responsibility to free() the string. Also note that the returned string
 *  contains newlines and tabs which may or may not be desirable. 
 *
 *  The following is an example of the output of MP4Info():
@verbatim
Track  Type   Info
1      video  MPEG−4 Simple @ L3, 119.625 secs, 1008 kbps, 352x288 @ 24.00 fps
2      audio  MPEG−4, 119.327 secs, 128 kbps, 44100 Hz
3      hint   Payload MP4V−ES for track 1
4      hint   Payload mpeg4−generic for track 2
5      od     Object Descriptors
6      scene  BIFS
@endverbatim
 *
 *  @param hFile handle of file to summarize.
 *  @param trackId specifies track to summarize. If the value is
 *      #MP4_INVALID_TRACK_ID, the summary info is created for all
 *      tracks in the file.
 *
 *  @return On success a malloc'd string containing summary information.
 *      On failre, NULL.
 */
MP4V2_EXPORT
char* MP4Info(
    MP4FileHandle hFile,
    MP4TrackId    trackId DEFAULT(MP4_INVALID_TRACK_ID) );

/** Modify an existing mp4 file.
 *
 *  MP4Modify is the first call that should be used when you want to modify
 *  an existing mp4 file. It is roughly equivalent to opening a file in
 *  read/write mode.
 *
 *  Since modifications to an existing mp4 file can result in a sub−optimal
 *  file layout, you may want to use MP4Optimize() after you have  modified
 *  and closed the mp4 file.
 *
 *  @param fileName pathname of the file to be modified.
 *  @param verbosity bitmask of diagnostic details the library
 *      should print to stdout during its functioning.
 *      See MP4SetVerbosity() for values.
 *  @param flags currently ignored.
 *
 *  @return On success a handle of the target file for use in subsequent calls
 *      to the library.
 *      On error, #MP4_INVALID_FILE_HANDLE is returned.
 */
MP4V2_EXPORT
MP4FileHandle MP4Modify(
    const char* fileName,
    uint32_t    verbosity DEFAULT(0),
    uint32_t    flags DEFAULT(0) );

/** Optimize the layour of an mp4 file.
 *
 *  MP4Optimize reads an existing mp4 file and writes a new version of the
 *  file with the two important changes:
 *
 *  First, the mp4 control information is moved to the beginning of the file.
 *  (Frequenty it is at the end of the file due to it being constantly
 *  modified as track samples are added to an mp4 file). This optimization
 *  is useful in that in allows the mp4 file to be HTTP streamed.
 *
 *  Second, the track samples are interleaved so that the samples for a
 *  particular instant in time are colocated within the file. This
 *  eliminates disk seeks during playback of the file which results in
 *  better performance.
 *
 *  There are also two important side effects of MP4Optimize():
 *
 *  First, any free blocks within the mp4 file are eliminated.
 *
 *  Second, as a side effect of the sample interleaving process any media
 *  data chunks that are not actually referenced by the mp4 control
 *  structures are deleted. This is useful if you have called MP4DeleteTrack()
 *  which only deletes the control information for a track, and not the
 *  actual media data.
 *
 *  @param fileName pathname of (existing) file to be optimized.
 *  @param newFileName pathname of the new optimized file.
 *      If NULL a temporary file will be used and <b>fileName</b>
 *      will be over-written upon successful completion.
 *  @param verbosity bitmask of diagnostic details the library
 *      should print to stdout during its functioning.
 *      See MP4SetVerbosity() for values.
 *
 *  @return <b>true</b> on success, <b>false</b> on failure.
 */
MP4V2_EXPORT
bool MP4Optimize(
    const char* fileName,
    const char* newFileName DEFAULT(NULL),
    uint32_t    verbosity DEFAULT(0) );

/** Read an existing mp4 file.
 *
 *  MP4Read is the first call that should be used when you want to just
 *  read an existing mp4 file. It is equivalent to opening a file for
 *  reading, but in addition the mp4 file is parsed and the control
 *  information is loaded into memory. Note that actual track samples are not
 *  read into memory until MP4ReadSample() is called.
 *
 *  @param fileName pathname of the file to be read.
 *  @param verbosity bitmask of diagnostic details the library
 *      should print to stdout during its functioning.
 *      See MP4SetVerbosity() for values.
 *
 *  @return On success a handle of the file for use in subsequent calls to
 *      the library.
 *      On error, #MP4_INVALID_FILE_HANDLE is returned.
 */
MP4V2_EXPORT
MP4FileHandle MP4Read(
    const char* fileName,
    uint32_t    verbosity DEFAULT(0) );

/** Read an existing mp4 file with extended options.
 *
 *  MP4ReadEx is an extended version of MP4Read() which is used when
 *  custom I/O routines are to be used with an open file.
 *
 *  @param fileName pathname of the file to be read.
 *  @param user undocumented.
 *  @param virtual_IO undocumented.
 *  @param verbosity bitmask of diagnostic details the library
 *      should print to stdout during its functioning.
 *      See MP4SetVerbosity() for values.
 *
 *  @return On success a handle of the file for use in subsequent calls to
 *      the library.
 *      On error, #MP4_INVALID_FILE_HANDLE is returned.
 */
MP4V2_EXPORT
MP4FileHandle MP4ReadEx(
    const char*   fileName,
    void*         user,
    Virtual_IO_t* virtual_IO,
    uint32_t      verbosity DEFAULT(0) );

/** @} */

/** @defgroup mp4_chapter MP4v2 Chapter Support
 *  @{
 */

/** Add a QuickTime chapter.
 *
 *  This function adds a QuickTime chapter to file <b>hFile</b>.
 *
 *  @param hFile handle of file to add chapter.
 *  @param chapterTrackId ID of chapter track or #MP4_INVALID_TRACK_ID
 *      if unknown.
 *  @param chapterDuration duration (in the timescale of the chapter track).
 *  @param chapterTitle title text for the chapter or NULL to use default
 *      title format ("Chapter %03d", n) where n is the chapter number.
 */
MP4V2_EXPORT
void MP4AddChapter(
    MP4FileHandle hFile,
    MP4TrackId    chapterTrackId,
    MP4Duration   chapterDuration,
    const char*   chapterTitle DEFAULT(0));

/** Add a QuickTime chapter track.
 *
 *  This function adds a chapter (text) track to file <b>hFile</b>.
 *  The optional parameter <b>timescale</b> may be supplied to give the new
 *  chapter a specific timescale. Otherwise the chapter track will have
 *  the same timescale as the reference track defined in parameter refTrackId.
 *
 *  @param hFile handle of file to add chapter track.
 *  @param refTrackId ID of the track that will reference the chapter track.
 *  @param timescale the timescale of the chapter track or 0 to use the
 *      timescale of track specified by <b>refTrackId</b>.
 *
 *  @return ID of the created chapter track.
 */
MP4V2_EXPORT
MP4TrackId MP4AddChapterTextTrack(
    MP4FileHandle hFile,
    MP4TrackId    refTrackId,
    uint32_t      timescale DEFAULT(0) );

/** Add a Nero chapter.
 *
 *  This function adds a Nero chapter to file <b>hFile</b>.
 *
 *  @param hFile handle of file to add chapter.
 *  @param chapterStart the start time of the chapter in 100 nanosecond units
 *  @param chapterTitle title text for the chapter or NULL to use default
 *      title format ("Chapter %03d", n) where n is the chapter number.
 */
MP4V2_EXPORT
void MP4AddNeroChapter(
    MP4FileHandle hFile,
    MP4Timestamp  chapterStart,
    const char*   chapterTitle DEFAULT(0));

/** Convert chapters to another type.
 *
 *  This function converts existing chapters in file <b>hFile</b>
 *  from one type to another type.
 *  Conversion from Nero to QuickTime or QuickTime to Nero is supported.
 *
 *  @param hFile handle of file to convert.
 *  @param toChapterType the chapter type to convert to:
 *      @li #MP4ChapterTypeQt (convert from Nero to Qt)
 *      @li #MP4ChapterTypeNero (convert from Qt to Nero)
 *
 *  @return the chapter type before conversion or #MP4ChapterTypeNone
 *      if the source chapters do not exist
 *      or invalid <b>toChapterType</b> was specified.
 */
MP4V2_EXPORT
MP4ChapterType MP4ConvertChapters(
    MP4FileHandle  hFile,
    MP4ChapterType toChapterType DEFAULT(MP4ChapterTypeQt));

/** Delete chapters.
 *
 *  This function deletes existing chapters in file <b>hFile</b>.
 *
 *  @param hFile handle of file to delete chapters.
 *  @param chapterType the type of chapters to delete:
 *      @li #MP4ChapterTypeAny (delete all known chapter types)
 *      @li #MP4ChapterTypeQt
 *      @li #MP4ChapterTypeNero
 *  @param chapterTrackId ID of the chapter track if known,
 *      or #MP4_INVALID_TRACK_ID.
 *      Only applies when <b>chapterType</b>=#MP4ChapterTypeQt.
 *
 *  @return the type of deleted chapters
 */
MP4V2_EXPORT
MP4ChapterType MP4DeleteChapters(
    MP4FileHandle  hFile,
    MP4ChapterType chapterType DEFAULT(MP4ChapterTypeQt),
    MP4TrackId     chapterTrackId DEFAULT(MP4_INVALID_TRACK_ID) );

/** Get list of chapters.
 *
 *  This function gets a chpter list from file <b>hFile</b>.
 *
 *  @param hFile handle of file to read.
 *  @param chapterList address receiving array of chapter items.
 *      If a non-NULL is received the caller is responsible for freeing the
 *      memory with MP4Free().
 *  @param chapterCount address receiving count of items in array.
 *  @param chapterType the type of chapters to read:
 *      @li #MP4ChapterTypeAny (any chapters, searched in order of Qt, Nero)
 *      @li #MP4ChapterTypeQt
 *      @li #MP4ChapterTypeNero
 *
 *  @result the first type of chapters found.
 */
MP4V2_EXPORT
MP4ChapterType MP4GetChapters(
    MP4FileHandle  hFile,
    MP4Chapter_t** chapterList,
    uint32_t*      chapterCount,
    MP4ChapterType chapterType DEFAULT(MP4ChapterTypeQt));

/** Set list of chapters.
 *
 *  This functions sets the complete chapter list in file <b>hFile</b>.
 *  If any chapters of the same type already exist they will first
 *  be deleted.
 *
 *  @param hFile handle of file to modify.
 *  @param chapterList array of chapters items.
 *  @param chapterCount count of items in array.
 *  @param chapterType type of chapters to write:
 *      @li #MP4ChapterTypeAny (chapters of all types are written)
 *      @li #MP4ChapterTypeQt
 *      @li #MP4ChapterTypeNero
 *
 *  @return the type of chapters written.
 */
MP4V2_EXPORT
MP4ChapterType MP4SetChapters(
    MP4FileHandle hFile,
    MP4Chapter_t* chapterList,
    uint32_t       chapterCount,
    MP4ChapterType chapterType DEFAULT(MP4ChapterTypeQt));

/** @} */

/* file properties */

/* specific file properties */

MP4V2_EXPORT
uint32_t MP4GetVerbosity( MP4FileHandle hFile );

MP4V2_EXPORT
void MP4SetVerbosity( MP4FileHandle hFile, uint32_t verbosity );

MP4V2_EXPORT
MP4Duration MP4GetDuration( MP4FileHandle hFile );

MP4V2_EXPORT
uint32_t MP4GetTimeScale( MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetTimeScale( MP4FileHandle hFile, uint32_t value );

/** Change the general timescale of file hFile.
 *
 *  This function changes the general timescale of the file <b>hFile</b>
 *  to the new timescale <b>value</b> by recalculating all values that depend
 *  on the timescale in "moov.mvhd".
 *
 *  If the timescale is already equal to value nothing is done.
 *
 *  @param hFile handle of file to change.
 *  @param value the new timescale.
 *
 *  @ingroup mp4_general
 */
MP4V2_EXPORT
void MP4ChangeMovieTimeScale( MP4FileHandle hFile, uint32_t value );

MP4V2_EXPORT
uint8_t MP4GetODProfileLevel( MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetODProfileLevel( MP4FileHandle hFile, uint8_t value );

MP4V2_EXPORT
uint8_t MP4GetSceneProfileLevel( MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetSceneProfileLevel( MP4FileHandle hFile, uint8_t value );

MP4V2_EXPORT
uint8_t MP4GetVideoProfileLevel(
    MP4FileHandle hFile,
    MP4TrackId    trackId DEFAULT(MP4_INVALID_TRACK_ID) );

MP4V2_EXPORT
void MP4SetVideoProfileLevel( MP4FileHandle hFile, uint8_t value );

MP4V2_EXPORT
uint8_t MP4GetAudioProfileLevel( MP4FileHandle hFile );

MP4V2_EXPORT
void MP4SetAudioProfileLevel( MP4FileHandle hFile, uint8_t value );

MP4V2_EXPORT
uint8_t MP4GetGraphicsProfileLevel( MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetGraphicsProfileLevel( MP4FileHandle hFile, uint8_t value );

/* generic file properties */

MP4V2_EXPORT
bool MP4HaveAtom(
    MP4FileHandle hFile,
    const char*   atomName );

MP4V2_EXPORT
bool MP4GetIntegerProperty(
    MP4FileHandle hFile,
    const char*   propName,
    uint64_t*     retval );

MP4V2_EXPORT
bool MP4GetFloatProperty(
    MP4FileHandle hFile,
    const char*   propName,
    float*        retvalue );

MP4V2_EXPORT
bool MP4GetStringProperty(
    MP4FileHandle hFile,
    const char*   propName,
    const char**  retvalue );

MP4V2_EXPORT
bool MP4GetBytesProperty(
    MP4FileHandle hFile,
    const char*   propName,
    uint8_t**     ppValue,
    uint32_t*     pValueSize );

MP4V2_EXPORT
bool MP4SetIntegerProperty(
    MP4FileHandle hFile,
    const char*   propName,
    int64_t       value );

MP4V2_EXPORT
bool MP4SetFloatProperty(
    MP4FileHandle hFile,
    const char*   propName,
    float         value );

MP4V2_EXPORT
bool MP4SetStringProperty(
    MP4FileHandle hFile,
    const char*   propName,
    const char*   value );

MP4V2_EXPORT
bool MP4SetBytesProperty(
    MP4FileHandle  hFile,
    const char*    propName,
    const uint8_t* pValue,
    uint32_t       valueSize );

/* track operations */

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

typedef struct mp4v2_ismacryp_session_params {
    uint32_t    scheme_type;
    uint16_t    scheme_version;
    uint8_t     key_ind_len;
    uint8_t     iv_len;
    uint8_t     selective_enc;
    const char* kms_uri;
} mp4v2_ismacrypParams;

/*
 * API to initialize ismacryp properties to sensible defaults
 * if input param is null then mallocs a params struct
 */

MP4V2_EXPORT
mp4v2_ismacrypParams* MP4DefaultISMACrypParams( mp4v2_ismacrypParams* ptr );

MP4V2_EXPORT
MP4TrackId MP4AddEncAudioTrack(
    MP4FileHandle         hFile,
    uint32_t              timeScale,
    MP4Duration           sampleDuration,
    mp4v2_ismacrypParams* icPp,
    uint8_t               audioType DEFAULT(MP4_MPEG4_AUDIO_TYPE) );

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
MP4TrackId MP4AddEncVideoTrack(
    MP4FileHandle         hFile,
    uint32_t              timeScale,
    MP4Duration           sampleDuration,
    uint16_t              width,
    uint16_t              height,
    mp4v2_ismacrypParams* icPp,
    uint8_t               videoType DEFAULT(MP4_MPEG4_VIDEO_TYPE),
    const char*           oFormat DEFAULT(NULL) );

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
MP4TrackId MP4AddEncH264VideoTrack(
    MP4FileHandle         dstFile,
    uint32_t              timeScale,
    MP4Duration           sampleDuration,
    uint16_t              width,
    uint16_t              height,
    MP4FileHandle         srcFile,
    MP4TrackId            srcTrackId,
    mp4v2_ismacrypParams* icPp );

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
MP4TrackId MP4EncAndCloneTrack(
    MP4FileHandle         srcFile,
    MP4TrackId            srcTrackId,
    mp4v2_ismacrypParams* icPp,
    MP4FileHandle         dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    MP4TrackId            dstHintTrackReferenceTrack DEFAULT(MP4_INVALID_TRACK_ID) );

MP4V2_EXPORT
MP4TrackId MP4CopyTrack(
    MP4FileHandle srcFile,
    MP4TrackId    srcTrackId,
    MP4FileHandle dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    bool          applyEdits DEFAULT(false),
    MP4TrackId    dstHintTrackReferenceTrack DEFAULT(MP4_INVALID_TRACK_ID) );

typedef uint32_t (*encryptFunc_t)( uint32_t, uint32_t, uint8_t*, uint32_t*, uint8_t** );

MP4V2_EXPORT
MP4TrackId MP4EncAndCopyTrack(
    MP4FileHandle         srcFile,
    MP4TrackId            srcTrackId,
    mp4v2_ismacrypParams* icPp,
    encryptFunc_t         encfcnp,
    uint32_t              encfcnparam1,
    MP4FileHandle         dstFile DEFAULT(MP4_INVALID_FILE_HANDLE),
    bool                  applyEdits DEFAULT(false),
    MP4TrackId            dstHintTrackReferenceTrack DEFAULT(MP4_INVALID_TRACK_ID) );

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

/* track properties */

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

/* sample operations */

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

/* rtp hint track operations */

MP4V2_EXPORT
bool MP4GetHintTrackRtpPayload(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    char**        ppPayloadName DEFAULT(NULL),
    uint8_t*      pPayloadNumber DEFAULT(NULL),
    uint16_t*     pMaxPayloadSize DEFAULT(NULL),
    char**        ppEncodingParams DEFAULT(NULL) );

#define MP4_SET_DYNAMIC_PAYLOAD 0xff

MP4V2_EXPORT
bool MP4SetHintTrackRtpPayload(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    const char*   pPayloadName,
    uint8_t*      pPayloadNumber,
    uint16_t      maxPayloadSize DEFAULT(0),
    const char *  encode_params DEFAULT(NULL),
    bool          include_rtp_map DEFAULT(true),
    bool          include_mpeg4_esid DEFAULT(true) );

MP4V2_EXPORT
const char* MP4GetSessionSdp(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetSessionSdp(
    MP4FileHandle hFile,
    const char*   sdpString );

MP4V2_EXPORT
bool MP4AppendSessionSdp(
    MP4FileHandle hFile,
    const char*   sdpString );

MP4V2_EXPORT
const char* MP4GetHintTrackSdp(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId );

MP4V2_EXPORT
bool MP4SetHintTrackSdp(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    const char*   sdpString );

MP4V2_EXPORT
bool MP4AppendHintTrackSdp(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    const char*   sdpString );

MP4V2_EXPORT
MP4TrackId MP4GetHintTrackReferenceTrackId(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId );

MP4V2_EXPORT
bool MP4ReadRtpHint(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    MP4SampleId   hintSampleId,
    uint16_t*     pNumPackets DEFAULT(NULL) );

MP4V2_EXPORT
uint16_t MP4GetRtpHintNumberOfPackets(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId );

MP4V2_EXPORT
int8_t MP4GetRtpPacketBFrame(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    uint16_t      packetIndex );

MP4V2_EXPORT
int32_t MP4GetRtpPacketTransmitOffset(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    uint16_t      packetIndex );

MP4V2_EXPORT
bool MP4ReadRtpPacket(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    uint16_t      packetIndex,
    uint8_t**     ppBytes,
    uint32_t*     pNumBytes,
    uint32_t      ssrc DEFAULT(0),
    bool          includeHeader DEFAULT(true),
    bool          includePayload DEFAULT(true) );

MP4V2_EXPORT
MP4Timestamp MP4GetRtpTimestampStart(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId );

MP4V2_EXPORT
bool MP4SetRtpTimestampStart(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    MP4Timestamp  rtpStart );

MP4V2_EXPORT
bool MP4AddRtpHint(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId );

MP4V2_EXPORT
bool MP4AddRtpVideoHint(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    bool          isBframe DEFAULT(false),
    uint32_t      timestampOffset DEFAULT(0) );

MP4V2_EXPORT
bool MP4AddRtpPacket(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    bool          setMbit DEFAULT(false),
    int32_t       transmitOffset DEFAULT(0) );

MP4V2_EXPORT
bool MP4AddRtpImmediateData(
    MP4FileHandle  hFile,
    MP4TrackId     hintTrackId,
    const uint8_t* pBytes,
    uint32_t       numBytes );

MP4V2_EXPORT
bool MP4AddRtpSampleData(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    MP4SampleId   sampleId,
    uint32_t      dataOffset,
    uint32_t      dataLength );

MP4V2_EXPORT
bool MP4AddRtpESConfigurationPacket(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId );

MP4V2_EXPORT
bool MP4WriteRtpHint(
    MP4FileHandle hFile,
    MP4TrackId    hintTrackId,
    MP4Duration   duration,
    bool          isSyncSample DEFAULT(true) );

/* 3GP specific utilities */

MP4V2_EXPORT
bool MP4Make3GPCompliant(
    const char* fileName,
    uint32_t    verbosity DEFAULT(0),
    char*       majorBrand DEFAULT(0),
    uint32_t    minorVersion DEFAULT(0),
    char**      supportedBrands DEFAULT(NULL),
    uint32_t    supportedBrandsCount DEFAULT(0),
    bool        deleteIodsAtom DEFAULT(true) );

/* ISMA specific utilities */

MP4V2_EXPORT
bool MP4MakeIsmaCompliant(
    const char* fileName,
    uint32_t    verbosity DEFAULT(0),
    bool        addIsmaComplianceSdp DEFAULT(true) );

MP4V2_EXPORT
char* MP4MakeIsmaSdpIod(
    uint8_t  videoProfile,
    uint32_t videoBitrate,
    uint8_t* videoConfig,
    uint32_t videoConfigLength,
    uint8_t  audioProfile,
    uint32_t audioBitrate,
    uint8_t* audioConfig,
    uint32_t audioConfigLength,
    uint32_t verbosity DEFAULT(0) );

/* edit list */

/* NOTE this section of functionality has not yet been fully tested */

MP4V2_EXPORT
MP4EditId MP4AddTrackEdit(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId DEFAULT(MP4_INVALID_EDIT_ID),
    MP4Timestamp  startTime DEFAULT(0),
    MP4Duration   duration DEFAULT(0),
    bool          dwell DEFAULT(false) );

MP4V2_EXPORT
bool MP4DeleteTrackEdit(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId );

MP4V2_EXPORT
uint32_t MP4GetTrackNumberOfEdits(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

MP4V2_EXPORT
MP4Timestamp MP4GetTrackEditStart(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId );

MP4V2_EXPORT
MP4Duration MP4GetTrackEditTotalDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId DEFAULT(MP4_INVALID_EDIT_ID) );

MP4V2_EXPORT
MP4Timestamp MP4GetTrackEditMediaStart(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId );

MP4V2_EXPORT
bool MP4SetTrackEditMediaStart(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId,
    MP4Timestamp  startTime );

MP4V2_EXPORT
MP4Duration MP4GetTrackEditDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId );

MP4V2_EXPORT
bool MP4SetTrackEditDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId,
    MP4Duration   duration );

MP4V2_EXPORT
int8_t MP4GetTrackEditDwell(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId );

MP4V2_EXPORT
bool MP4SetTrackEditDwell(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4EditId     editId,
    bool          dwell );

MP4V2_EXPORT
bool MP4ReadSampleFromEditTime(
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
MP4SampleId MP4GetSampleIdFromEditTime(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  when,
    MP4Timestamp* pStartTime DEFAULT(NULL),
    MP4Duration*  pDuration DEFAULT(NULL) );

/* iTunes metadata handling */
MP4V2_EXPORT
bool MP4MetadataDelete(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4GetMetadataByIndex(
    MP4FileHandle hFile,
    uint32_t      index,
    char**        ppName, // need to free memory
    uint8_t**     ppValue,  // need to free
    uint32_t*     pValueSize );

MP4V2_EXPORT
bool MP4SetMetadataName(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataName(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataName(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataArtist(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataArtist(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataArtist(
    MP4FileHandle hFile );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4SetMetadataComposer
  */
bool MP4SetMetadataWriter(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4GetMetadataComposer
  */
bool MP4GetMetadataWriter(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4DeleteMetadataComposer
  */
bool MP4DeleteMetadataWriter(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataComposer(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataComposer(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataComposer(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataComment(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataComment(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataComment(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataTool(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataTool(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataTool(
    MP4FileHandle hFile );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4SetMetadataReleaseDate.
  */
bool MP4SetMetadataYear(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4GetMetadataReleaseDate.
  */
bool MP4GetMetadataYear(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4DeleteMetadataReleaseDate.
  */
bool MP4DeleteMetadataYear(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataReleaseDate(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataReleaseDate(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataReleaseDate(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataAlbum(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataAlbum(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataAlbum(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataTrack(
    MP4FileHandle hFile,
    uint16_t      track,
    uint16_t      totalTracks );

MP4V2_EXPORT
bool MP4GetMetadataTrack(
    MP4FileHandle hFile,
    uint16_t*     track,
    uint16_t*     totalTracks );

MP4V2_EXPORT
bool MP4DeleteMetadataTrack(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataDisk(
    MP4FileHandle hFile,
    uint16_t      disk,
    uint16_t      totalDisks );

MP4V2_EXPORT
bool MP4GetMetadataDisk(
    MP4FileHandle hFile,
    uint16_t*     disk,
    uint16_t*     totalDisks );

MP4V2_EXPORT
bool MP4DeleteMetadataDisk(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataGenre(
    MP4FileHandle hFile,
    const char*   genre );

MP4V2_EXPORT
bool MP4GetMetadataGenre(
    MP4FileHandle hFile,
    char**        genre );

MP4V2_EXPORT
bool MP4DeleteMetadataGenre(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataGrouping(
    MP4FileHandle hFile,
    const char* grouping );

MP4V2_EXPORT
bool MP4GetMetadataGrouping(
    MP4FileHandle hFile,
    char**        grouping );

MP4V2_EXPORT
bool MP4DeleteMetadataGrouping(
    MP4FileHandle hFile );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4SetMetadataBPM.
  */
bool MP4SetMetadataTempo(
    MP4FileHandle hFile,
    uint16_t      tempo );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4GetMetadataBPM.
  */
bool MP4GetMetadataTempo(
    MP4FileHandle hFile,
    uint16_t*     tempo );

MP4V2_EXPORT
MP4V2_DEPRECATED
/** 
  * @deprecated scheduled for removal. Use MP4DeleteMetadataBPM.
  */
bool MP4DeleteMetadataTempo(
    MP4FileHandle hFile );
    
MP4V2_EXPORT
bool MP4SetMetadataBPM(
    MP4FileHandle hFile,
    uint16_t      tempo );

MP4V2_EXPORT
bool MP4GetMetadataBPM(
    MP4FileHandle hFile,
    uint16_t*     tempo );

MP4V2_EXPORT
bool MP4DeleteMetadataBPM(
    MP4FileHandle hFile );

MP4V2_EXPORT
MP4V2_DEPRECATED
bool MP4DeleteMetadataTempo(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataCompilation(
    MP4FileHandle hFile,
    uint8_t       cpl );

MP4V2_EXPORT
bool MP4GetMetadataCompilation(
    MP4FileHandle hFile,
    uint8_t*      cpl );

MP4V2_EXPORT
bool MP4DeleteMetadataCompilation(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataPartOfGaplessAlbum(
    MP4FileHandle hFile,
    uint8_t       pgap );

MP4V2_EXPORT
bool MP4GetMetadataPartOfGaplessAlbum(
    MP4FileHandle hFile,
    uint8_t*      pgap );

MP4V2_EXPORT
bool MP4DeleteMetadataPartOfGaplessAlbum(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataCoverArt(
    MP4FileHandle hFile,
    uint8_t*      coverArt,
    uint32_t      size );

MP4V2_EXPORT
bool MP4GetMetadataCoverArt(
    MP4FileHandle hFile,
    uint8_t**     coverArt,
    uint32_t*     size,
    uint32_t      index DEFAULT(0) );

MP4V2_EXPORT
uint32_t MP4GetMetadataCoverArtCount(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4DeleteMetadataCoverArt(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataAlbumArtist(
    MP4FileHandle hFile,
    const char*   value );

MP4V2_EXPORT
bool MP4GetMetadataAlbumArtist(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4DeleteMetadataAlbumArtist(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataFreeForm(
    MP4FileHandle  hFile,
    const char*    name,
    const uint8_t* pValue,
    uint32_t       valueSize,
    const char*    owner DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4GetMetadataFreeForm(
    MP4FileHandle hFile,
    const char*   name,
    uint8_t**     pValue,
    uint32_t*     valueSize,
    const char*   owner DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4DeleteMetadataFreeForm(
    MP4FileHandle hFile,
    const char*   name,
    const char*   owner DEFAULT(NULL) );

MP4V2_EXPORT
void MP4AddIPodUUID(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

/* time conversion utilties */

/* predefined values for timeScale parameter below */
#define MP4_SECONDS_TIME_SCALE      1
#define MP4_MILLISECONDS_TIME_SCALE 1000
#define MP4_MICROSECONDS_TIME_SCALE 1000000
#define MP4_NANOSECONDS_TIME_SCALE  1000000000

#define MP4_SECS_TIME_SCALE     MP4_SECONDS_TIME_SCALE
#define MP4_MSECS_TIME_SCALE    MP4_MILLISECONDS_TIME_SCALE
#define MP4_USECS_TIME_SCALE    MP4_MICROSECONDS_TIME_SCALE
#define MP4_NSECS_TIME_SCALE    MP4_NANOSECONDS_TIME_SCALE

MP4V2_EXPORT
uint64_t MP4ConvertFromMovieDuration(
    MP4FileHandle hFile,
    MP4Duration   duration,
    uint32_t      timeScale );

MP4V2_EXPORT
uint64_t MP4ConvertFromTrackTimestamp(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Timestamp  timeStamp,
    uint32_t      timeScale );

MP4V2_EXPORT
MP4Timestamp MP4ConvertToTrackTimestamp(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint64_t      timeStamp,
    uint32_t      timeScale );

MP4V2_EXPORT
uint64_t MP4ConvertFromTrackDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    MP4Duration   duration,
    uint32_t      timeScale );

MP4V2_EXPORT
MP4Duration MP4ConvertToTrackDuration(
    MP4FileHandle hFile,
    MP4TrackId    trackId,
    uint64_t      duration,
    uint32_t      timeScale );

MP4V2_EXPORT
char* MP4BinaryToBase16(
    const uint8_t* pData,
    uint32_t       dataSize );

MP4V2_EXPORT
char* MP4BinaryToBase64(
    const uint8_t* pData,
    uint32_t       dataSize );

MP4V2_EXPORT
uint8_t* Base64ToBinary(
    const char* pData,
    uint32_t    decodeSize,
    uint32_t*   pDataSize );

MP4V2_EXPORT
void MP4Free(
    void* p );

MP4V2_EXPORT
void MP4SetLibFunc(
    lib_message_func_t libfunc );

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/* undefined our utlity macro to avoid conflicts */
#undef DEFAULT

/*****************************************************************************/

#endif /* MP4V2_MP4V2_H */
