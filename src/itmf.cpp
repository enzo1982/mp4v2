#include "impl.h"
#include "libutil.h"

namespace mp4v2 { namespace util { namespace itmf {
    using namespace mp4v2::impl;

///////////////////////////////////////////////////////////////////////////////

namespace {
    typedef map<BasicType,string> BasicTypeToString;
    typedef map<string,BasicType> BasicTypeFromString;

    BasicTypeToString   __basicTypeToString;
    BasicTypeFromString __basicTypeFromString;

    typedef map<Genre,string> GenreToString;
    typedef map<string,Genre> GenreFromString;

    GenreToString   __genreToString;
    GenreFromString __genreFromString;

    // portable static initialization
    class InitConverters {
    public:
        InitConverters();
    };

    InitConverters __initConverters;

    BasicType computeBasicType( const ArtItem& item );
}

///////////////////////////////////////////////////////////////////////////////

ArtItem::ArtItem()
    : type     ( BT_UNDEFINED )
    , buffer   ( NULL )
    , size     ( 0 )
    , autofree ( false )
{
}

///////////////////////////////////////////////////////////////////////////////

ArtItem::~ArtItem()
{
    if( autofree && buffer )
        free( buffer );
}

///////////////////////////////////////////////////////////////////////////////

void
ArtItem::reset()
{
    if( autofree && buffer )
        free( buffer );

    type     = BT_UNDEFINED;
    buffer   = NULL;
    size     = 0;
    autofree = false;
}

///////////////////////////////////////////////////////////////////////////////

ArtList::ArtList()
{
}

///////////////////////////////////////////////////////////////////////////////

ArtList::~ArtList()
{
}

///////////////////////////////////////////////////////////////////////////////

bool
artAdd( MP4FileHandle file, const ArtItem& item )
{
    if( file == MP4_INVALID_FILE_HANDLE )
        return true;
    MP4File& mp4 = *((MP4File*)file);

    const char* const covr_name = "moov.udta.meta.ilst.covr";
    MP4Atom* covr = mp4.FindAtom( covr_name );
    if( !covr ) {
        mp4.AddDescendantAtoms( "moov", "udta.meta.ilst.covr" );

        covr = mp4.FindAtom( covr_name );
        if( !covr )
            return true;
    }

    // use empty data atom if one exists
    MP4Atom* data = NULL;
    uint32_t index = 0;
    const uint32_t atomc = covr->GetNumberOfChildAtoms();
    for( uint32_t i = 0; i < atomc; i++ ) {
        MP4Atom* atom = covr->GetChildAtom( i );

        MP4BytesProperty* metadata = NULL;
        if ( !atom->FindProperty( "data.metadata", (MP4Property**)&metadata ))
            continue;

        if ( metadata->GetCount() )
            continue;

        data = atom;
        index = i;
        break;
    }

    // no empty atom found, create one.
    if( !data ) {
        data = MP4Atom::CreateAtom( "data" );
        covr->AddChildAtom( data );
        index = covr->GetNumberOfChildAtoms() - 1;
    }

    return artSet( file, item, index );
}

///////////////////////////////////////////////////////////////////////////////

bool
artGet( MP4FileHandle file, ArtItem& item, uint32_t index )
{
    item.reset();

    if( file == MP4_INVALID_FILE_HANDLE )
        return true;
    MP4File& mp4 = *((MP4File*)file);

    MP4Atom* covr = mp4.FindAtom( "moov.udta.meta.ilst.covr" );
    if( !covr )
        return true;

    if( !(index < covr->GetNumberOfChildAtoms()) )
        return true;

    MP4Atom* data = covr->GetChildAtom( index );
    if( !data )
        return true;

    MP4BytesProperty* metadata = NULL;
    if ( !data->FindProperty( "data.metadata", (MP4Property**)&metadata ))
        return true;

    metadata->GetValue( &item.buffer, &item.size );
    item.autofree = true;
    item.type = static_cast<BasicType>( data->GetFlags() );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
artList( MP4FileHandle file, ArtList& out )
{
    out.clear();

    if( file == MP4_INVALID_FILE_HANDLE )
        return true;
    MP4File& mp4 = *((MP4File*)file);

    const uint32_t artc = mp4.GetMetadataCoverArtCount();
    out.resize( artc );
    for( uint32_t i = 0; i < artc; i++ )
        artGet( file, out[i], i );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
artRemove( MP4FileHandle file, uint32_t index )
{
    if( file == MP4_INVALID_FILE_HANDLE )
        return true;
    MP4File& mp4 = *((MP4File*)file);

    if( index == numeric_limits<uint32_t>::max() )
        return !mp4.DeleteMetadataAtom( "covr" );

    MP4Atom* covr = mp4.FindAtom( "moov.udta.meta.ilst.covr" );
    if( !covr )
        return true;

    if( !(index < covr->GetNumberOfChildAtoms()) )
        return true;

    MP4Atom* data = covr->GetChildAtom( index );
    if( !data )
        return true;

    covr->DeleteChildAtom( data );
    delete data;

    if( covr->GetNumberOfChildAtoms() == 0 )
        return !mp4.DeleteMetadataAtom( "covr" );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool
artSet( MP4FileHandle file, const ArtItem& item, uint32_t index )
{
    if( file == MP4_INVALID_FILE_HANDLE )
        return true;
    MP4File& mp4 = *((MP4File*)file);

    MP4Atom* covr = mp4.FindAtom( "moov.udta.meta.ilst.covr" );
    if( !covr )
        return true;

    if( !(index < covr->GetNumberOfChildAtoms()) )
        return true;

    MP4Atom* data = covr->GetChildAtom( index );
    if( !data )
        return true;

    MP4BytesProperty* metadata = NULL;
    if ( !data->FindProperty( "data.metadata", (MP4Property**)&metadata ))
        return true;

    // autodetect type if BT_UNDEFINED
    const BasicType final_type = (item.type == BT_UNDEFINED) ? computeBasicType(item) : item.type;

    // set type; note: not really flags due to b0rked atom structure
    data->SetFlags( final_type );
    metadata->SetValue( item.buffer, item.size );

    return false;
}

///////////////////////////////////////////////////////////////////////////////

string
convertBasicType( BasicType value )
{
    string buffer;
    return convertBasicType( value, buffer );
}

///////////////////////////////////////////////////////////////////////////////

string&
convertBasicType( BasicType value, string& buffer )
{
    const BasicTypeToString::const_iterator found = __basicTypeToString.find( value );
    if( found != __basicTypeToString.end() ) {
        buffer = found->second;
        return buffer;
    }

    ostringstream oss;
    oss << "UNDEFINED(" << value << ")";
    buffer = oss.str();
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////

BasicType
convertBasicType( const string& value )
{
    // if number perform enum lookup
    istringstream convert;
    convert.str( value );
    int ivalue;
    if( convert >> ivalue && ivalue > 0 ) {
        const BasicTypeToString::const_iterator found = __basicTypeToString.find( static_cast<BasicType>(ivalue) );
        if( found != __basicTypeToString.end() )
            return found->first;
    }

    // exact match
    const BasicTypeFromString::const_iterator found = __basicTypeFromString.find( value );
    if( found != __basicTypeFromString.end() )
        return found->second;

    // partial match
    int matches = 0;
    BasicType matched = BT_UNDEFINED;

    const BasicTypeFromString::const_iterator ie = __basicTypeFromString.end();
    for( BasicTypeFromString::const_iterator it = __basicTypeFromString.begin(); it != ie; it++ ) {
        if( it->first.find( value ) == 0 ) {
            matches++;
            matched = it->second;
        }
    }

    return (matches == 1) ? matched : BT_UNDEFINED;
}

///////////////////////////////////////////////////////////////////////////////

string
convertGenre( Genre value )
{
    string buffer;
    return convertGenre( value, buffer );
}

///////////////////////////////////////////////////////////////////////////////

string&
convertGenre( Genre value, string& buffer )
{
    const GenreToString::const_iterator found = __genreToString.find( value );
    if( found != __genreToString.end() ) {
        buffer = found->second;
        return buffer;
    }

    ostringstream oss;
    oss << "UNDEFINED(" << value << ")";
    buffer = oss.str();
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////

Genre
convertGenre( const string& value )
{
    // if number perform enum lookup
    istringstream convert;
    convert.str( value );
    int ivalue;
    if( convert >> ivalue && ivalue > 0 ) {
        const GenreToString::const_iterator found = __genreToString.find( static_cast<Genre>(ivalue) );
        if( found != __genreToString.end() )
            return found->first;
    }

    // exact match
    const GenreFromString::const_iterator found = __genreFromString.find( value );
    if( found != __genreFromString.end() )
        return found->second;

    // partial match
    int matches = 0;
    Genre matched = GENRE_UNDEFINED;

    const GenreFromString::const_iterator ie = __genreFromString.end();
    for( GenreFromString::const_iterator it = __genreFromString.begin(); it != ie; it++ ) {
        if( it->first.find( value ) == 0 ) {
            matches++;
            matched = it->second;
        }
    }

    return (matches == 1) ? matched : GENRE_UNDEFINED;
}

///////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////

InitConverters::InitConverters()
{
    struct BasicTypeData {
        BasicType         type;
        const char* const text;
    };

    BasicTypeData basicTypeData[] = {
        { BT_IMPLICIT,   "IMPLICIT" },
        { BT_UTF8,       "UTF8" },
        { BT_UTF16,      "UTF16" },
        { BT_SJIS,       "SJIS" },
        { BT_HTML,       "HTML" },
        { BT_XML,        "XML" },
        { BT_UUID,       "UUID" },
        { BT_ISRC,       "ISRC" },
        { BT_MI3P,       "MI3P" },
        { BT_GIF,        "GIF" },
        { BT_JPEG,       "JPEG" },
        { BT_PNG,        "PNG" },
        { BT_URL,        "URL" },
        { BT_DURATION,   "DURATION" },
        { BT_DATETIME,   "DATETIME" },
        { BT_GENRES,     "GENRES" },
        { BT_INTEGER,    "INTEGER" },
        { BT_RIAA_PA,    "RIAA_PA" },
        { BT_UPC,        "UPC" },
        { BT_BMP,        "BMP" },
        { BT_UNDEFINED,  "" } // must be last
    };

    for( BasicTypeData* p = basicTypeData; p->type != BT_UNDEFINED; p++ ) {
        __basicTypeToString.insert( BasicTypeToString::value_type( p->type, p->text ));
        __basicTypeFromString.insert( BasicTypeFromString::value_type( p->text, p->type ));
    }

    struct GenreData {
        Genre             type;
        const char* const text;
    };

    GenreData genreData[] = {
        { GENRE_BLUES,             "Blues" },
        { GENRE_CLASSIC_ROCK,      "Classic Rock" },
        { GENRE_COUNTRY,           "Country" },
        { GENRE_DANCE,             "Dance" },
        { GENRE_DISCO,             "Disco" },
        { GENRE_FUNK,              "Funk" },
        { GENRE_GRUNGE,            "Grunge" },
        { GENRE_HIP_HOP,           "Hip-Hop" },
        { GENRE_JAZZ,              "Jazz" },
        { GENRE_METAL,             "Metal" },
        { GENRE_NEW_AGE,           "New Age" },
        { GENRE_OLDIES,            "Oldies" },
        { GENRE_OTHER,             "Other" },
        { GENRE_POP,               "Pop" },
        { GENRE_R_AND_B,           "R&B" },
        { GENRE_RAP,               "Rap" },
        { GENRE_REGGAE,            "Reggae" },
        { GENRE_ROCK,              "Rock" },
        { GENRE_TECHNO,            "Techno" },
        { GENRE_INDUSTRIAL,        "Industrial" },
        { GENRE_ALTERNATIVE,       "Alternative" },
        { GENRE_SKA,               "Ska" },
        { GENRE_DEATH_METAL,       "Death Metal" },
        { GENRE_PRANKS,            "Pranks" },
        { GENRE_SOUNDTRACK,        "Soundtrack" },
        { GENRE_EURO_TECHNO,       "Euro-Techno" },
        { GENRE_AMBIENT,           "Ambient" },
        { GENRE_TRIP_HOP,          "Trip-Hop" },
        { GENRE_VOCAL,             "Vocal" },
        { GENRE_JAZZ_FUNK,         "Jazz+Funk" },
        { GENRE_FUSION,            "Fusion" },
        { GENRE_TRANCE,            "Trance" },
        { GENRE_CLASSICAL,         "Classical" },
        { GENRE_INSTRUMENTAL,      "Instrumental" },
        { GENRE_ACID,              "Acid" },
        { GENRE_HOUSE,             "House" },
        { GENRE_GAME,              "Game" },
        { GENRE_SOUND_CLIP,        "Sound Clip" },
        { GENRE_GOSPEL,            "Gospel" },
        { GENRE_NOISE,             "Noise" },
        { GENRE_ALTERNROCK,        "AlternRock" },
        { GENRE_BASS,              "Bass" },
        { GENRE_SOUL,              "Soul" },
        { GENRE_PUNK,              "Punk" },
        { GENRE_SPACE,             "Space" },
        { GENRE_MEDITATIVE,        "Meditative" },
        { GENRE_INSTRUMENTAL_POP,  "Instrumental Pop" },
        { GENRE_INSTRUMENTAL_ROCK, "Instrumental Rock" },
        { GENRE_ETHNIC,            "Ethnic" },
        { GENRE_GOTHIC,            "Gothic" },
        { GENRE_DARKWAVE,          "Darkwave" },
        { GENRE_TECHNO_INDUSTRIAL, "Techno-Industrial" },
        { GENRE_ELECTRONIC,        "Electronic" },
        { GENRE_POP_FOLK,          "Pop-Folk" },
        { GENRE_EURODANCE,         "Eurodance" },
        { GENRE_DREAM,             "Dream" },
        { GENRE_SOUTHERN_ROCK,     "Southern Rock" },
        { GENRE_COMEDY,            "Comedy" },
        { GENRE_CULT,              "Cult" },
        { GENRE_GANGSTA,           "Gangsta" },
        { GENRE_TOP_40,            "Top 40" },
        { GENRE_CHRISTIAN_RAP,     "Christian Rap" },
        { GENRE_POP_FUNK,          "Pop/Funk" },
        { GENRE_JUNGLE,            "Jungle" },
        { GENRE_NATIVE_AMERICAN,   "Native American" },
        { GENRE_CABARET,           "Cabaret" },
        { GENRE_NEW_WAVE,          "New Wave" },
        { GENRE_PSYCHADELIC,       "Psychadelic" },
        { GENRE_RAVE,              "Rave" },
        { GENRE_SHOWTUNES,         "Showtunes" },
        { GENRE_TRAILER,           "Trailer" },
        { GENRE_LO_FI,             "Lo-Fi" },
        { GENRE_TRIBAL,            "Tribal" },
        { GENRE_ACID_PUNK,         "Acid Punk" },
        { GENRE_ACID_JAZZ,         "Acid Jazz" },
        { GENRE_POLKA,             "Polka" },
        { GENRE_RETRO,             "Retro" },
        { GENRE_MUSICAL,           "Musical" },
        { GENRE_ROCK_AND_ROLL,     "Rock & Roll" },
        { GENRE_HARD_ROCK,         "Hard Rock" },
        { GENRE_FOLK,              "Folk" },
        { GENRE_FOLK_ROCK,         "Folk-Rock" },
        { GENRE_NATIONAL_FOLK,     "National Folk" },
        { GENRE_SWING,             "Swing" },
        { GENRE_FAST_FUSION,       "Fast Fusion" },
        { GENRE_BEBOB,             "Bebob" },
        { GENRE_LATIN,             "Latin" },
        { GENRE_REVIVAL,           "Revival" },
        { GENRE_CELTIC,            "Celtic" },
        { GENRE_BLUEGRASS,         "Bluegrass" },
        { GENRE_AVANTGARDE,        "Avantgarde" },
        { GENRE_GOTHIC_ROCK,       "Gothic Rock" },
        { GENRE_PROGRESSIVE_ROCK,  "Progressive Rock" },
        { GENRE_PSYCHEDELIC_ROCK,  "Psychedelic Rock" },
        { GENRE_SYMPHONIC_ROCK,    "Symphonic Rock" },
        { GENRE_SLOW_ROCK,         "Slow Rock" },
        { GENRE_BIG_BAND,          "Big Band" },
        { GENRE_CHORUS,            "Chorus" },
        { GENRE_EASY_LISTENING,    "Easy Listening" },
        { GENRE_ACOUSTIC,          "Acoustic" },
        { GENRE_HUMOUR,            "Humour" },
        { GENRE_SPEECH,            "Speech" },
        { GENRE_CHANSON,           "Chanson" },
        { GENRE_OPERA,             "Opera" },
        { GENRE_CHAMBER_MUSIC,     "Chamber Music" },
        { GENRE_SONATA,            "Sonata" },
        { GENRE_SYMPHONY,          "Symphony" },
        { GENRE_BOOTY_BASS,        "Booty Bass" },
        { GENRE_PRIMUS,            "Primus" },
        { GENRE_PORN_GROOVE,       "Porn Groove" },
        { GENRE_SATIRE,            "Satire" },
        { GENRE_SLOW_JAM,          "Slow Jam" },
        { GENRE_CLUB,              "Club" },
        { GENRE_TANGO,             "Tango" },
        { GENRE_SAMBA,             "Samba" },
        { GENRE_FOLKLORE,          "Folklore" },
        { GENRE_BALLAD,            "Ballad" },
        { GENRE_POWER_BALLAD,      "Power Ballad" },
        { GENRE_RHYTHMIC_SOUL,     "Rhythmic Soul" },
        { GENRE_FREESTYLE,         "Freestyle" },
        { GENRE_DUET,              "Duet" },
        { GENRE_PUNK_ROCK,         "Punk Rock" },
        { GENRE_DRUM_SOLO,         "Drum Solo" },
        { GENRE_A_CAPELLA,         "A capella" },
        { GENRE_EURO_HOUSE,        "Euro-House" },
        { GENRE_DANCE_HALL,        "Dance Hall" },
        { GENRE_NONE,              "<NONE>" },
        { GENRE_UNDEFINED,         "" } // must be last
    };

    for( GenreData* p = genreData; p->type != GENRE_UNDEFINED; p++ ) {
        __genreToString.insert( GenreToString::value_type( p->type, p->text ));
        __genreFromString.insert( GenreFromString::value_type( p->text, p->type ));
    }
}

///////////////////////////////////////////////////////////////////////////////

BasicType
computeBasicType( const ArtItem& item )
{
    struct Header {
        BasicType type;
        string    data;
    };

    static Header headers[] = {
        { BT_BMP,  "\x4d\x42" },
        { BT_GIF,  "GIF87a" },
        { BT_GIF,  "GIF89a" },
        { BT_JPEG, "\xff\xd8\xff\xe0" },
        { BT_PNG,  "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a" },
        { BT_UNDEFINED } // must be last
    };

    Header* found = NULL;
    for( Header* p = headers; p->type != BT_UNDEFINED; p++ ) {
        Header& h = *p;

        if( item.size < h.data.size() )
            continue;

        if( memcmp(h.data.data(), item.buffer, h.data.size()) == 0 ) {
            found = &h;
            break;
        }
    }

    return found ? found->type : BT_IMPLICIT;
}

///////////////////////////////////////////////////////////////////////////////

}}}} // namespace mp4v2::util::itmf::anonymous
