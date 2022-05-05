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
#include <string.h>
#include <stdlib.h>

static bool convertBase64 (const char data, uint8_t *value)
{
    static const uint8_t decodingarr64[128] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
        0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
        0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,
    };
    uint8_t index = (uint8_t)data;
    if ((index & 0x80) != 0) return false;

    if (decodingarr64[index] == 0xff) return false;
    *value = decodingarr64[index];
    return true;
}

uint8_t *Base64ToBinary (const char *pData, uint32_t decodeSize, uint32_t *pDataSize)
{
    uint8_t *ret;
    uint32_t size, ix, groups;
    if (pData == NULL ||  decodeSize == 0 || pDataSize == NULL)
        return NULL;

    if ((decodeSize % 4) != 0) {
        // must be multiples of 4 characters
        return NULL;
    }
    size = (decodeSize * 3) / 4;
    groups = decodeSize / 4;
    ret = (uint8_t *)calloc(1, size);
    if (ret == NULL) return NULL;
    for (ix = 0; ix < groups; ix++) {
        uint8_t value[4];
        for (uint8_t jx = 0; jx < 4; jx++) {
            if (pData[jx] == '=') {
                if (ix != (groups - 1)) {
                    free(ret);
                    return NULL;
                }
                size--;
                value[jx] = 0;
            } else if (convertBase64(pData[jx], &value[jx]) == false) {
                free(ret);
                return NULL;
            }
        }
        ret[(ix * 3)] = value[0] << 2 | ((value[1] >> 4) & 0x3);
        ret[(ix * 3) + 1] = (value[1] << 4) | (value[2] >> 2 & 0xf);
        ret[(ix * 3) + 2] = ((value[2] & 0x3) << 6) | value[3];
        pData += 4;
    }
    *pDataSize = size;
    return ret;
}

main(int argc, char** argv)
{
#if 1
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    MP4LogSetLevel(MP4_LOG_VERBOSE4);
    MP4FileHandle mp4File = MP4Create(argv[1]);
    if (!mp4File)
        return 1;

    printf("Created skeleton\n");
    MP4Dump(mp4File);

    MP4SetODProfileLevel(mp4File, 1);
    MP4SetSceneProfileLevel(mp4File, 1);
    MP4SetVideoProfileLevel(mp4File, 1);
    MP4SetAudioProfileLevel(mp4File, 1);
    MP4SetGraphicsProfileLevel(mp4File, 1);

    MP4TrackId odTrackId = 
        MP4AddODTrack(mp4File);

    MP4TrackId bifsTrackId = 
        MP4AddSceneTrack(mp4File);

    MP4TrackId videoTrackId = 
#if 0
    MP4AddVideoTrack(mp4File, 90000, 3000, 320, 240);
#else
    MP4AddH264VideoTrack(mp4File, 90000, 3000, 320, 240, 
                 1, 2, 3, 1);
    static uint8_t pseq[] = { 0, 1, 2, 3, 4, 5, 6,7, 8, 9 };

    MP4AddH264SequenceParameterSet(mp4File, videoTrackId, pseq, 10);
    MP4AddH264SequenceParameterSet(mp4File, videoTrackId, pseq, 6);
    MP4AddH264PictureParameterSet(mp4File, videoTrackId, pseq, 7);
    MP4AddH264PictureParameterSet(mp4File, videoTrackId, pseq, 8);
    MP4AddH264PictureParameterSet(mp4File, videoTrackId, pseq, 7);

#endif

    MP4TrackId videoHintTrackId = 
        MP4AddHintTrack(mp4File, videoTrackId);

    MP4TrackId audioTrackId = 
        MP4AddAudioTrack(mp4File, 44100, 1152);

    MP4TrackId audioHintTrackId = 
        MP4AddHintTrack(mp4File, audioTrackId);

    printf("Added tracks\n");
    MP4Dump(mp4File);

    MP4Close(mp4File);

    // MP4MakeIsmaCompliant(argv[1], verbosity);

    return 0;
#else
   uint8_t *bin = NULL;

   for (uint32_t ix = 4; ix < 1024; ix++) {
     printf("pass %d\n", ix);
     bin = (uint8_t *)malloc(ix);
     for (uint32_t jx = 0; jx < ix; jx++) {
       bin[jx] = ((uint32_t)rand()) & 0xFF;
     }
     char *test;
     test = MP4BinaryToBase64(bin, ix);
     uint8_t *ret;
     uint32_t retsize;
     ret = Base64ToBinary(test, strlen(test), &retsize);
     if (retsize != ix) {
       printf("return size not same %d %d\n", ix, retsize);
       return 0;
     }
     if (memcmp(ret, bin, ix) != 0) {
       printf("memory not same\n");
       return 0;
     }
     free(test);
     free(ret);
     free(bin);
   }
   return 0;
#endif
}

