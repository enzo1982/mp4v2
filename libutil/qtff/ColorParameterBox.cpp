#include "impl.h"

namespace mp4v2 { namespace util { namespace qtff {

///////////////////////////////////////////////////////////////////////////////

namespace {
    bool findColorParameterBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& colr );

    class Singleton
    {
    public:
        Singleton()
            : BOX_TYPE( "colr" )
        {
        }

        const string BOX_TYPE; // 4-char type code
    };
    Singleton* SINGLETON;
}

///////////////////////////////////////////////////////////////////////////////

void
ColorParameterBox_sinit()
{
    SINGLETON = new Singleton();
}

///////////////////////////////////////////////////////////////////////////////

void
ColorParameterBox_sshutdown()
{
    delete SINGLETON;
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

    colr = MP4Atom::CreateAtom( coding, SINGLETON->BOX_TYPE.c_str() );
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
    : primariesIndex        ( 6 )
    , transferFunctionIndex ( 1 )
    , matrixIndex           ( 6 )
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
    primariesIndex        = 6;
    transferFunctionIndex = 1;
    matrixIndex           = 6;
}

///////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////

bool
findColorParameterBox( MP4FileHandle file, MP4Atom& coding, MP4Atom*& colr )
{
    colr = NULL;

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

} // anonymous

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::qtff
