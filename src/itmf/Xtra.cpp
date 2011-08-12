#include "src/impl.h"

namespace mp4v2 { namespace impl { namespace Xtra {

///////////////////////////////////////////////////////////////////////////////

void
XtraTag::c_alloc( MP4XtraTag*& tag )
{
    tag = new MP4XtraTag();
    memset( tag, 0, sizeof(MP4XtraTag) ); // safe: pure C-struct
    tag->__handle = this;
}

///////////////////////////////////////////////////////////////////////////////

void
XtraTag::c_fetch( MP4XtraTag*& tag )
{
    tag->name = name.c_str();
    tag->type = type;

    switch( type )
    {
        case MP4_XTRA_BT_UNICODE:
            tag->pwstr = pwstr->c_str();
            break;
        case MP4_XTRA_BT_INT64:
            tag->number = number;
            break;
        case MP4_XTRA_BT_FILETIME:
            tag->fileTime = fileTime;
            break;
        case MP4_XTRA_BT_GUID:
            tag->guid = guid;
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////

void
XtraTag::c_free( MP4XtraTag*& tag )
{
    MP4XtraTag* c = const_cast<MP4XtraTag*>(tag);

    delete c;

    tag = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void
xtraGetItems( MP4File& file, XtraTagMap &items )
{
    MP4XtraAtom *xtraAtom = (MP4XtraAtom *)file.FindAtom( "moov.udta.Xtra" );
    if( !xtraAtom )
        return;
    items = xtraAtom->tags;
}

const XtraTagProperty*
xtraGetItem( MP4File& file, const char *name )
{
    MP4XtraAtom *xtraAtom = (MP4XtraAtom *)file.FindAtom( "moov.udta.Xtra" );
    if( !xtraAtom )
        return NULL;
    return xtraAtom->Get(name);
}

///////////////////////////////////////////////////////////////////////////////

bool
xtraSetItem( MP4File& file, const char* name, const XtraTagProperty& item )
{
    MP4Atom* xtra = file.FindAtom( "moov.udta.Xtra" );
    if( !xtra ) {
        file.AddDescendantAtoms( "moov", "udta.Xtra" );
        xtra = file.FindAtom( "moov.udta.Xtra" );
        ASSERT( xtra );
    }
    MP4XtraAtom* xtraAtom = (MP4XtraAtom *)xtra;
    xtraAtom->Set(name, item);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

bool
xtraRemoveItem( MP4File& file, const char *name )
{
    MP4Atom* xtra = file.FindAtom( "moov.udta.Xtra" );
    if( !xtra )
        return false;
    MP4XtraAtom* xtraAtom = (MP4XtraAtom *)xtra;
    return xtraAtom->Remove(name);
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::impl::Xtra
