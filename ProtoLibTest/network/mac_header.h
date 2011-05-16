#ifndef MAC_HEADER_H
#define MAC_HEADER_H

#include <stdio.h>
#include "Protocol.h"
#include "i64u64.h"

namespace RitM
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

        mac_addr(const char * str)
        {
            *this = str;
        }

        mac_addr & operator = (const char * str)
        {
            unsigned arr[6];

            if (str[2] == ':')
            {
                sscanf_s(str, "%2x:%2x:%2x:%2x:%2x:%2x",
                         arr + 0, arr + 1, arr + 2,
                         arr + 3, arr + 4, arr + 5);
            }
            else
            if (str[2] == '-')
            {
                sscanf_s(str, "%2x-%2x-%2x-%2x-%2x-%2x",
                         arr + 0, arr + 1, arr + 2,
                         arr + 3, arr + 4, arr + 5);
            }
            else
            {
                sscanf_s(str, "%2x%2x%2x%2x%2x%2x",
                         arr + 0, arr + 1, arr + 2,
                         arr + 3, arr + 4, arr + 5);
            }
            std::copy(arr, arr+6, bytes);
            return *this;
        }

        bool isBroadcast() const
        {
            return (words[0] == 0xffff)
                && (words[1] == 0xffff)
                && (words[2] == 0xffff);
        }

        bool isZero() const
        {
            return (words[0] == 0x0000)
                && (words[1] == 0x0000)
                && (words[2] == 0x0000);
        }

        bool isConcrete() const
        {
            return !isBroadcast() && !isZero();
        }

        static mac_addr broadcast()
        {
            return mac_addr(0xffff,0xffff,0xffff);
        }

        bool operator == (const mac_addr & addr) const
        {
            return (words[0] == addr.words[0])
                && (words[1] == addr.words[1])
                && (words[2] == addr.words[2]);
        }

        bool operator < (const mac_addr & addr) const
        {
            if (words[0] != addr.words[0])
                return (words[0] < addr.words[0]);
            if (words[1] != addr.words[1])
                return (words[1] < addr.words[1]);
            if (words[2] != addr.words[2])
                return (words[2] < addr.words[2]);
            return false;
        }
    };
    #pragma pack(pop)

    #pragma pack(push,1)
    struct mac_header
    {
        mac_addr dst;
        mac_addr src;
        u16be proto;
    };
    #pragma pack(pop)
}
// namespace RitM

#endif // MAC_HEADER_H