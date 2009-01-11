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
 * 3GPP features implementation is based on 3GPP's TS26.234-v5.60,
 * and was contributed by Ximpo Group Ltd.
 *
 * Portions created by Ximpo Group Ltd. are
 * Copyright (C) Ximpo Group Ltd. 2003, 2004.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie                dmackie@cisco.com
 *      Alix Marchandise-Franquet  alix@cisco.com
 *      Ximpo Group Ltd.           mp4v2@ximpo.com
 *      Bill May                   wmay@cisco.com
 */

/*
 * MP4 library API functions
 *
 * These are wrapper functions that provide C linkage conventions
 * to the library, and catch any internal errors, ensuring that
 * a proper return value is given.
 */

#include "src/impl.h"

#define PRINT_ERROR(e) \
    VERBOSE_ERROR(((MP4File*)hFile)->GetVerbosity(), e->Print());

using namespace mp4v2::impl;

extern "C" {

///////////////////////////////////////////////////////////////////////////////

/* iTunes meta data handling */
    bool MP4GetMetadataByIndex(MP4FileHandle hFile, uint32_t index,
                               char** ppName,
                               uint8_t** ppValue, uint32_t* pValueSize)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataByIndex(
                           index, ppName, ppValue, pValueSize);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4MetadataDelete(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->MetadataDelete();
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataName(MP4FileHandle hFile,
                            const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251nam", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataName(MP4FileHandle hFile,
                            char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251nam", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataName(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251nam");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataWriter(MP4FileHandle hFile,
                              const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251wrt", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataWriter(MP4FileHandle hFile,
                              char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251wrt", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4DeleteMetadataWriter(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251wrt");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataAlbum(MP4FileHandle hFile,
                             const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251alb", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataAlbum(MP4FileHandle hFile,
                             char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251alb", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataAlbum(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251alb");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataArtist(MP4FileHandle hFile,
                              const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251ART", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataArtist(MP4FileHandle hFile,
                              char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251ART", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataArtist(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251ART");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataTool(MP4FileHandle hFile,
                            const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251too", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataTool(MP4FileHandle hFile,
                            char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251too", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataTool(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251too");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataComment(MP4FileHandle hFile,
                               const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251cmt", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataComment(MP4FileHandle hFile,
                               char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251cmt", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataComment(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251cmt");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataYear(MP4FileHandle hFile,
                            const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251day", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataYear(MP4FileHandle hFile,
                            char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251day", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4DeleteMetadataYear(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251day");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataTrack(MP4FileHandle hFile,
                             uint16_t track, uint16_t totalTracks)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataTrack(track, totalTracks);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataTrack(MP4FileHandle hFile,
                             uint16_t* track, uint16_t* totalTracks)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataTrack(track, totalTracks);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataTrack(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("trkn");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataDisk(MP4FileHandle hFile,
                            uint16_t disk, uint16_t totalDisks)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataDisk(disk, totalDisks);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataDisk(MP4FileHandle hFile,
                            uint16_t* disk, uint16_t* totalDisks)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataDisk(disk, totalDisks);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataDisk(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("disk");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataGenre(MP4FileHandle hFile, const char *genre)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataGenre(genre);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataGenre(MP4FileHandle hFile, char **genre)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataGenre(genre);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataGenre(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataGenre();
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataGrouping(MP4FileHandle hFile, const char *grouping)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("\251grp", grouping);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataGrouping(MP4FileHandle hFile, char **grouping)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("\251grp", grouping);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataGrouping(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("\251grp");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataTempo(MP4FileHandle hFile, uint16_t tempo)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint16("tmpo", tempo);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataTempo(MP4FileHandle hFile, uint16_t* tempo)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint16("tmpo", tempo);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4DeleteMetadataTempo(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("tmpo");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataCompilation(MP4FileHandle hFile, uint8_t cpl)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint8("cpil", cpl & 0x1);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataCompilation(MP4FileHandle hFile, uint8_t* cpl)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint8("cpil", cpl);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataCompilation(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("cpil");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataPartOfGaplessAlbum (MP4FileHandle hFile,
                                           uint8_t pgap)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint8("pgap", pgap & 0x1);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataPartOfGaplessAlbum (MP4FileHandle hFile,
                                           uint8_t* pgap)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint8("pgap", pgap);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataPartOfGaplessAlbum (MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("pgap");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataCoverArt(MP4FileHandle hFile,
                                uint8_t *coverArt, uint32_t size)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataCoverArt(coverArt, size);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataCoverArt(MP4FileHandle hFile,
                                uint8_t **coverArt, uint32_t* size,
                                uint32_t index)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataCoverArt(coverArt, size, index);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    uint32_t MP4GetMetadataCoverArtCount(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataCoverArtCount();
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataCoverArt(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("covr");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4SetMetadataAlbumArtist (MP4FileHandle hFile,
                                    const char* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataString("aART", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataAlbumArtist (MP4FileHandle hFile,
                                    char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("aART", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataAlbumArtist (MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("aART");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataHDVideo(MP4FileHandle hFile, uint8_t value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint8("hdvd", value & 0x1);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataHDVideo(MP4FileHandle hFile, uint8_t* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint8("hdvd", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataHDVideo(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("hdvd");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataTVEpisode(MP4FileHandle hFile, uint32_t value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint32("tves", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataTVEpisode(MP4FileHandle hFile, uint32_t* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint32("tves", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4DeleteMetadataTVEpisode(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("tves");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataTVSeason(MP4FileHandle hFile, uint32_t* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint32("tvsn", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataKeywords (MP4FileHandle hFile,
                                    char** value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataString("keyw", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataCNID(MP4FileHandle hFile, uint32_t value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint32("cnID", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataCNID(MP4FileHandle hFile, uint32_t* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint32("cnID", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4DeleteMetadataCNID(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("cnID");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4SetMetadataMediaType(MP4FileHandle hFile, uint8_t value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataUint8("stik", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4GetMetadataMediaType(MP4FileHandle hFile, uint8_t* value)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataUint8("stik", value);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }
    
    bool MP4DeleteMetadataMediaType(MP4FileHandle hFile)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataAtom("stik");
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }    
    
    bool MP4SetMetadataFreeForm(MP4FileHandle hFile,
                                const char *name,
                                const uint8_t* pValue,
                                uint32_t valueSize,
                                const char *owner)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->SetMetadataFreeForm(name, pValue, valueSize, owner);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4GetMetadataFreeForm(MP4FileHandle hFile, const char *name,
                                uint8_t** pValue, uint32_t* valueSize, const char *owner)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->GetMetadataFreeForm(name, pValue, valueSize, owner);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

    bool MP4DeleteMetadataFreeForm(MP4FileHandle hFile, const char *name, const char *owner)
    {
        if (MP4_IS_VALID_FILE_HANDLE(hFile)) {
            try {
                return ((MP4File*)hFile)->DeleteMetadataFreeForm(name, owner);
            }
            catch (MP4Error* e) {
                PRINT_ERROR(e);
                delete e;
            }
        }
        return false;
    }

///////////////////////////////////////////////////////////////////////////////

const MP4Metadata*
MP4MetadataAlloc( MP4FileHandle file )
{
    MP4Metadata* result = NULL;

    if( !MP4_IS_VALID_FILE_HANDLE( file ))
        return result;

    try {
        itmf::Metadata& m = *new itmf::Metadata( file );
        m.c_alloc( result );
        return result;
    }
    catch( MP4Error* e ) {
        VERBOSE_ERROR( static_cast<MP4File*>(file)->GetVerbosity(), e->Print() );
        delete e;
    }

    delete result;
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataFree( const MP4Metadata* m )
{
    itmf::Metadata* cpp = static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata* c = const_cast<MP4Metadata*>(m);
    cpp->c_free( c );
    delete cpp;
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataFetch( const MP4Metadata* m )
{
    itmf::Metadata* cpp = static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata* c = const_cast<MP4Metadata*>(m);

    try {
        cpp->c_fetch( c );
    }
    catch( MP4Error* e ) {
        VERBOSE_ERROR( cpp->file.GetVerbosity(), e->Print() );
        delete e;
    }
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataStore( const MP4Metadata* m )
{
    itmf::Metadata* cpp = static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata* c = const_cast<MP4Metadata*>(m);

    try {
        cpp->c_store( c );
    }
    catch( MP4Error* e ) {
        VERBOSE_ERROR( cpp->file.GetVerbosity(), e->Print() );
        delete e;
    }
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetName( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.name, c.name );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetArtist( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.artist, c.artist );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetAlbumArtist( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.albumArtist, c.albumArtist );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetAlbum( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.album, c.album );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetGrouping( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.grouping, c.grouping );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetComposer( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.composer, c.composer );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetComments( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.comments, c.comments );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetReleaseDate( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.releaseDate, c.releaseDate );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetDescription( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.description, c.description );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetCopyright( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.copyright, c.copyright );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetEncodingTool( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.encodingTool, c.encodingTool );
}

///////////////////////////////////////////////////////////////////////////////

void
MP4MetadataSetEncodedBy( const MP4Metadata* m, const char* value )
{
    itmf::Metadata& cpp = *static_cast<itmf::Metadata*>(m->__handle);
    MP4Metadata& c = *const_cast<MP4Metadata*>(m);
    cpp.setString( value, cpp.encodedBy, c.encodedBy );
}

///////////////////////////////////////////////////////////////////////////////
} // extern "C"
