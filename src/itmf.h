#ifndef MP4V2_IMPL_ITMF_H
#define MP4V2_IMPL_ITMF_H

/// @namespace mp4v2::util::itmf (private) iTunes Metadata Format.
/// <b>WARNING: THIS IS A PRIVATE NAMESPACE. NOT FOR PUBLIC CONSUMPTION.</b>
///
/// This namespace implements some features that are specified by the
/// iTunes Metadata Format Specification, revision 2008-04-16.
///
namespace mp4v2 { namespace util { namespace itmf {
    using namespace std;

///////////////////////////////////////////////////////////////////////////////

/// Basic set of types as detailed in spec.
enum BasicType {
    BT_UNDEFINED = -1,

    BT_IMPLICIT  = 0,   ///< for use with tags for which no type needs to be indicated
    BT_UTF8      = 1,   ///< without any count or null terminator
    BT_UTF16     = 2,   ///< also known as UTF-16BE
    BT_SJIS      = 3,   ///< deprecated unless it is needed for special Japanese characters
    BT_HTML      = 6,   ///< the HTML file header specifies which HTML version
    BT_XML       = 7,   ///< the XML header must identify the DTD or schemas
    BT_UUID      = 8,   ///< also known as GUID; stored as 16 bytes in binary (valid as an ID)
    BT_ISRC      = 9,   ///< stored as UTF-8 text (valid as an ID)
    BT_MI3P      = 10,  ///< stored as UTF-8 text (valid as an ID)
    BT_GIF       = 12,  ///< (deprecated) a GIF image
    BT_JPEG      = 13,  ///< a JPEG image
    BT_PNG       = 14,  ///< a PNG image
    BT_URL       = 15,  ///< absolute, in UTF-8 characters
    BT_DURATION  = 16,  ///< in milliseconds, 32-bit integer
    BT_DATETIME  = 17,  ///< in UTC, counting seconds since midnight, January 1, 1904; 32 or 64-bits
    BT_GENRES    = 18,  ///< a list of enumerated values, see #Genre
    BT_INTEGER   = 21,  ///< a signed big-endian integer with length one of { 1,2,3,4,8 } bytes
    BT_RIAA_PA   = 24,  ///< RIAA parental advisory; { -1=no, 1=yes, 0=unspecified }, 8-bit ingteger
    BT_UPC       = 25,  ///< Universal Product Code, in text UTF-8 format (valid as an ID)
    BT_BMP       = 27,  ///< Windows bitmap image
};

///////////////////////////////////////////////////////////////////////////////

/// enumerated genre as defined in ID3v1 specification but +1 as per iTMF spec.
/// Note values beyond 80 are Winamp extensions.
enum Genre {
    GENRE_UNDEFINED          = 0,

