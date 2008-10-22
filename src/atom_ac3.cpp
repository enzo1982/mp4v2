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
 *      Edward Groenendaal      egroenen@cisco.com
 */

#include "impl.h"

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

MP4Ac3Atom::MP4Ac3Atom() 
    : MP4Atom("ac-3") 
{
    AddProperty( new MP4BytesProperty("data", 47));
}

void MP4Ac3Atom::Generate()
{
    MP4Atom::Generate();

    // This is a nasty nasty hack because Apple haven't told us what is
    // in the AC3 atom. So I've just copied this raw data from a sample
    // file.
    // I believe most of this is actually the same as the mp4a atom.
    // I can play with this later to get it 100% right, for now this works.
    //
    // Note that the format of this atom is now well known and will be
    // filled in properly soon using the correct AC3 values.
    //
    static uint8_t ac3[39] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0xbb,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 
        0x64, 0x61, 0x63, 0x33, 0x10, 0x3d, 0x40,
    };

    m_pProperties[0]->SetReadOnly(false);
    ((MP4BytesProperty*)m_pProperties[0])->SetValue(ac3, sizeof(ac3));
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl
