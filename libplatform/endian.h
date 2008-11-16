#ifndef MP4V2_PLATFORM_ENDIAN_H
#define MP4V2_PLATFORM_ENDIAN_H

///////////////////////////////////////////////////////////////////////////////

#define MP4V2_BYTESWAP_16(x) \
    mp4v2::platform::uint16_t( ((mp4v2::platform::uint16_t(x) & 0xff00) >> 8) \
                             | ((mp4v2::platform::uint16_t(x) & 0x00ff) << 8) )
                           
#define MP4V2_BYTESWAP_32(x) \
    mp4v2::platform::uint32_t( ((mp4v2::platform::uint32_t(x) & 0xff000000) >> 24) \
                             | ((mp4v2::platform::uint32_t(x) & 0x00ff0000) >>  8) \
                             | ((mp4v2::platform::uint32_t(x) & 0x0000ff00) <<  8) \
                             | ((mp4v2::platform::uint32_t(x) & 0x000000ff) << 24) )

#define MP4V2_BYTESWAP_64(x) \
    mp4v2::platform::uint64_t( ((mp4v2::platform::uint64_t(x) & 0xff00000000000000) >> 56) \
                             | ((mp4v2::platform::uint64_t(x) & 0x00ff000000000000) >> 40) \
                             | ((mp4v2::platform::uint64_t(x) & 0x0000ff0000000000) >> 24) \
                             | ((mp4v2::platform::uint64_t(x) & 0x000000ff00000000) >>  8) \
                             | ((mp4v2::platform::uint64_t(x) & 0x00000000ff000000) <<  8) \
                             | ((mp4v2::platform::uint64_t(x) & 0x0000000000ff0000) << 24) \
                             | ((mp4v2::platform::uint64_t(x) & 0x000000000000ff00) << 40) \
                             | ((mp4v2::platform::uint64_t(x) & 0x00000000000000ff) << 56) )
                           
///////////////////////////////////////////////////////////////////////////////

#if defined( __BIG_ENDIAN__ )
#   define MP4V2_HTONS(x)  (x)
#   define MP4V2_HTONL(x)  (x)
#   define MP4V2_NTOHS(x)  (x)
#   define MP4V2_NTOHL(x)  (x)
#else
#   define MP4V2_HTONS(x)  MP4V2_BYTESWAP_16(x)
#   define MP4V2_HTONL(x)  MP4V2_BYTESWAP_32(x)
#   define MP4V2_NTOHS(x)  MP4V2_BYTESWAP_16(x)
#   define MP4V2_NTOHL(x)  MP4V2_BYTESWAP_32(x)
#endif

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_PLATFORM_ENDIAN_H
