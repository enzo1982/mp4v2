#include "impl.h"
#include "libutil.h"

namespace mp4v2 { namespace util { namespace qtff {
    using namespace mp4v2::impl;

///////////////////////////////////////////////////////////////////////////////

namespace {
    // portable mechanism for static-init
    class SupportedCodings : public set<string>
    {
    public:
        SupportedCodings();
    };
    const SupportedCodings __supportedCodings;

    bool findCoding( MP4FileHandle file, uint16_t trackIndex, MP4Atom*& coding );
    bool findColorParameterBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& colr );
    bool findPictureAspectRatioBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& pasp );
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::add( MP4FileHandle file, uint16_t trackIndex, const Item& item )
{
    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* colr;
    if( !findColorParameterBox( file, *coding, colr ))
        throw new UtilException( "colr-box already exists" );

    colr = MP4Atom::CreateAtom( BOX_TYPE.c_str() );
    coding->AddChildAtom( colr );
    colr->Generate();

    MP4StringProperty*    type;
    MP4Integer16Property* primariesIndex;
    MP4Integer16Property* transferFunctionIndex;
    MP4Integer16Property* matrixIndex;

    if( colr->FindProperty( "colr.colorParameterType", (MP4Property**)&type ))
        type->SetValue( "nclc" );

    if( colr->FindProperty( "colr.primariesIndex", (MP4Property**)&primariesIndex ))
        primariesIndex->SetValue( item.primariesIndex );

    if( colr->FindProperty( "colr.transferFunctionIndex", (MP4Property**)&transferFunctionIndex ))
        transferFunctionIndex->SetValue( item.transferFunctionIndex );

    if( colr->FindProperty( "colr.matrixIndex", (MP4Property**)&matrixIndex ))
        matrixIndex->SetValue( item.matrixIndex );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::add( MP4FileHandle file, MP4TrackId trackId, const Item& item )
{
    MP4File& mp4 = *((MP4File*)file);
    return add( file, mp4.FindTrackIndex( trackId ), item );
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::get( MP4FileHandle file, uint16_t trackIndex, Item& item )
{
    item.reset();

    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* colr;
    if( findColorParameterBox( file, *coding, colr ))
        throw new UtilException( "colr-box not found" );

    MP4Integer16Property* primariesIndex;
    MP4Integer16Property* transferFunctionIndex;
    MP4Integer16Property* matrixIndex;

    if( colr->FindProperty( "colr.primariesIndex", (MP4Property**)&primariesIndex ))
        item.primariesIndex = primariesIndex->GetValue();

    if( colr->FindProperty( "colr.transferFunctionIndex", (MP4Property**)&transferFunctionIndex ))
        item.transferFunctionIndex = transferFunctionIndex->GetValue();

    if( colr->FindProperty( "colr.matrixIndex", (MP4Property**)&matrixIndex ))
        item.matrixIndex = matrixIndex->GetValue();

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::get( MP4FileHandle file, MP4TrackId trackId, Item& item )
{
    MP4File& mp4 = *((MP4File*)file);
    return get( file, mp4.FindTrackIndex( trackId ), item );
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::list( MP4FileHandle file, ItemList& itemList )
{
    itemList.clear();
    MP4File& mp4 = *((MP4File*)file);

    const uint16_t trackc = mp4.GetNumberOfTracks();
    for( uint16_t i = 0; i < trackc; i++) {
        MP4TrackId id = mp4.FindTrackId( i );
        if( id == MP4_INVALID_TRACK_ID )
            continue;

        const char* type = mp4.GetTrackType( id );
        if( !type )
            continue;

        itemList.resize( itemList.size() + 1 );
        IndexedItem& xitem = itemList[itemList.size()-1];

        xitem.trackIndex = i;
        xitem.trackId    = id;

        bool success = false;
        try {
            success = !get( file, i, xitem.item );
        }
        catch( UtilException* x ) {
            delete x;
        }

        if( !success ) {
            itemList.resize( itemList.size() - 1 );
            continue;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::remove( MP4FileHandle file, uint16_t trackIndex )
{
    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* colr;
    if( findColorParameterBox( file, *coding, colr ))
        throw new UtilException( "colr-box not found" );

    coding->DeleteChildAtom( colr );
    delete colr;

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::remove( MP4FileHandle file, MP4TrackId trackId )
{
    MP4File& mp4 = *((MP4File*)file);
    return remove( file, mp4.FindTrackIndex( trackId ));
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::set( MP4FileHandle file, uint16_t trackIndex, const Item& item )
{
    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* colr;
    if( findColorParameterBox( file, *coding, colr ))
        throw new UtilException( "colr-box not found" );

    MP4Integer16Property* primariesIndex;
    MP4Integer16Property* transferFunctionIndex;
    MP4Integer16Property* matrixIndex;

    if( colr->FindProperty( "colr.primariesIndex", (MP4Property**)&primariesIndex ))
        primariesIndex->SetValue( item.primariesIndex );

    if( colr->FindProperty( "colr.transferFunctionIndex", (MP4Property**)&transferFunctionIndex ))
        transferFunctionIndex->SetValue( item.transferFunctionIndex );

    if( colr->FindProperty( "colr.matrixIndex", (MP4Property**)&matrixIndex ))
        matrixIndex->SetValue( item.matrixIndex );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
ColorParameterBox::set( MP4FileHandle file, MP4TrackId trackId, const Item& item )
{
    MP4File& mp4 = *((MP4File*)file);
    return set( file, mp4.FindTrackIndex( trackId ), item );
}

///////////////////////////////////////////////////////////////////////////////

ColorParameterBox::IndexedItem::IndexedItem()
    : trackIndex ( numeric_limits<uint16_t>::max() )
    , trackId    ( MP4_INVALID_TRACK_ID )
{
}

///////////////////////////////////////////////////////////////////////////////

ColorParameterBox::Item::Item()
    : primariesIndex        ( 1 )
    , transferFunctionIndex ( 1 )
    , matrixIndex           ( 1 )
{
}

///////////////////////////////////////////////////////////////////////////////

void
ColorParameterBox::Item::convertFromCSV( const string& text )
{
    istringstream iss( text );
    char delim;

    iss >> primariesIndex;
    iss >> delim;
    iss >> transferFunctionIndex;
    iss >> delim;
    iss >> matrixIndex;

    // input was good if we end up with only eofbit set
    if( iss.rdstate() != ios::eofbit ) {
        reset();
        ostringstream xss;
        xss << "invalid ColorParameterBox format"
            << " (expecting: INDEX1,INDEX2,INDEX3)"
            << " got: " << text;
        throw new UtilException( xss );
    }
}

///////////////////////////////////////////////////////////////////////////////

string
ColorParameterBox::Item::convertToCSV() const
{
    string buffer;
    return convertToCSV( buffer );
}

///////////////////////////////////////////////////////////////////////////////

string&
ColorParameterBox::Item::convertToCSV( string& buffer ) const
{
    ostringstream oss;
    oss << primariesIndex << ',' << transferFunctionIndex << ',' << matrixIndex;
    buffer = oss.str();
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////

void
ColorParameterBox::Item::reset()
{
    primariesIndex        = 1;
    transferFunctionIndex = 1;
    matrixIndex           = 1;
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::add( MP4FileHandle file, uint16_t trackIndex, const Item& item )
{
    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* pasp;
    if( !findPictureAspectRatioBox( file, *coding, pasp ))
        throw new UtilException( "pasp-box already exists" );

    pasp = MP4Atom::CreateAtom( BOX_TYPE.c_str() );
    coding->AddChildAtom( pasp );
    pasp->Generate();

    MP4Integer16Property* hSpacing;
    MP4Integer16Property* vSpacing;

    if( pasp->FindProperty( "pasp.hSpacing", (MP4Property**)&hSpacing ))
        hSpacing->SetValue( item.hSpacing );

    if( pasp->FindProperty( "pasp.vSpacing", (MP4Property**)&vSpacing ))
        vSpacing->SetValue( item.vSpacing );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::add( MP4FileHandle file, MP4TrackId trackId, const Item& item )
{
    MP4File& mp4 = *((MP4File*)file);
    return add( file, mp4.FindTrackIndex( trackId ), item );
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::get( MP4FileHandle file, uint16_t trackIndex, Item& item )
{
    item.reset();

    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* pasp;
    if( findPictureAspectRatioBox( file, *coding, pasp ))
        throw new UtilException( "pasp-box not found" );

    MP4Integer16Property* hSpacing;
    MP4Integer16Property* vSpacing;

    if( pasp->FindProperty( "pasp.hSpacing", (MP4Property**)&hSpacing ))
        item.hSpacing = hSpacing->GetValue();

    if( pasp->FindProperty( "pasp.vSpacing", (MP4Property**)&vSpacing ))
        item.vSpacing = vSpacing->GetValue();

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::get( MP4FileHandle file, MP4TrackId trackId, Item& item )
{
    MP4File& mp4 = *((MP4File*)file);
    return get( file, mp4.FindTrackIndex( trackId ), item );
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::list( MP4FileHandle file, ItemList& itemList )
{
    itemList.clear();
    MP4File& mp4 = *((MP4File*)file);

    const uint16_t trackc = mp4.GetNumberOfTracks();
    for( uint16_t i = 0; i < trackc; i++) {
        MP4TrackId id = mp4.FindTrackId( i );
        if( id == MP4_INVALID_TRACK_ID )
            continue;

        const char* type = mp4.GetTrackType( id );
        if( !type )
            continue;

        itemList.resize( itemList.size() + 1 );
        IndexedItem& xitem = itemList[itemList.size()-1];

        xitem.trackIndex = i;
        xitem.trackId    = id;

        bool success = false;
        try {
            success = !get( file, i, xitem.item );
        }
        catch( UtilException* x ) {
            delete x;
        }

        if( !success ) {
            itemList.resize( itemList.size() - 1 );
            continue;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::remove( MP4FileHandle file, uint16_t trackIndex )
{
    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* pasp;
    if( findPictureAspectRatioBox( file, *coding, pasp ))
        throw new UtilException( "pasp-box not found" );

    coding->DeleteChildAtom( pasp );
    delete pasp;

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::remove( MP4FileHandle file, MP4TrackId trackId )
{
    MP4File& mp4 = *((MP4File*)file);
    return remove( file, mp4.FindTrackIndex( trackId ));
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::set( MP4FileHandle file, uint16_t trackIndex, const Item& item )
{
    MP4Atom* coding;
    if( findCoding( file, trackIndex, coding ))
        throw new UtilException( "supported coding not found" );

    MP4Atom* pasp;
    if( findPictureAspectRatioBox( file, *coding, pasp ))
        throw new UtilException( "pasp-box not found" );

    MP4Integer16Property* hSpacing;
    MP4Integer16Property* vSpacing;

    if( pasp->FindProperty( "pasp.hSpacing", (MP4Property**)&hSpacing ))
        hSpacing->SetValue( item.hSpacing );

    if( pasp->FindProperty( "pasp.vSpacing", (MP4Property**)&vSpacing ))
        vSpacing->SetValue( item.vSpacing );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
PictureAspectRatioBox::set( MP4FileHandle file, MP4TrackId trackId, const Item& item )
{
    MP4File& mp4 = *((MP4File*)file);
    return set( file, mp4.FindTrackIndex( trackId ), item );
}

///////////////////////////////////////////////////////////////////////////////

PictureAspectRatioBox::IndexedItem::IndexedItem()
    : trackIndex ( numeric_limits<uint16_t>::max() )
    , trackId    ( MP4_INVALID_TRACK_ID )
{
}

///////////////////////////////////////////////////////////////////////////////

PictureAspectRatioBox::Item::Item()
    : hSpacing ( 1 )
    , vSpacing ( 1 )
{
}

///////////////////////////////////////////////////////////////////////////////

void
PictureAspectRatioBox::Item::reset()
{
    hSpacing = 1;
    vSpacing = 1;
}

///////////////////////////////////////////////////////////////////////////////

void
PictureAspectRatioBox::Item::convertFromCSV( const string& text )
{
    istringstream iss( text );
    char delim;

    iss >> hSpacing;
    iss >> delim;
    iss >> vSpacing;

    // input was good if we end up with only eofbit set
    if( iss.rdstate() != ios::eofbit ) {
        reset();
        ostringstream xss;
        xss << "invalid PcitureAspectRatioBox format"
            << " (expecting: hSpacing,vSpacing)"
            << " got: " << text;
        throw new UtilException( xss );
    }
}

///////////////////////////////////////////////////////////////////////////////

string
PictureAspectRatioBox::Item::convertToCSV() const
{
    string buffer;
    return convertToCSV( buffer );
}

///////////////////////////////////////////////////////////////////////////////

string&
PictureAspectRatioBox::Item::convertToCSV( string& buffer ) const
{
    ostringstream oss;
    oss << hSpacing << ',' << vSpacing;
    buffer = oss.str();
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////

SupportedCodings::SupportedCodings()
{
    insert( "avc1" );
    insert( "mp4v" );
}

///////////////////////////////////////////////////////////////////////////////

bool
findCoding( MP4FileHandle file, uint16_t trackIndex, MP4Atom*& coding )
{
    coding = NULL;
    MP4File& mp4 = *((MP4File*)file);

    if( trackIndex == numeric_limits<uint16_t>::max() ) {
        ostringstream xss;
        xss << "invalid track-index: " << trackIndex;
        throw new UtilException( xss );
    }

    ostringstream oss;
    oss << "moov.trak[" << trackIndex << "].mdia.hdlr";
    MP4Atom* hdlr = mp4.FindAtom( oss.str().c_str() );
    if( !hdlr )
        throw new UtilException( "media handler not found" );

    MP4StringProperty* handlerType;
    if( !hdlr->FindProperty( "hdlr.handlerType", (MP4Property**)&handlerType ))
        throw new UtilException( "media handler type-property not found" );

    const string video = "vide";
    if( video != handlerType->GetValue() )
        throw new UtilException( "video-track required" );

    oss.str( "" );
    oss.clear();
    oss << "moov.trak[" << trackIndex << "].mdia.minf.stbl.stsd";
    MP4Atom* stsd = mp4.FindAtom( oss.str().c_str() );
    if( !stsd )
        throw new UtilException( "media handler type-property not found" );

    // find first atom which is a supported coding
    const uint32_t atomc = stsd->GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < atomc; i++ ) {
        MP4Atom* atom = stsd->GetChildAtom( i );
        if( __supportedCodings.find( atom->GetType() ) == __supportedCodings.end() )
            continue;
        coding = atom;
    }

    return coding == NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool
findColorParameterBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& colr )
{
    colr = NULL;

    MP4Atom* found = NULL;
    const uint32_t atomc = coding.GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < atomc; i++ ) {
        MP4Atom* atom = coding.GetChildAtom( i );
        if( ColorParameterBox::BOX_TYPE != atom->GetType() )
            continue;
        found = atom;
    }
    if( !found )
        return true;

    MP4StringProperty* type;
    if( !found->FindProperty( "colr.colorParameterType", (MP4Property**)&type ))
        return true;

    const string type_nclc = "nclc";
    if( type_nclc != type->GetValue() )
        return true;

    colr = found;
    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
findPictureAspectRatioBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& pasp )
{
    pasp = NULL;

    MP4Atom* found = NULL;
    const uint32_t atomc = coding.GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < atomc; i++ ) {
        MP4Atom* atom = coding.GetChildAtom( i );
        if( PictureAspectRatioBox::BOX_TYPE != atom->GetType() )
            continue;
        found = atom;
    }
    if( !found )
        return true;

    pasp = found;
    return false;
}

///////////////////////////////////////////////////////////////////////////////

} // anonymous

///////////////////////////////////////////////////////////////////////////////

const string ColorParameterBox::BOX_TYPE     = "colr";
const string PictureAspectRatioBox::BOX_TYPE = "pasp";

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::qtff