    /* ID3v1 standard */
    GENRE_BLUES              = 1,
    GENRE_CLASSIC_ROCK       = 2,
    GENRE_COUNTRY            = 3,
    GENRE_DANCE              = 4,
    GENRE_DISCO              = 5,
    GENRE_FUNK               = 6,
    GENRE_GRUNGE             = 7,
    GENRE_HIP_HOP            = 8,
    GENRE_JAZZ               = 9,
    GENRE_METAL              = 10,
    GENRE_NEW_AGE            = 11,
    GENRE_OLDIES             = 12,
    GENRE_OTHER              = 13,
    GENRE_POP                = 14,
    GENRE_R_AND_B            = 15,
    GENRE_RAP                = 16,
    GENRE_REGGAE             = 17,
    GENRE_ROCK               = 18,
    GENRE_TECHNO             = 19,
    GENRE_INDUSTRIAL         = 20,
    GENRE_ALTERNATIVE        = 21,
    GENRE_SKA                = 22,
    GENRE_DEATH_METAL        = 23,
    GENRE_PRANKS             = 24,
    GENRE_SOUNDTRACK         = 25,
    GENRE_EURO_TECHNO        = 26,
    GENRE_AMBIENT            = 27,
    GENRE_TRIP_HOP           = 28,
    GENRE_VOCAL              = 29,
    GENRE_JAZZ_FUNK          = 30,
    GENRE_FUSION             = 31,
    GENRE_TRANCE             = 32,
    GENRE_CLASSICAL          = 33,
    GENRE_INSTRUMENTAL       = 34,
    GENRE_ACID               = 35,
    GENRE_HOUSE              = 36,
    GENRE_GAME               = 37,
    GENRE_SOUND_CLIP         = 38,
    GENRE_GOSPEL             = 39,
    GENRE_NOISE              = 40,
    GENRE_ALTERNROCK         = 41,
    GENRE_BASS               = 42,
    GENRE_SOUL               = 43,
    GENRE_PUNK               = 44,
    GENRE_SPACE              = 45,
    GENRE_MEDITATIVE         = 46,
    GENRE_INSTRUMENTAL_POP   = 47,
    GENRE_INSTRUMENTAL_ROCK  = 48,
    GENRE_ETHNIC             = 49,
    GENRE_GOTHIC             = 50,
    GENRE_DARKWAVE           = 51,
    GENRE_TECHNO_INDUSTRIAL  = 52,
    GENRE_ELECTRONIC         = 53,
    GENRE_POP_FOLK           = 54,
    GENRE_EURODANCE          = 55,
    GENRE_DREAM              = 56,
    GENRE_SOUTHERN_ROCK      = 57,
    GENRE_COMEDY             = 58,
    GENRE_CULT               = 59,
    GENRE_GANGSTA            = 60,
    GENRE_TOP_40             = 61,
    GENRE_CHRISTIAN_RAP      = 62,
    GENRE_POP_FUNK           = 63,
    GENRE_JUNGLE             = 64,
    GENRE_NATIVE_AMERICAN    = 65,
    GENRE_CABARET            = 66,
    GENRE_NEW_WAVE           = 67,
    GENRE_PSYCHADELIC        = 68,
    GENRE_RAVE               = 69,
    GENRE_SHOWTUNES          = 70,
    GENRE_TRAILER            = 71,
    GENRE_LO_FI              = 72,
    GENRE_TRIBAL             = 73,
    GENRE_ACID_PUNK          = 74,
    GENRE_ACID_JAZZ          = 75,
    GENRE_POLKA              = 76,
    GENRE_RETRO              = 77,
    GENRE_MUSICAL            = 78,
    GENRE_ROCK_AND_ROLL      = 79,

    /* Winamp extension */
    GENRE_HARD_ROCK          = 80,
    GENRE_FOLK               = 81,
    GENRE_FOLK_ROCK          = 82,
    GENRE_NATIONAL_FOLK      = 83,
    GENRE_SWING              = 84,
    GENRE_FAST_FUSION        = 85,
    GENRE_BEBOB              = 86,
    GENRE_LATIN              = 87,
    GENRE_REVIVAL            = 88,
    GENRE_CELTIC             = 89,
    GENRE_BLUEGRASS          = 90,
    GENRE_AVANTGARDE         = 91,
    GENRE_GOTHIC_ROCK        = 92,
    GENRE_PROGRESSIVE_ROCK   = 93,
    GENRE_PSYCHEDELIC_ROCK   = 94,
    GENRE_SYMPHONIC_ROCK     = 95,
    GENRE_SLOW_ROCK          = 96,
    GENRE_BIG_BAND           = 97,
    GENRE_CHORUS             = 98,
    GENRE_EASY_LISTENING     = 99,
    GENRE_ACOUSTIC           = 100,
    GENRE_HUMOUR             = 101,
    GENRE_SPEECH             = 102,
    GENRE_CHANSON            = 103,
    GENRE_OPERA              = 104,
    GENRE_CHAMBER_MUSIC      = 105,
    GENRE_SONATA             = 106,
    GENRE_SYMPHONY           = 107,
    GENRE_BOOTY_BASS         = 108,
    GENRE_PRIMUS             = 109,
    GENRE_PORN_GROOVE        = 110,
    GENRE_SATIRE             = 111,
    GENRE_SLOW_JAM           = 112,
    GENRE_CLUB               = 113,
    GENRE_TANGO              = 114,
    GENRE_SAMBA              = 115,
    GENRE_FOLKLORE           = 116,
    GENRE_BALLAD             = 117,
    GENRE_POWER_BALLAD       = 118,
    GENRE_RHYTHMIC_SOUL      = 119,
    GENRE_FREESTYLE          = 120,
    GENRE_DUET               = 121,
    GENRE_PUNK_ROCK          = 122,
    GENRE_DRUM_SOLO          = 123,
    GENRE_A_CAPELLA          = 124,
    GENRE_EURO_HOUSE         = 125,
    GENRE_DANCE_HALL         = 126,

