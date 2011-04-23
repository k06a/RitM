#ifndef UDP_HEADER_H
#define UDP_HEADER_H

#include "i64u64.h"
#include "LittleBigEndian.h"

#pragma pack(push,1)
struct udp_header
{
    u16be src_port;
    u16be dst_port;
    u16be length;
    u16be checksum;
};
#pragma pack(pop)

#endif // UDP_HEADER_H