#ifndef IPV4_HEADER_H
#define IPV4_HEADER_H

#include "LittleBigEndian.h"
#include "i64u64.h"

namespace DiplomBukov
{
	#pragma pack(push,1)
	struct ipv4_addr
	{
		union
		{
			u8  bytes[4];
			u16 words[2];
			u32 dword;
		};

        bool operator < (const ipv4_addr & addr) const
        {
            return (dword < addr.dword);
        }
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct ipv4_header
	{
		// ---------------- First DWORD ----------------

		u8 hdr_len:4;
        u8 version:4;

		struct {
			u8 dscp:6;
			u8 ect:1;
			u8 ce:1;
		} dsfield;

		boolib::util::BigEndian<u16> totalLength;

		// ---------------- Second DWORD ----------------

		u16 identificator;

    private:
        u8 frag_offset1:5;
    public:
        u8 flag_mf:1;
        u8 flag_df:1;
        u8 flag_rb:1;
	private:
        u8 frag_offset2;
	public:
        u16 fragmentOffset() {
			return (frag_offset1 << 11) | (frag_offset2 << 3);
		}

		void setFragmentOffset(u16 value) {
			frag_offset1 = (value >> 11);
            frag_offset2 = (value >> 3);
		}

		// ---------------- Third DWORD -----------------

		u8 ttl;
		u8 proto;
		u16 checksum;

		// ---------- Fourth and Fifth DWORD ------------

		ipv4_addr src_data;
		ipv4_addr dst_data;

        // ----------------------------------------------
        #pragma warning(push)
        #pragma warning(disable:4200)
        u8 data[];
        #pragma warning(pop)
        // ----------------------------------------------

        /*
		static unsigned short CalculateChecksum(char * buffer, unsigned short length)
		{
			unsigned int sum = 0;

			for (char * end = buffer + length; buffer < end; buffer += 2)
				sum += (*buffer<<8) | buffer[1];

			while (sum >> 16)
				sum = (sum & 0xffff) + (sum >> 16);

            sum = (sum >> 8) | (sum << 8);

            return ~sum;
		}
        */

        static u16 countCheckSum(char *p1, i32 iSize)
        {
            u16 *usBuf = (u16 *)p1;

            u32 usChksum = 0;
            while (iSize > 1)
            {
                usChksum += *usBuf++;
                iSize -= sizeof(u16);
            }

            if (iSize)
                usChksum += *(u8*)usBuf;

            usChksum = (usChksum >> 16) + (usChksum & 0xffff);
            usChksum += (usChksum >> 16);

            return (u8)(~usChksum);
        }

		u16 recountSum()
		{
            checksum = 0;
			checksum = countCheckSum((char*)this, size());
			return checksum;
		}

		const int size() const
		{
			return (hdr_len << 2);
		}

        ipv4_header()
            : version(4), hdr_len(5)
        {
        }
	};
	#pragma pack(pop)
}
// namespace DiplomBukov

#endif // IPV4_HEADER_H