    GENRE_NONE               = 255,
};

///////////////////////////////////////////////////////////////////////////////

/// Object representing one cover-art item.
/// This correlates to a covr->data atom and offers automatic memory freeing
/// when <b>autofree</b> is true.
class MP4V2_EXPORT ArtItem
{
public:
    ArtItem();
    ~ArtItem();

    /// Reset to state of newly constructed object.
    /// If <b>buffer</b> is not NULL and <b>autofree</b> is true the
    /// buffer will be free'd.
    void reset();

    BasicType type;     ///< cover-art type.
    uint8_t*  buffer;   ///< buffer point to raw cover-art data.
    uint32_t  size;     ///< size of cover-art buffer in bytes.
    bool      autofree; ///< when true invoke free(buffer) upon destruction.
};

///////////////////////////////////////////////////////////////////////////////

/// Object representing a list of cover-art items.
class MP4V2_EXPORT ArtList : public vector<ArtItem>
{
public:
    ArtList();
    ~ArtList();
};

///////////////////////////////////////////////////////////////////////////////

/// Convert #BasicType to string.
MP4V2_EXPORT string
convertBasicType( BasicType value );

/// Convert #BasicType to string with buffer.
MP4V2_EXPORT string&
convertBasicType( BasicType value, string& buffer );

/// Convert string to #BasicType.
MP4V2_EXPORT BasicType
convertBasicType( const string& value );

///////////////////////////////////////////////////////////////////////////////

/// Convert #Genre to string.
MP4V2_EXPORT string
convertGenre( Genre value );

/// Convert #Genre to string with buffer.
MP4V2_EXPORT string&
convertGenre( Genre value, string& buffer );

/// Convert string to #Genre.
MP4V2_EXPORT Genre
convertGenre( const string& value );

///////////////////////////////////////////////////////////////////////////////

/// Fetch list of cover-art items from file.
///
/// @param file on which to operate.
/// @param out vector of #ArtItem objects.
///
/// @return <b>true</b> on failure, <b>false</b> on success.

MP4V2_EXPORT bool
artList( MP4FileHandle file, ArtList& out );

/// Add cover-art item to file.
/// Any necessary metadata atoms are first created.
/// Additionally, if an empty data-atom exists it will be used,
/// otherwise a new data-atom is added to <b>covr-atom</b>.
///
/// @param file on which to operate.
/// @param item cover-art object to place in file.
///
/// @return <b>true</b> on failure, <b>false</b> on success.
MP4V2_EXPORT bool
artAdd( MP4FileHandle file, const ArtItem& item );

/// Remove cover-art item from file.
///
/// @param file on which to operate.
/// @param index 0-based index of cover-art to remove.
///     Default value indicates wildcard behavior to remove all items.
///
/// @return <b>true</b> on failure, <b>false</b> on success.
MP4V2_EXPORT bool
artRemove( MP4FileHandle file, uint32_t index = numeric_limits<uint32_t>::max() );

/// Fetch cover-art item from file.
///
/// @param file on which to operate.
/// @param item cover-art object populated with data.
///     The resulting object owns the malloc'd buffer and <b>item.autofree</b>
///     is set to true for convenient memory management.
/// @param index 0-based index of cover-art to fetch.
///
/// @return <b>true</b> on failure, <b>false</b> on success.
MP4V2_EXPORT bool
artGet( MP4FileHandle file, ArtItem& item, uint32_t index );

/// Replace cover-art item in file.
///
/// @param file on which to operate.
/// @param item cover-art object to place in file.
/// @param index 0-based index of cover-art to replace.
///
/// @return <b>true</b> on failure, <b>false</b> on success.
MP4V2_EXPORT bool
artSet( MP4FileHandle file, const ArtItem& item, uint32_t index );

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::itmf

#endif // MP4V2_IMPL_ITMF_H
