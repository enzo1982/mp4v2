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
 * Copyright (C) Cisco Systems Inc. 2001-2002.  All Rights Reserved.
 * 
 * Contributor(s): 
 *        Dave Mackie        dmackie@cisco.com
 */

// N.B. mp4clips clips tracks in an mp4 file
// without regard to sync samples aka "key frames"
// as a testing app the burden is on the user to choose
// an appropriate clip start time

#include "util/impl.h"

using namespace mp4v2::util;

char* ProgName;

// forward declaration
void ClipTrack(
    MP4FileHandle srcFile, 
    MP4TrackId trackId, 
    MP4FileHandle dstFile,
    float startTime,
    float duration);


int main(int argc, char** argv)
{
    const char* usageString = "usage: %s [-t <track-id>] [-v [<level>]] [-s <start>] -l <duration> <file-name>\n";
    char* srcFileName = NULL;
    char* dstFileName = NULL;
    MP4TrackId trackId = MP4_INVALID_TRACK_ID;
    MP4LogLevel verbosity = MP4_LOG_ERROR;
    float startTime = 0.0;
    float duration = 0.0;

    /* begin processing command line */
    ProgName = argv[0];
    while (true) {
        int c = -1;
        int option_index = 0;
        static const prog::Option long_options[] = {
            { "length",  prog::Option::REQUIRED_ARG, 0, 'l' },
            { "start",   prog::Option::REQUIRED_ARG, 0, 's' },
            { "track",   prog::Option::REQUIRED_ARG, 0, 't' },
            { "verbose", prog::Option::OPTIONAL_ARG, 0, 'v' },
            { "version", prog::Option::NO_ARG,       0, 'V' },
            { NULL, prog::Option::NO_ARG, 0, 0 }
        };

        c = prog::getOptionSingle( argc, argv, "l:s:t:v::V", long_options, &option_index );

        if (c == -1)
            break;

        switch (c) {
        case 'l':
            if (sscanf(prog::optarg, "%f", &duration) != 1) {
                fprintf(stderr, 
                    "%s: bad length specified: %s\n",
                     ProgName, optarg);
            }
            break;
        case 's':
            if (sscanf(prog::optarg, "%f", &startTime) != 1) {
                fprintf(stderr, 
                    "%s: bad start time specified: %s\n",
                     ProgName, optarg);
            }
            break;
        case 't':
            if (sscanf(prog::optarg, "%u", &trackId) != 1) {
                fprintf(stderr, 
                    "%s: bad track-id specified: %s\n",
                     ProgName, optarg);
                return 1;
            }
            break;
        case 'v':
            verbosity = MP4_LOG_VERBOSE1;
            if (prog::optarg) {
                uint32_t level;
                if (sscanf(prog::optarg, "%u", &level) == 1) {
                    if (level >= 2) {
                        verbosity = MP4_LOG_VERBOSE2;
                    } 
                    if (level >= 3) {
                        verbosity = MP4_LOG_VERBOSE3;
                    } 
                    if (level >= 4) {
                        verbosity = MP4_LOG_VERBOSE4;
                    }
                }
            }
            break;
        case '?':
            fprintf(stderr, usageString, ProgName);
            return 0;
        case 'V':
          fprintf(stderr, "%s - %s version %s\n", 
              ProgName, MP4V2_PROJECT_name, MP4V2_PROJECT_version);
          return 0;
        default:
            fprintf(stderr, "%s: unknown option specified, ignoring: %c\n", 
                ProgName, c);
        }
    }

    /* check that we have at least one non-option argument */
    if ((argc - prog::optind) < 1) {
        fprintf(stderr, usageString, ProgName);
        return 1;
    }
    
    MP4LogSetLevel(verbosity);
    if (verbosity > MP4_LOG_INFO) {
        fprintf(stderr, "%s version %s\n", ProgName, MP4V2_PROJECT_version);
    }

    /* point to the specified file name */
    srcFileName = argv[prog::optind++];

    /* get dest file name */
    if ((argc - optind) > 0) {
        dstFileName = argv[prog::optind++];
    }

    /* warn about extraneous non-option arguments */
    if (optind < argc) {
        fprintf(stderr, "%s: unknown options specified, ignoring: ", ProgName);
        while (prog::optind < argc) {
            fprintf(stderr, "%s ", argv[prog::optind++]);
        }
        fprintf(stderr, "\n");
    }

    if (duration == 0.0) {
        fprintf(stderr,
            "%s: please specify clip length with -l option\n",
            ProgName);
    }

    /* end processing of command line */

    MP4FileHandle srcFile = MP4Modify(srcFileName);
    if (!srcFile)
        return 1;

    MP4FileHandle dstFile = MP4_INVALID_FILE_HANDLE;
    if (dstFileName)
        dstFile = MP4Create(dstFileName);

    if (trackId == MP4_INVALID_TRACK_ID) {
        uint32_t numTracks = MP4GetNumberOfTracks(srcFile);

        for (uint32_t i = 0; i < numTracks; i++) {
            trackId = MP4FindTrackId(srcFile, i);
            ClipTrack(srcFile, trackId, dstFile, startTime, duration);
        }
    } else {
        ClipTrack(srcFile, trackId, dstFile, startTime, duration);
    }

    MP4Close(srcFile);
    if (dstFile != MP4_INVALID_FILE_HANDLE)
        MP4Close(dstFile);

    return(0);
}

void ClipTrack(
    MP4FileHandle srcFile, 
    MP4TrackId trackId, 
    MP4FileHandle dstFile,
    float startTime,
    float duration)
{
    MP4Timestamp trackStartTime =
        MP4ConvertToTrackTimestamp(
            srcFile,
            trackId,
            (uint64_t)(startTime * 1000),
            MP4_MSECS_TIME_SCALE);

    MP4Duration trackDuration =
        MP4ConvertToTrackDuration(
            srcFile,
            trackId,
            (uint64_t)(duration * 1000),
            MP4_MSECS_TIME_SCALE);

    MP4EditId editId = 
        MP4AddTrackEdit(
            srcFile, 
            trackId,
            1,
            trackStartTime,
            trackDuration);

    if (editId == MP4_INVALID_EDIT_ID) {
        fprintf(stderr,
            "%s: can't create track edit\n",
            ProgName);
        return;
    }

    if (dstFile) {
        MP4CopyTrack(
            srcFile,
            trackId,
            dstFile,
            true);

        MP4DeleteTrackEdit(
            srcFile,
            trackId, 
            editId);
    }
}

