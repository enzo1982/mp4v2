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
 * See ETSI TS 102 366 V1.2.1 Annex F for how to put Ac3 in MP4.
 *
 * Contributor(s):
 *      Edward Groenendaal      egroenen@cisco.com
 */

#include "impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////


MP4DAc3Atom::MP4DAc3Atom()
        : MP4Atom("dac3")
{
    AddProperty( new MP4BitfieldProperty("fscod", 2)); /* 0 */
    AddProperty( new MP4BitfieldProperty("bsid", 5)); /* 1 */
    AddProperty( new MP4BitfieldProperty("bsmod", 3)); /* 2 */
    AddProperty( new MP4BitfieldProperty("acmod", 3)); /* 3 */
    AddProperty( new MP4BitfieldProperty("lfeon", 1)); /* 4 */
    AddProperty( new MP4BitfieldProperty("bit_rate_code", 5)); /* 5 */
    AddProperty( new MP4BitfieldProperty("reserved", 5)); /* 6 */
    m_pProperties[6]->SetReadOnly(true);
}

void MP4DAc3Atom::Generate()
{
    MP4Atom::Generate();

    // The user had better fill in these values themselves based on what is in the bitstream.
    //
    //  Table F.1: bit_rate_code 
    //
    // Bit_rate_code Nominal bit rate (kbit/s) 
    // 00000         32 
    // 00001         40 
    // 00010         48 
    // 00011         56 
    // 00100         64 
    // 00101         80 
    // 00110         96 
    // 00111        112 
    // 01000        128 
    // 01001        160 
    // 01010        192 
    // 01011        224 
    // 01100        256 
    // 01101        320 
    // 01110        384 
    // 01111        448 
    // 10000        512 
    // 10001        576 
    // 10010        640 

}

///////////////////////////////////////////////////////////////////////////////

}
} // namespace mp4v2::impl
