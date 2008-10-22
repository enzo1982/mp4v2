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
 * Contributor(s): 
 *      Ben Allison         benski at nullsoft.com
 *
 * Virtual I/O support, for file support other than fopen/fread/fwrite
 */

#include "impl.h"

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

/* --------- Virtual IO for FILE * --------- */

uint64_t FILE_GetFileLength(void *user)
{
    FILE *fp = (FILE *)user;
    struct stat s;
    if (fstat(fileno(fp), &s) < 0) { 
        throw new MP4Error(errno, "stat failed", "MP4Open");
    }
    return s.st_size;
}

int FILE_SetPosition(void *user, uint64_t position)
{
    FILE *fp = (FILE *)user;
#if 0
    fpos_t fpos;
    VAR_TO_FPOS(fpos, position);
    return fsetpos(fp, &fpos);
#else
    off_t fpos = (off_t)position;
    return fseeko(fp, fpos, SEEK_SET);
#endif
}

int FILE_GetPosition(void *user, uint64_t *position)
{
    FILE *fp = (FILE *)user;
#if 0
    fpos_t fpos;
    if (fgetpos(fp, &fpos) < 0) { 
        throw new MP4Error(errno, "MP4-FILE_GetPosition");
    }
    
    FPOS_TO_VAR(fpos, uint64_t, *position);
#else
    off_t fpos = ftello(fp);
    if (fpos == -1) {
        throw new MP4Error(errno, "MP4-FILE_GetPosition");
    }
    *position = (uint64_t)fpos;
#endif
    return 0;
}

size_t FILE_Read(void *user, void *buffer, size_t size)
{
    FILE *fp = (FILE *)user;
    return fread(buffer, 1, size, fp);
}

size_t FILE_Write(void *user, void *buffer, size_t size)
{
    FILE *fp = (FILE *)user;
    return fwrite(buffer, 1, size, fp); 
}

int FILE_EndOfFile(void *user)
{
    FILE *fp = (FILE *)user;
    return feof(fp);
}

int FILE_Close(void *user)
{
    FILE *fp = (FILE *)user;
    return fclose(fp);
}

Virtual_IO FILE_virtual_IO =
{
    FILE_GetFileLength,
    FILE_SetPosition,
    FILE_GetPosition,
    FILE_Read,
    FILE_Write,
    FILE_EndOfFile,
    FILE_Close,
};

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl
