#ifndef ICMP_HEADER_H
#define ICMP_HEADER_H

#include "i64u64.h"

#pragma pack(push,1)
struct icmp_header
{
    u8 type;
    u8 code;
    u16 checksum;
    u16 identifier;
    u16 sequence_number;
};
#pragma pack(pop)

#endif // ICMP_HEADER_H