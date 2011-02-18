#ifndef MAC_HEADER_H
#define MAC_HEADER_H

#include <stdio.h>
#include "Protocol.h"
#include "i64u64.h"

namespace DiplomBukov
{
	#pragma pack(push,1)
	struct mac_addr
	{
		union
		{
			u8  bytes[6];
			u16 words[3];
		};

        mac_addr()
        {
            words[0] = 0x0000;
            words[1] = 0x0000;
            words[2] = 0x0000;
        }

        mac_addr(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f)
        {
            bytes[0] = a; bytes[1] = b; bytes[2] = c;
            bytes[3] = d; bytes[4] = e; bytes[5] = f;
        }

        mac_addr(u16 a, u16 b, u16 c)
        {
            words[0] = a;
            words[1] = b;
            words[2] = c;
        }

        mac_addr(char * str)
        {
            if (str[2] == ':')
            {
                sscanf_s(str, "%2x:%2x:%2x:%2x:%2x:%2x",
                       bytes+0, bytes+1, bytes+2,
                       bytes+3, bytes+4, bytes+5);
            }
            else
            {
                sscanf_s(str, "%2x%2x%2x%2x%2x%2x",
                    bytes+0, bytes+1, bytes+2,
                    bytes+3, bytes+4, bytes+5);
            }
        }

        bool isBroadcast()
        {
            return (words[0] == 0xffff)
                && (words[1] == 0xffff)
                && (words[2] == 0xffff);
        }

        bool isZero()
        {
            return (words[0] == 0x0000)
                && (words[1] == 0x0000)
                && (words[2] == 0x0000);
        }

        static mac_addr broadcast()
        {
            return mac_addr(0xffff,0xffff,0xffff);
        }
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct mac_header
	{
		mac_addr src;
		mac_addr dst;
		Protocol proto;
	};
	#pragma pack(pop)
}
// namespace DiplomBukov

#endif // MAC_HEADER_H