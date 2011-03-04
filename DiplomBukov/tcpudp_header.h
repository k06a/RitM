#ifndef TCPUDP_HEADER_H
#define TCPUDP_HEADER_H

#include "i64u64.h"

#pragma pack(push,1)
struct tcpudp_header
{
    u16be src_port;
    u16be dst_port;
};
#pragma pack(pop)

#endif // TCPUDP_HEADER_H