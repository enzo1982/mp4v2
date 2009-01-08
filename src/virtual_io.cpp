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

#include "src/impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

/* --------- Virtual IO for FILE * --------- */

uint64_t FILE_GetFileLength(void *user)
{
    // TODO-KB: test io::StdioFile
    io::File::Size i;
    if( io::StdioFile::getSize( (FILE*)user, i ))
        throw new MP4Error( sys::getLastError(), "getSize failed", "MP4-FILE_GetFileLength" );
    return i;
}

int FILE_SetPosition(void *user, uint64_t pos)
{
    // TODO-KB: test io::StdioFile
    return io::StdioFile::setPosition( (FILE*)user, pos );
}

int FILE_GetPosition(void *user, uint64_t* pos)
{
    // TODO-KB: test io::StdioFile
    io::File::Size i;
    if( io::StdioFile::getPosition( (FILE*)user, i ))
        throw new MP4Error( sys::getLastError(), "MP4-FILE_GetPosition" );
    *pos = i;
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

}
} // namespace mp4v2::impl
