#ifndef MP4V2_META_H
#define MP4V2_META_H

/**************************************************************************//**
 *
 *  @defgroup mp4_meta MP4v2 iTunes Metadata
 *  @{
 *
 *****************************************************************************/

MP4V2_EXPORT
bool MP4MetadataDelete(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4GetMetadataByIndex(
    MP4FileHandle hFile,
    uint32_t      index,
    char**        ppName,   /* need to free memory */
    uint8_t**     ppValue,  /* need to free */
    uint32_t*     pValueSize );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataName(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataName(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataName(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataArtist(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataArtist(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataArtist(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataWriter(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataWriter(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataWriter(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataComment(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataComment(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataComment(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataTool(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataTool(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataTool(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataYear(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataYear(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataYear(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataAlbum(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataAlbum(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataAlbum(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataTrack(
    MP4FileHandle hFile,
    uint16_t      track,
    uint16_t      totalTracks );

MP4V2_EXPORT
bool MP4GetMetadataTrack(
    MP4FileHandle hFile,
    uint16_t*     track,
    uint16_t*     totalTracks );

MP4V2_EXPORT
bool MP4DeleteMetadataTrack(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataDisk(
    MP4FileHandle hFile,
    uint16_t      disk,
    uint16_t      totalDisks );

MP4V2_EXPORT
bool MP4GetMetadataDisk(
    MP4FileHandle hFile,
    uint16_t*     disk,
    uint16_t*     totalDisks );

MP4V2_EXPORT
bool MP4DeleteMetadataDisk(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataGenre(
    MP4FileHandle hFile,
    const char*   genre );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataGenre(
    MP4FileHandle hFile,
    char**        genre );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataGenre(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataGrouping(
    MP4FileHandle hFile,
    const char* grouping );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataGrouping(
    MP4FileHandle hFile,
    char**        grouping );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataGrouping(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataTempo(
    MP4FileHandle hFile,
    uint16_t      tempo );

MP4V2_EXPORT
bool MP4GetMetadataTempo(
    MP4FileHandle hFile,
    uint16_t*     tempo );

MP4V2_EXPORT
bool MP4DeleteMetadataTempo(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataCompilation(
    MP4FileHandle hFile,
    uint8_t       cpl );

MP4V2_EXPORT
bool MP4GetMetadataCompilation(
    MP4FileHandle hFile,
    uint8_t*      cpl );

MP4V2_EXPORT
bool MP4DeleteMetadataCompilation(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataPartOfGaplessAlbum(
    MP4FileHandle hFile,
    uint8_t       pgap );

MP4V2_EXPORT
bool MP4GetMetadataPartOfGaplessAlbum(
    MP4FileHandle hFile,
    uint8_t*      pgap );

MP4V2_EXPORT
bool MP4DeleteMetadataPartOfGaplessAlbum(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataCoverArt(
    MP4FileHandle hFile,
    uint8_t*      coverArt,
    uint32_t      size );

MP4V2_EXPORT
bool MP4GetMetadataCoverArt(
    MP4FileHandle hFile,
    uint8_t**     coverArt,
    uint32_t*     size,
    uint32_t      index DEFAULT(0) );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
uint32_t MP4GetMetadataCoverArtCount(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4DeleteMetadataCoverArt(
    MP4FileHandle hFile );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4SetMetadataAlbumArtist(
    MP4FileHandle hFile,
    const char*   value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4GetMetadataAlbumArtist(
    MP4FileHandle hFile,
    char**        value );

/** 
  * @deprecated Deprecated, scheduled for removal. Please use the tags convenience API.
  */
MP4V2_DEPRECATED
MP4V2_EXPORT
bool MP4DeleteMetadataAlbumArtist(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataHDVideo(
    MP4FileHandle hFile,
    uint8_t       value );

MP4V2_EXPORT
bool MP4GetMetadataHDVideo(
    MP4FileHandle hFile,
    uint8_t*      value );

MP4V2_EXPORT
bool MP4DeleteMetadataHDVideo(
    MP4FileHandle hFile );
  
MP4V2_EXPORT
bool MP4SetMetadataTVEpisode(
    MP4FileHandle hFile,
    uint32_t       value );

MP4V2_EXPORT
bool MP4GetMetadataTVEpisode(
    MP4FileHandle hFile,
    uint32_t*      value );

MP4V2_EXPORT
bool MP4DeleteMetadataTVEpisode(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4GetMetadataTVSeason(
    MP4FileHandle hFile,
    uint32_t*      value );
    
MP4V2_EXPORT
bool MP4GetMetadataKeywords(
    MP4FileHandle hFile,
    char**        value );

MP4V2_EXPORT
bool MP4SetMetadataCNID(
    MP4FileHandle hFile,
    uint32_t       value );

MP4V2_EXPORT
bool MP4DeleteMetadataCNID(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4GetMetadataCNID(
    MP4FileHandle hFile,
    uint32_t*      value );

MP4V2_EXPORT
bool MP4SetMetadataMediaType(
    MP4FileHandle hFile,
    uint8_t       value );

MP4V2_EXPORT
bool MP4GetMetadataMediaType(
    MP4FileHandle hFile,
    uint8_t*      value );

MP4V2_EXPORT
bool MP4DeleteMetadataMediaType(
    MP4FileHandle hFile );

MP4V2_EXPORT
bool MP4SetMetadataFreeForm(
    MP4FileHandle  hFile,
    const char*    name,
    const uint8_t* pValue,
    uint32_t       valueSize,
    const char*    owner DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4GetMetadataFreeForm(
    MP4FileHandle hFile,
    const char*   name,
    uint8_t**     pValue,
    uint32_t*     valueSize,
    const char*   owner DEFAULT(NULL) );

MP4V2_EXPORT
bool MP4DeleteMetadataFreeForm(
    MP4FileHandle hFile,
    const char*   name,
    const char*   owner DEFAULT(NULL) );

MP4V2_EXPORT
void MP4AddIPodUUID(
    MP4FileHandle hFile,
    MP4TrackId    trackId );

/*****************************************************************************/

/** Enumeration of possible MP4TagArtwork::type values. */
typedef enum MP4TagArtworkType_e
{
    MP4_ART_UNDEFINED = 0,
    MP4_ART_BMP       = 1,
    MP4_ART_GIF       = 2,
    MP4_ART_JPEG      = 3,
    MP4_ART_PNG       = 4,
} MP4TagArtworkType;

/** Data object representing a single piece of artwork. */
typedef struct MP4TagArtwork_s
{
    const void*       data; /**< raw picture data */
    uint32_t          size; /**< data size in bytes */
    MP4TagArtworkType type; /**< data type */
} MP4TagArtwork;

/** Tags <b>convenience</b> structure.
 *
 *  This structure is used in the tags convenience API which allows for
 *  simplified retrieval and modification of the majority of known tags.
 *
 *  This is a read-only structure and each tag is present if and only if the
 *  pointer is a <b>non-NULL</b> value. The actual data is backed by a hidden
 *  data cache which is only updated when the appropriate metadata <b>set</b>
 *  function is used, or if MP4TagsFetch() is invoked. Thus, if other API
 *  is used to manipulate relevent atom structure of the MP4 file, the user
 *  is responsible for re-fetching the data in this structure.
 *
 *  Example usage:
 *  @include example/tags/tags.c
 */
typedef struct MP4Tags_s
{
    void* __handle; /* private use */

    const char*     name;
    const char*     artist;
    const char*     albumArtist; 
    const char*     album;
    const char*     grouping;
    const char*     composer;
    const char*     comments;
    const char*     genre;
    const char*     releaseDate;
    const uint16_t* beatsPerMinute;
    const uint8_t*  compilation;
    
    const char*     tvShow;
    const char*     tvNetwork;
    const char*     tvEpisodeID;
    const uint32_t* tvSeason;
    const uint32_t* tvEpisode;
    
    const char*     description;
    const char*     longDescription;
    const char*     lyrics;
    
    const char*     sortName;
    const char*     sortArtist;
    const char*     sortAlbumArtist; 
    const char*     sortAlbum;
    const char*     sortComposer;
    const char*     sortTVShow;

    const MP4TagArtwork* artwork;
    uint32_t             artworkCount;

    const char*     copyright;
    const char*     encodingTool;
    const char*     encodedBy;
    const char*     purchaseDate;
 
    const char*     keywords;  /* TODO: Needs testing */
    const char*     category;
    const uint8_t*  podcast;
    
    const uint8_t*  hdVideo;
    const uint8_t*  mediaType;
    const uint8_t*  contentRating;
    const uint8_t*  gapless;
    
    const char*     iTunesAccount;
    const uint32_t* cnID;
} MP4Tags;

/** Allocate tags convenience structure for reading and settings tags.
 *
 *  This function allocates a new structure which represents a snapshot
 *  of all the tags therein, tracking if the tag is missing,
 *  or present and with value. It is the caller's responsibility to free
 *  the structure with MP4TagsFree().
 *
 *  @return structure with all tags missing.
 */
MP4V2_EXPORT
const MP4Tags* MP4TagsAlloc();

/** Fetch data from mp4 file and populate structure.
 *
 *  The tags structure and its hidden data-cache is updated to
 *  reflect the actual tags values found in the <b>hFile</b>.
 *
 *  @param tags structure to fetch (write) into.
 *  @param hFile handle of file to fetch data from.
 */
MP4V2_EXPORT
void MP4TagsFetch( const MP4Tags* tags, MP4FileHandle hFile );

/** Store data to mp4 file from structure.
 *
 *  The tags structure is pushed out to the mp4 file,
 *  adding tags if needed, removing tags if needed, and updating
 *  the values to modified tags.
 *
 *  @param tags structure to store (read) from.
 *  @param hFile handle of file to store data to.
 */
MP4V2_EXPORT
void MP4TagsStore( const MP4Tags* tags, MP4FileHandle hFile );

/** Free tags convenience structure.
 *
 *  This function frees memory associated with the structure.
 *
 *  @param tags structure to destroy.
 */
MP4V2_EXPORT
void MP4TagsFree( const MP4Tags* tags );

MP4V2_EXPORT void MP4TagsSetName        ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetArtist      ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetAlbumArtist ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetAlbum       ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetGrouping    ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetComposer    ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetComments    ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetGenre       ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetReleaseDate ( const MP4Tags*, const char* );

MP4V2_EXPORT void MP4TagsSetDescription( const MP4Tags*, const char* );

MP4V2_EXPORT void MP4TagsSetCopyright    ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetEncodingTool ( const MP4Tags*, const char* );
MP4V2_EXPORT void MP4TagsSetEncodedBy    ( const MP4Tags*, const char* );

MP4V2_EXPORT void MP4TagsAddArtwork    ( const MP4Tags*, MP4TagArtwork* );
MP4V2_EXPORT void MP4TagsSetArtwork    ( const MP4Tags*, uint32_t, MP4TagArtwork* );
MP4V2_EXPORT void MP4TagsRemoveArtwork ( const MP4Tags*, uint32_t );

/** @} ***********************************************************************/

#endif /* MP4V2_META_H */
