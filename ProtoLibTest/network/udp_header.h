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

    int header_size() const 
    {
        return sizeof(udp_header);
    }

    u16 udp_sum_calc(u8 * buff, u16 len_udp, u32 src_addr_32, u32 dest_addr_32, bool padding = true)
    {
        u8 * src_addr = (u8*)&src_addr_32;
        u8 * dest_addr = (u8*)&dest_addr_32;
        u16 prot_udp = 17;
        
        // make 16 bit words out of every two adjacent 8 bit words and 
        // calculate the sum of all 16 bit words
        u32 sum = 0;
        for (int i = 0; i < len_udp; i+=2)
        {
            u16 word16 = (buff[i]<<8) | buff[i+1];
            sum += (unsigned long)word16;
        }
        if ((len_udp & 1) && padding)
            sum += (buff[len_udp-1] << 8);
        
        // add the UDP pseudo header which contains the IP source and destinationn addresses
        for (int i = 0; i < 4; i+=2)
        {
            u16 word16 = (src_addr[i]<<8) + src_addr[i+1];
            sum += word16;	
        }
        for (int i = 0; i < 4; i+=2)
        {
            u16 word16 = (dest_addr[i]<<8) + dest_addr[i+1];
            sum += word16; 	
        }
        
        // the protocol number and the length of the UDP packet
        sum += prot_udp + len_udp;

        // keep only the last 16 bits of the 32 bit calculated sum and add the carries
        while (sum>>16)
            sum = (sum & 0xFFFF) + (sum >> 16);

        // Take the one's complement of sum
        sum = ~sum;

        return ((u16) sum);
    }

    inline u16 htons(u16 a)
    {
        return (a<<8)|(a>>8);
    }

    u16 lwip_standard_chksum(u8 * octetptr, u16 len)
    {
        u32 acc = 0;
        while (len > 1)
        {
            u16 src = (*octetptr) << 8;
            octetptr++;
            src |= (*octetptr);
            octetptr++;
            acc += src;
            len -= 2;
        }
        
        if (len > 0)
        {
            u16 src = (*octetptr) << 8;
            acc += src;
        }
        
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
        if ((acc & 0xffff0000) != 0)
            acc = (acc >> 16) + (acc & 0x0000ffffUL);

        return acc;
    }

    u16 inet_chksum_pseudo(
        u8 * p,
        u32 src,
        u32 dest,
        u8 proto,
        u16 udp_len)
    {
        u32 acc;
        u8 swapped;

        acc = 0;
        
        acc += lwip_standard_chksum(p, udp_len);
        
        while ((acc >> 16) != 0)
            acc = (acc & 0xffffUL) + (acc >> 16);
            
        acc += (src & 0xffffUL);
        acc += ((src >> 16) & 0xffffUL);
        acc += (dest & 0xffffUL);
        acc += ((dest >> 16) & 0xffffUL);
        acc += (u32)htons((u16)proto);
        acc += (u32)htons(udp_len);

        while ((acc >> 16) != 0)
            acc = (acc & 0xffffUL) + (acc >> 16);
        
        return (u16)~(acc & 0xffffUL);
    }
};
#pragma pack(pop)

#endif // UDP_HEADER_H