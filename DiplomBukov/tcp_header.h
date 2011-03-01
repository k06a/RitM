#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include "i64u64.h"

namespace DiplomBukov
{
    #pragma pack(push,1)
    struct tcp_header
    {
        u16 src_port;   /* source port */
        u16 dst_port;   /* destination port */
        u32 seq;        /* sequence number */
        u32 ack;        /* acknowledgment number */
        u8  reserved:4; /* (unused) */
        u8  offset:4;   /* data offset */
        
        const int header_size() const
        {
            return (offset << 2);
        }

        struct {
            u8 flags_fin:1;  // 0x01
            u8 flags_syn:1;  // 0x02
            u8 flags_rst:1;  // 0x04
            u8 flags_psh:1;  // 0x08
            u8 flags_ack:1;  // 0x10
            u8 flags_urg:1;  // 0x20
            u8 flags_res2:1; // 0x40
            u8 flags_res1:1; // 0x80
        } flags;

        u16 window_size;    /* window */
        u16 check_sum;      /* checksum */
        u16 urgent_ptr;     /* urgent pointer */

        tcp_header()
        {
            memset(this, 0, sizeof(tcp_header));
        }
    };
    #pragma pack(pop)
}
// namespace DiplomBukov

#endif // TCP_HEADER_H