
#include "src/impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

void XtraTagProperty::initData( uint8_t* valueData )
{
    if( valueData ) {
        this->valueData = (uint8_t*)MP4Malloc( valueSize );
        memcpy( this->valueData, valueData, valueSize );
    }
    else
        this->valueData = NULL;
}

XtraTagProperty::XtraTagProperty( uint16_t type, uint32_t valueSize,
                                  uint8_t* valueData )
    : type      ( type )
    , valueSize ( valueSize )
{
    initData( valueData );
}

XtraTagProperty::XtraTagProperty( const XtraTagProperty& prop )
    : type      ( prop.type )
    , valueSize ( prop.valueSize )
{
    initData( prop.valueData );
}

XtraTagProperty::XtraTagProperty()
    : type      ( 0 )
    , valueSize ( 0 )
    , valueData ( 0 )
{
}

XtraTagProperty::~XtraTagProperty()
{
    MP4Free( valueData );
}

void XtraTagProperty::swap( XtraTagProperty& other )
{
    std::swap( type, other.type );
    std::swap( valueSize, other.valueSize );
    std::swap( valueData, other.valueData );
}

XtraTagProperty& XtraTagProperty::operator=( XtraTagProperty other )
{
    swap( other );
    return *this;
    // other is destroyed, releasing the memory
}

///////////////////////////////////////////////////////////////////////////////

MP4XtraAtom::MP4XtraAtom( MP4File &file )
        : MP4Atom( file, "Xtra" )
{
}

void MP4XtraAtom::Set( const char* name, const XtraTagProperty& prop )
{
    tags[name] = prop;
}

const XtraTagProperty* MP4XtraAtom::Get( const char *name )
{
    XtraTagMap::const_iterator it = tags.find( name );
    if( it == tags.end() )
        return NULL;
    else
        return &it->second;
}

bool MP4XtraAtom::Remove( const char *name )
{
    return tags.erase( name ) > 0;
}

void MP4XtraAtom::Read ()
{
    while( m_File.GetPosition() < GetEnd() )
    {
        uint32_t dataSize = m_File.ReadUInt32();
        if( dataSize >= sizeof(uint32_t) * 2 )
        {
            dataSize -= sizeof(uint32_t) * 2;
            if( dataSize > 0 )
            {
                uint32_t idNameSize = m_File.ReadUInt32();
                if( idNameSize > 0 )
                {
                    char* idName = new char[idNameSize + 1];
                    m_File.ReadBytes( (uint8_t *)idName, idNameSize );
                    idName[idNameSize] = '\0';

                    uint32_t versionFlags = m_File.ReadUInt32();
                    assert( versionFlags == 0x00000001 );
                    uint32_t propSize = m_File.ReadUInt32();
                    if( propSize >= sizeof(uint32_t) + sizeof(uint16_t) )
                    {
                        uint32_t propType = m_File.ReadUInt16();
                        uint32_t propValueSize = propSize - sizeof(uint32_t) - sizeof(uint16_t);

                        uint8_t* propDataValue = new uint8_t[propValueSize];
                        m_File.ReadBytes( propDataValue, propValueSize );

                        tags.insert( XtraTagMap::value_type( idName,
                            XtraTagProperty(propType, propValueSize, propDataValue ) ) );

                        delete[] propDataValue;
                        delete[] idName;

                    } else assert( 0 );
                } else assert( 0 );
            } else assert( 0 );
        } else assert( 0 );
    }
}

void MP4XtraAtom::Write()
{
    BeginWrite();

    for( XtraTagMap::const_iterator it = tags.begin(); it != tags.end(); ++it )
    {
        uint16_t propType = it->second.type;
        uint32_t propValueSize = it->second.valueSize;
        const uint8_t *propValueData = it->second.valueData;

        unsigned atomHeaderSize = sizeof(uint32_t) * 2 + it->first.size();
        unsigned propSize = sizeof(uint32_t) + sizeof(uint16_t) + propValueSize;

        // Atom header
        m_File.WriteUInt32( atomHeaderSize + sizeof(uint32_t) + propSize );
        m_File.WriteUInt32( it->first.size() );
        m_File.WriteBytes( (uint8_t *)it->first.c_str(), it->first.size() );

        m_File.WriteUInt32( 0x00000001 );

        // Property
        m_File.WriteUInt32( propSize );
        m_File.WriteUInt16( propType );
        m_File.WriteBytes( const_cast<uint8_t *>(propValueData), propValueSize );
    }
    
    FinishWrite();
}

///////////////////////////////////////////////////////////////////////////////

}
} // namespace mp4v2::impl
