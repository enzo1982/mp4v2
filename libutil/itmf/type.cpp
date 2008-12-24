#include "impl.h"

namespace mp4v2 { namespace util { namespace itmf {

///////////////////////////////////////////////////////////////////////////////

namespace {
    struct CompareIgnoreCase : less<string>
    {
        bool operator()( const string&, const string& ) const;
    };

    typedef map<BasicType,string> BasicToString;
    typedef map<string,BasicType,CompareIgnoreCase> BasicFromString;

    typedef map<Genre,string> GenreToString;
    typedef map<string,Genre,CompareIgnoreCase> GenreFromString;

    typedef map<StikType,string> StikToString;
    typedef map<string,StikType,CompareIgnoreCase> StikFromString;

    class Singleton
    {
    public:
        Singleton();

        BasicToString   basicToString;
        BasicFromString basicFromString;

        GenreToString   genreToString;
        GenreFromString genreFromString;

        StikToString   stikToStringCompact;
        StikToString   stikToStringFormal;
        StikFromString stikFromString;
    };
    Singleton* SINGLETON;

    struct ImageHeader {
        BasicType type;
        string    data;
    };

    // POD static init does not need singletons
    static ImageHeader IMAGE_HEADERS[] = {
        { BT_BMP,  "\x4d\x42" },
        { BT_GIF,  "GIF87a" },
        { BT_GIF,  "GIF89a" },
        { BT_JPEG, "\xff\xd8\xff\xe0" },
        { BT_PNG,  "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a" },
        { BT_UNDEFINED } // must be last
    };
}

///////////////////////////////////////////////////////////////////////////////

void
type_sinit()
{
    SINGLETON = new Singleton();
}

///////////////////////////////////////////////////////////////////////////////

void
type_sshutdown()
{
    delete SINGLETON;
}

///////////////////////////////////////////////////////////////////////////////

BasicType
computeBasicType( const void* buffer, uint32_t size )
{
    ImageHeader* found = NULL;
    for( ImageHeader* p = IMAGE_HEADERS; p->type != BT_UNDEFINED; p++ ) {
        ImageHeader& h = *p;

        if( size < h.data.size() )
            continue;

        if( memcmp(h.data.data(), buffer, h.data.size()) == 0 ) {
            found = &h;
            break;
        }
    }

    return found ? found->type : BT_IMPLICIT;
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
    const BasicToString::const_iterator found = SINGLETON->basicToString.find( value );
    if( found != SINGLETON->basicToString.end() ) {
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
    int ivalue;
    istringstream iss( value );
    iss >> ivalue;
    if( iss.rdstate() == ios::eofbit ) {
        const BasicToString::const_iterator found = SINGLETON->basicToString.find(
            static_cast<BasicType>(ivalue) );
        if( found != SINGLETON->basicToString.end() )
            return found->first;
    }

    // exact match
    const BasicFromString::const_iterator found = SINGLETON->basicFromString.find( value );
    if( found != SINGLETON->basicFromString.end() )
        return found->second;

    // partial match
    int matches = 0;
    BasicType matched = BT_UNDEFINED;

    const BasicFromString::const_iterator ie = SINGLETON->basicFromString.end();
    for( BasicFromString::const_iterator it = SINGLETON->basicFromString.begin(); it != ie; it++ ) {
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
    const GenreToString::const_iterator found = SINGLETON->genreToString.find( value );
    if( found != SINGLETON->genreToString.end() ) {
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
    int ivalue;
    istringstream iss( value );
    iss >> ivalue;
    if( iss.rdstate() == ios::eofbit ) {
        const GenreToString::const_iterator found = SINGLETON->genreToString.find( static_cast<Genre>(ivalue) );
        if( found != SINGLETON->genreToString.end() )
            return found->first;
    }

    // exact match
    const GenreFromString::const_iterator found = SINGLETON->genreFromString.find( value );
    if( found != SINGLETON->genreFromString.end() )
        return found->second;

    // partial match
    int matches = 0;
    Genre matched = GENRE_UNDEFINED;

    const GenreFromString::const_iterator ie = SINGLETON->genreFromString.end();
    for( GenreFromString::const_iterator it = SINGLETON->genreFromString.begin(); it != ie; it++ ) {
        if( it->first.find( value ) == 0 ) {
            matches++;
            matched = it->second;
        }
    }

    return (matches == 1) ? matched : GENRE_UNDEFINED;
}

///////////////////////////////////////////////////////////////////////////////

string
convertStikType( StikType value, bool formal )
{
    string buffer;
    return convertStikType( value, buffer, formal );
}

///////////////////////////////////////////////////////////////////////////////

string&
convertStikType( StikType value, string& buffer, bool formal )
{
    const StikToString& sts = formal
        ? SINGLETON->stikToStringFormal
        : SINGLETON->stikToStringCompact;

    const StikToString::const_iterator found = sts.find( value );
    if( found != sts.end() ) {
        buffer = found->second;
        return buffer;
    }

    ostringstream oss;
    oss << "UNDEFINED(" << value << ")";
    buffer = oss.str();
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////

StikType
convertStikType( const string& value )
{
    // if number perform enum lookup
    int ivalue;
    istringstream iss( value );
    iss >> ivalue;
    if( iss.rdstate() == ios::eofbit ) {
        StikToString::const_iterator found = SINGLETON->stikToStringCompact.find( static_cast<StikType>(ivalue) );
        if( found != SINGLETON->stikToStringCompact.end() )
            return found->first;
    }

    // exact match
    const StikFromString::const_iterator found = SINGLETON->stikFromString.find( value );
    if( found != SINGLETON->stikFromString.end() )
        return found->second;

    // partial match
    int matches = 0;
    StikType matched = STIK_UNDEFINED;

    const StikFromString::const_iterator ie = SINGLETON->stikFromString.end();
    for( StikFromString::const_iterator it = SINGLETON->stikFromString.begin(); it != ie; it++ ) {
        if( it->first.find( value ) == 0 ) {
            matches++;
            matched = it->second;
        }
    }

    return (matches == 1) ? matched : STIK_UNDEFINED;
}

///////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////

bool
CompareIgnoreCase::operator()( const string& x, const string& y ) const
{
    const string::size_type xlen = x.length();
    const string::size_type ylen = y.length();

    if( xlen < ylen ) {
        for( string::size_type i = 0; i < xlen; i++ ) {
            if( std::toupper(x[i]) < std::toupper(y[i]) )
                return true;
        }
        return true;
    }
    else {
        for( string::size_type i = 0; i < ylen; i++ ) {
            if( std::toupper(x[i]) < std::toupper(y[i]) )
                return true;
        }
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////

Singleton::Singleton()
{
    struct BasicData {
        BasicType         type;
        const char* const text;
    };

    BasicData basicData[] = {
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

    for( BasicData* p = basicData; p->type != BT_UNDEFINED; p++ ) {
        basicToString.insert( BasicToString::value_type( p->type, p->text ));
        basicFromString.insert( BasicFromString::value_type( p->text, p->type ));
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
        genreToString.insert( GenreToString::value_type( p->type, p->text ));
        genreFromString.insert( GenreFromString::value_type( p->text, p->type ));
    }

    struct StikData {
        StikType          type;
        const char* const compact;
        const char* const formal;
    };

    StikData stikData[] = {
        { STIK_MOVIE,       "movie",      "Movie" },
        { STIK_NORMAL,      "normal",     "Normal" },
        { STIK_AUDIOBOOK,   "audiobook",  "Audiobook" },
        { STIK_MUSIC_VIDEO, "musicvideo", "Music Video" },
        { STIK_SHORT_FILM,  "shortfilm",  "Short Film" },
        { STIK_TV_SHOW,     "tvshow",     "TV Show" },
        { STIK_BOOKLET,     "booklet",    "Booklet" },
        { STIK_UNDEFINED,   "", "" } // must be last
    };

    for( StikData* p = stikData; p->type != STIK_UNDEFINED; p++ ) {
        stikToStringCompact.insert( StikToString::value_type( p->type, p->compact ));
        stikToStringFormal.insert( StikToString::value_type( p->type, p->formal ));
        stikFromString.insert( StikFromString::value_type( p->compact, p->type ));
    }
}

///////////////////////////////////////////////////////////////////////////////

}}}} // namespace mp4v2::util::itmf::anonymous
