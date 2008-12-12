#include "impl.h"

namespace mp4v2 { namespace util { namespace qtff {

///////////////////////////////////////////////////////////////////////////////

namespace {
    class Singleton
    {
    public:
        Singleton()
        {
            supportedCodings.insert( "avc1" );
            supportedCodings.insert( "mp4v" );
        }

        set<string> supportedCodings;
    };
    Singleton* SINGLETON;
}

///////////////////////////////////////////////////////////////////////////////

void
coding_sinit()
{
    SINGLETON = new Singleton();
}

///////////////////////////////////////////////////////////////////////////////

void
coding_sshutdown()
{
    delete SINGLETON;
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
        if( SINGLETON->supportedCodings.find( atom->GetType() ) == SINGLETON->supportedCodings.end() )
            continue;
        coding = atom;
    }

    return coding == NULL;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::qtff
