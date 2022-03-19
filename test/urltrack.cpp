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
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *        Dave Mackie        dmackie@cisco.com
 */

#include <mp4v2/mp4v2.h>
#include <memory.h>
#include <string.h>

main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    MP4LogSetLevel(MP4_LOG_VERBOSE4);
    MP4FileHandle mp4File = MP4Create(argv[1]);
    if (!mp4File)
        return 1;

    MP4TrackId urlTrackId = 
#if 0
    MP4AddTrack(mp4File, "URLF");
#else
    MP4AddHrefTrack(mp4File, 90000, MP4_INVALID_DURATION);
#endif
    printf("urlTrackId %d\n", urlTrackId);

    uint8_t i;
    char url[128];

    for (i = 1; i <= 5; i++) {
        sprintf(url, "http://server.com/foo/bar%u.html", i);

        MP4WriteSample(mp4File, urlTrackId, 
            (uint8_t*)url, strlen(url) + 1, (MP4Duration)i);
    }

    MP4Close(mp4File);

    mp4File = MP4Read(argv[1]);

    // check that we can find the track again
#if 0
    urlTrackId = MP4FindTrackId(mp4File, 0, "URLF");
#else
    urlTrackId = MP4FindTrackId(mp4File, 0, MP4_CNTL_TRACK_TYPE);
#endif
    printf("urlTrackId %d\n", urlTrackId);
    
    for (i = 1; i <= 5; i++) {
        uint8_t* pSample = NULL;
        uint32_t sampleSize = 0;
        MP4Duration duration;
        bool rc;

        rc = MP4ReadSample(mp4File, urlTrackId, i,
            &pSample, &sampleSize, NULL, &duration);

        if (rc) {
            printf("Sample %i duration %llu: %s\n", 
                i, duration, pSample);
            MP4Free(pSample);
        } else {
            printf("Couldn't read sample %i\n", i);
        }
    }

    MP4Close(mp4File);

    return 0;
}

