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
 *      Dave Mackie     dmackie@cisco.com
 */

#include "src/impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

MP4FreeAtom::MP4FreeAtom( MP4File &file, const char* type )
    : MP4Atom( file, type ? type : "free" )
{
}

void MP4FreeAtom::Read()
{
    Skip();
}

void MP4FreeAtom::Write()
{
    bool use64 = (GetSize() > (0xFFFFFFFF - 8));
    BeginWrite(use64);

    static uint8_t freebuf[1024];
    for (uint64_t ix = 0; ix < GetSize(); ix += sizeof(freebuf)) {
        m_File.WriteBytes(freebuf, min(GetSize() - ix, (uint64_t)sizeof(freebuf)));
    }

    FinishWrite(use64);
}

///////////////////////////////////////////////////////////////////////////////

}
} // namespace mp4v2::impl
