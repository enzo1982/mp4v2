#ifndef MP4V2_XTRA_TAGS_H
#define MP4V2_XTRA_TAGS_H

typedef struct MP4FILETIME_s
{
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} MP4FILETIME;

typedef struct MP4GUID_s {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} MP4GUID;

typedef enum MP4XtraBasicType_e
{
    MP4_XTRA_BT_UNICODE   =  8,
    MP4_XTRA_BT_INT64     = 19,
    MP4_XTRA_BT_FILETIME  = 21,
    MP4_XTRA_BT_GUID      = 72,
} MP4XtraBasicType;

typedef struct MP4XtraTag_s
{
    void* __handle; /* internal use only */

	const char*         name;
	MP4XtraBasicType    type;
	union {
		const wchar_t*  pwstr;
		uint64_t        number;
		MP4GUID         guid;
		MP4FILETIME	    fileTime;
	};
} MP4XtraTag;

//MP4V2_EXPORT const MP4XtraTags* MP4XtraTagsAlloc();
//MP4V2_EXPORT void MP4XtraTagsFetch( const MP4XtraTags* tags, MP4FileHandle hFile );
//MP4V2_EXPORT void MP4XtraTagsStore( const MP4XtraTags* tags, MP4FileHandle hFile );
//MP4V2_EXPORT void MP4XtraTagsFree ( const MP4XtraTags* tags );

MP4V2_EXPORT const MP4XtraTag *MP4XtraTagsGet( MP4FileHandle hFile, const char *name );
MP4V2_EXPORT void MP4XtraTagFree        ( const MP4XtraTag* tag );
MP4V2_EXPORT void MP4XtraTagsSetString  ( MP4FileHandle hFile, const char*, const wchar_t * );
MP4V2_EXPORT void MP4XtraTagsSetInt     ( MP4FileHandle hFile, const char*, const uint64_t * );
MP4V2_EXPORT void MP4XtraTagsSetFileTime( MP4FileHandle hFile, const char*, const MP4FILETIME * );
MP4V2_EXPORT void MP4XtraTagsSetGUID    ( MP4FileHandle hFile, const char*, const MP4GUID * );
MP4V2_EXPORT bool MP4XtraTagsRemove     ( MP4FileHandle hFile, const char* );

#endif /* MP4V2_XTRA_TAGS_H */
