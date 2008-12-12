#include "impl.h"

namespace mp4v2 { namespace util { namespace qtff {

///////////////////////////////////////////////////////////////////////////////

namespace {
    bool findPictureAspectRatioBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& pasp );

    class Singleton
    {
    public:
        Singleton()
            : BOX_TYPE( "pasp" )
        {
        }

        const string BOX_TYPE; // 4-char type code
    };
    Singleton* SINGLETON;
}

///////////////////////////////////////////////////////////////////////////////

void
PictureAspectRatioBox_sinit()
{
    SINGLETON = new Singleton();
}

///////////////////////////////////////////////////////////////////////////////

void
PictureAspectRatioBox_sshutdown()
{
    delete SINGLETON;
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

    pasp = MP4Atom::CreateAtom( SINGLETON->BOX_TYPE.c_str() );
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

bool
findPictureAspectRatioBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& pasp )
{
    pasp = NULL;

    MP4Atom* found = NULL;
    const uint32_t atomc = coding.GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < atomc; i++ ) {
        MP4Atom* atom = coding.GetChildAtom( i );
        if( SINGLETON->BOX_TYPE != atom->GetType() )
            continue;
        found = atom;
    }
    if( !found )
        return true;

    pasp = found;
    return false;
}

///////////////////////////////////////////////////////////////////////////////

}}}} // namespace mp4v2::util::qtff::anonymous
