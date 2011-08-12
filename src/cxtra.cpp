
#include "src/impl.h"
#include "src/itmf/Xtra.h"

#define PRINT_ERROR(e) VERBOSE_ERROR(((MP4File*)hFile)->GetVerbosity(), e->Print());

using namespace mp4v2::impl;

extern "C" {

//const MP4XtraTags*
//MP4XtraTagsAlloc()
//{
//    MP4XtraTags* xtraTags = new MP4XtraTags();
//    return xtraTags;
//}

///////////////////////////////////////////////////////////////////////////////

//void
//MP4XtraTagsFree( const MP4XtraTags* tags )
//{
//    MP4XtraTags* xtraTags = const_cast<MP4XtraTags*>(tags);
//	delete xtraTags;
//}

///////////////////////////////////////////////////////////////////////////////

//void
//MP4XtraTagsFetch( const MP4XtraTags* tags, MP4FileHandle hFile )
//{
//    if( !MP4_IS_VALID_FILE_HANDLE( hFile ))
//        return;
//
//	MP4File& file = *static_cast<MP4File*>(hFile);
//    MP4XtraTags* c = const_cast<MP4XtraTags*>(tags);
//
//    try {
//		list<MP4XtraTag> itags;
//		Xtra::xtraGetItems( *(MP4File*)hFile, itags );
//
//		for (list<MP4XtraTag>::const_iterator it = itags.begin(); it != itags.end(); ++it)
//			c->tags.push_back(MP4XtraTag(*it));
//    }
//    catch( MP4Error* e ) {
//        VERBOSE_ERROR( static_cast<MP4File*>(hFile)->GetVerbosity(), e->Print() );
//        delete e;
//    }
//}

///////////////////////////////////////////////////////////////////////////////

//void
//MP4XtraTagsStore( const MP4XtraTags* tags, MP4FileHandle hFile )
//{
//	MP4File& file = *static_cast<MP4File*>(hFile);
//    MP4XtraTags* c = const_cast<MP4XtraTags*>(tags);
//
//    try {
//		for (list<MP4XtraTag>::const_iterator it = c->tags.begin(); it != c->tags.end(); ++it)
//			Xtra::xtraSetItem(file, &*it);
//    }
//    catch( MP4Error* e ) {
//        VERBOSE_ERROR( static_cast<MP4File*>(hFile)->GetVerbosity(), e->Print() );
//        delete e;
//    }
//}

///////////////////////////////////////////////////////////////////////////////

void MP4XtraTagsSetString(MP4FileHandle hFile, const char *name, const wchar_t *value)
{
    MP4File& file = *static_cast<MP4File*>(hFile);

    try {
        XtraTagProperty prop(MP4_XTRA_BT_UNICODE, wcslen(value) * sizeof(wchar_t),
            (uint8_t*)value);
        Xtra::xtraSetItem(file, name, prop);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed",__FUNCTION__);
    }
}

void MP4XtraTagsSetInt(MP4FileHandle hFile, const char *name, const uint64_t *value)
{
    MP4File& file = *static_cast<MP4File*>(hFile);

    try {
        XtraTagProperty prop(MP4_XTRA_BT_INT64, sizeof(uint64_t), (uint8_t*)value);
        Xtra::xtraSetItem(file, name, prop);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed",__FUNCTION__);
    }
}

void MP4XtraTagsSetFileTime(MP4FileHandle hFile, const char *name, const MP4FILETIME *value)
{
    MP4File& file = *static_cast<MP4File*>(hFile);

    try {
        XtraTagProperty prop(MP4_XTRA_BT_FILETIME, sizeof(MP4FILETIME),
            (uint8_t*)value);
        Xtra::xtraSetItem(file, name, prop);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed",__FUNCTION__);
    }
}

void MP4XtraTagsSetGUID(MP4FileHandle hFile, const char *name, const MP4GUID *value)
{
    MP4File& file = *static_cast<MP4File*>(hFile);

    try {
        XtraTagProperty prop(MP4_XTRA_BT_GUID, sizeof(MP4GUID), (uint8_t*)value);
        Xtra::xtraSetItem(file, name, prop);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed",__FUNCTION__);
    }
}

const MP4XtraTag *MP4XtraTagsGet(MP4FileHandle hFile, const char *name)
{
    MP4File& file = *static_cast<MP4File*>(hFile);

    const XtraTagProperty *prop = NULL;
    try {
        prop = Xtra::xtraGetItem(file, name);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed",__FUNCTION__);
    }

    if( !prop )
        return NULL;

    MP4XtraTag* result = NULL;
    Xtra::XtraTag* tag = NULL;

    try {

        switch( prop->type )
        {

            case MP4_XTRA_BT_UNICODE:
                if( prop->valueSize % sizeof(wchar_t) == 0 )
                    // need to explicitly pass the string length because the string
                    // itself isn't null-terminated
                    tag = new Xtra::XtraTag( name, (const wchar_t *)prop->valueData,
                        prop->valueSize / sizeof(wchar_t) );
                else
                    return NULL;
                break;

            case MP4_XTRA_BT_INT64:
                tag = new Xtra::XtraTag( name, *(const uint64_t *)prop->valueData );
                break;

            case MP4_XTRA_BT_FILETIME:
                tag = new Xtra::XtraTag( name, *(const MP4FILETIME *)prop->valueData );
                break;

            case MP4_XTRA_BT_GUID:
                tag = new Xtra::XtraTag( name, *(const MP4GUID *)prop->valueData );
                break;

        }

        tag->c_alloc( result );
        tag->c_fetch( result );
        return result;
    }
    catch( std::bad_alloc ) {
        // This could be a failure to allocate Xtra::XtraTag or
        // a failure to allocate inside c_alloc.
        mp4v2::impl::log.errorf("%s: memory allocation error", __FUNCTION__);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed", __FUNCTION__ );
    }

    if( result )
        delete result;

    if( tag )
        delete tag;

    return NULL;
}

void MP4XtraTagFree( const MP4XtraTag* tag )
{
    if( !tag || !tag->__handle )
        return;

    Xtra::XtraTag* cpp = static_cast<Xtra::XtraTag*>(tag->__handle);
    MP4XtraTag* c = const_cast<MP4XtraTag*>(tag);

    try {
        cpp->c_free( c );
        delete cpp;
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed", __FUNCTION__ );
    }
}

bool MP4XtraTagsRemove(MP4FileHandle hFile, const char *name)
{
    MP4File& file = *static_cast<MP4File*>(hFile);

    bool modified = false;
    try {
        modified = Xtra::xtraRemoveItem(file, name);
    }
    catch( Exception* x ) {
        mp4v2::impl::log.errorf(*x);
        delete x;
    }
    catch( ... ) {
        mp4v2::impl::log.errorf("%s: failed",__FUNCTION__);
    }
    return modified;
}

} // extern "C"
