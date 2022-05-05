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

int main(int argc, char** argv)
{
#if 0
    MP4FileHandle mp4File = MP4Read(argv[1]);
    if (!mp4File)
        return 1;

    MP4Dump(mp4File, 0);
    MP4Close(mp4File, 0);

    return 0;
#else
    MP4FileHandle mp4File = MP4Read(argv[1]);
    MP4LogSetLevel(MP4_LOG_VERBOSE4);
    if (MP4HaveAtom(mp4File, "moov.mvhd")) {
      printf("found moov.mvhd\n");
    }
    if (MP4HaveTrackAtom(mp4File, 1, argv[2])) {
      printf("found %s\n", argv[2]);
    } else {
      printf("didn't find it\n");
    }
        
#if 0
    uint8_t* data = (uint8_t*)"this is my tag data";
    uint32_t len = strlen((char*)data);
    MP4SetMetadataFreeForm(mp4File, "mytag1", data, len);
    MP4SetMetadataFreeForm(mp4File, "my_tag2", data, len);
#endif
    MP4Close(mp4File, 0);
    return 0;
#endif
}

