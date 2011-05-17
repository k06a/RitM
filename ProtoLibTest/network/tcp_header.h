#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include "i64u64.h"
#include "LittleBigEndian.h"
#include "ipv4_header.h"

namespace RitM
{
    #pragma pack(push,1)
    struct tcp_header
    {
        u16be src_port;   /* source port */
        u16be dst_port;   /* destination port */
        u32be seq;        /* sequence number */
        u32be ack;        /* acknowledgment number */
        u8    reserved:4; /* (unused) */
    
        u8    data_offset:4;   /* data data_offset */
        const int header_size() const
        {
            return (data_offset << 2);
        }
        void set_header_size(const int value)
        {
            data_offset = (value >> 2);
        }

        struct flags_struct
        {
            #define PTR_TO_BYTE(x) (*(u8*)(x))

            enum Flags
            {
                FIN = 0x01,
                SYN = 0x02,
                RST = 0x04,
                PSH = 0x08,
                ACK = 0x10,
                URG = 0x20
            };

            u8 fin:1;  // 0x01
            u8 syn:1;  // 0x02
            u8 rst:1;  // 0x04
            u8 psh:1;  // 0x08
            u8 ack:1;  // 0x10
            u8 urg:1;  // 0x20
            u8 res2:1; // 0x40
            u8 res1:1; // 0x80

            flags_struct(int flags = 0)
            {
                PTR_TO_BYTE(this) = flags;
            }

            flags_struct & operator = (const flags_struct & a)
            {
                PTR_TO_BYTE(this) = PTR_TO_BYTE(&a);
                return *this;
            }

            bool operator == (const flags_struct & a) const
            {
                return PTR_TO_BYTE(this) == PTR_TO_BYTE(&a);
            }

            bool haveFlags(const flags_struct & a)
            {
                return (PTR_TO_BYTE(this) & PTR_TO_BYTE(&a)) == PTR_TO_BYTE(&a);
            }

            #undef PTR_TO_BYTE
        } flags;

        u16be window_size;    /* window */
        u16   check_sum;      /* checksum */
        u16be urgent_ptr;     /* urgent pointer */

        tcp_header()
        {
            memset(this, 0, sizeof(tcp_header));
        }

        u16 CheckSum(u16 *buffer, int size)
        {
            unsigned long cksum = 0;
            while(size > 1)
            {
                cksum += *buffer++;
                size -= sizeof(u16);
            }
            if(size)
                cksum += *(u16*)buffer;

            cksum = (cksum >> 16) + (cksum & 0xffff);
            cksum += (cksum >>16);

            return (u16)(~cksum);
        }

        u16 TcpCheckSum(ipv4_header * iph, tcp_header * tcph, u8 * data, int size)
        {
            struct PSD_HEADER
            {
                ipv4_addr m_daddr;
                ipv4_addr m_saddr;
                u8 m_mbz;
                u8 m_ptcl;
                u16be m_tcpl;
            };

            tcph->check_sum = 0;
            PSD_HEADER psd_header;
            psd_header.m_daddr = iph->dst_data;
            psd_header.m_saddr = iph->src_data;
            psd_header.m_mbz = 0;
            psd_header.m_ptcl = 6;  // TCP
            psd_header.m_tcpl = tcph->header_size() + size;

            char tcpBuf[65536] = {};
            memcpy(tcpBuf,&psd_header,sizeof(PSD_HEADER));
            memcpy(tcpBuf+sizeof(PSD_HEADER),tcph,tcph->header_size());
            if (size > 0)
                memcpy(tcpBuf+sizeof(PSD_HEADER)+tcph->header_size(),data,size);
            if ((sizeof(PSD_HEADER)+tcph->header_size()+size) & 1)
            {
                tcpBuf[sizeof(PSD_HEADER)+tcph->header_size()+size] = 0;
                size++;
            }
            return tcph->check_sum = CheckSum((u16*)tcpBuf,
                sizeof(PSD_HEADER)+tcph->header_size()+size);
        }
    };
    #pragma pack(pop)
}
// namespace RitM

#endif // TCP_HEADER_H