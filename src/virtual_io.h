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

#ifndef MP4V2_IMPL_VIRTUAL_IO_H
#define MP4V2_IMPL_VIRTUAL_IO_H

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

extern Virtual_IO FILE_virtual_IO;

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

#endif // MP4V2_IMPL_VIRTUAL_IO_H
