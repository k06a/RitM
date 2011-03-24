#ifndef DNS_HEADER_H
#define DNS_HEADER_H

#include "i64u64.h"

#pragma pack(push,1)
struct dns_header
{
    u16be ID;

    struct flags_struct
    {
        u16 RCODE:4;    // Question-Answer ID
        u16 Z:3;        // Reserved bits
        u16 RA:1;       // 1 in answers mean server can work recursive
        u16 RD:1;       // 1 - Recursive execution
        u16 TC:1;       // 1 - was truncated (wait for TCP transmitting)
        u16 AA:1;       // 1 - Authorized answer
        u16 Opcode:4;   // 0 - Req/Resp, 4 - Notify
        u16 QR:1;       // 0 - Request, 1 - Respounce
    } flags;

    u16be QDCOUNT;      // Number of Questions
    u16be ANCOUNT;      // Number of Answers
    u16be NSCOUNT;      // Number of Authority
    u16be ARCOUNT;      // Number of Additional
};
#pragma pack(pop)

#endif // DNS_HEADER_H