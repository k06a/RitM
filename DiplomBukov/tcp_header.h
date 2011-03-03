#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include "i64u64.h"
#include "LittleBigEndian.h"

namespace DiplomBukov
{
    #pragma pack(push,1)
    struct tcp_header
    {
        boolib::util::BigEndian<u16> src_port;   /* source port */
        boolib::util::BigEndian<u16> dst_port;   /* destination port */
        u32 seq;        /* sequence number */
        u32 ack;        /* acknowledgment number */
        u8  reserved:4; /* (unused) */
    private:
        u8  offset:4;   /* data offset */
    public:
        const int header_size() const {
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

        //Вычисление контрольной суммы TCP сегмента
        static u16 crc16(u16 * buffer, int length)
        {
            u32 crc = 0;

            // Вычисление CRC
            while (length > 1)
            {
                crc += *buffer++;
                length -= 2;
            }

            if (length)
                crc += *(u8*)buffer;

            // Закончить вычисления
            while (crc >> 16)
                crc = (crc >> 16) + (crc & 0xffff);
            
            return ~crc;
        }

        static u16 countCheckSum(u32 src_addr, u32 dst_addr, char * data, u16 packet_length)
        {
            #pragma pack(push,1)
            struct pseudo_header
            {
                unsigned int src_addr;      // адрес отправителя
                unsigned int dst_addr;      // адрес получателя
                unsigned char zero ;        // начальная установка
                unsigned char proto;        // протокол
                boolib::util::BigEndian<u16> length;   // длина заголовка
            
                #pragma warning(push)
                #pragma warning(disable:4200)
                unsigned char data[];
                #pragma warning(pop)
            };
            #pragma pack(pop)

            int full_length = sizeof(pseudo_header) + packet_length;
            pseudo_header * ph = (pseudo_header *)new char [full_length];

            ph->src_addr = src_addr;
            ph->dst_addr = dst_addr;
            ph->zero = 0;
            ph->proto = 6;  // TCP
            ph->length = packet_length;
            memcpy(ph->data, data, packet_length);

            u16 crc = crc16((u16*)ph, full_length);
            delete [] ph;

            return crc;
        }

        const u16 recountSum(u32 src_ip, u32 dst_ip, int dataLength)
        {
            check_sum = 0;
            check_sum = countCheckSum(src_ip, dst_ip, (char*)this, dataLength);
            return check_sum;
        }
    };
    #pragma pack(pop)
}
// namespace DiplomBukov

#endif // TCP_HEADER_H