#ifndef IPV4_HEADER_H
#define IPV4_HEADER_H

#include "LittleBigEndian.h"

namespace DiplomBukov
{
	#pragma pack(push,1)
	struct ipv4_addr
	{
		union
		{
			unsigned  char bytes[4];
			unsigned short words[2];
			unsigned   int dword;
		};

        bool operator < (const ipv4_addr & addr) const
        {
            return dword < addr.dword;
        }
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct ipv4_header
	{
		// ---------------- First DWORD ----------------

		unsigned char hdr_len:4;
        unsigned char version:4;

		struct {
			unsigned char dscp:6;
			unsigned char ect:1;
			unsigned char ce:1;
		} dsfield;

		boolib::util::BigEndian<unsigned short> totalLength;

		// ---------------- Second DWORD ----------------

		unsigned short identificator;

    private:
        unsigned char frag_offset1:5;
    public:
        unsigned char flag_mf:1;
        unsigned char flag_df:1;
        unsigned char flag_rb:1;
	private:
        unsigned char frag_offset2;
	public:
        unsigned short fragmentOffset() {
			return (frag_offset1 << 11) | (frag_offset2 << 3);
		}

		void setFragmentOffset(unsigned short value) {
			frag_offset1 = (value >> 11);
            frag_offset2 = (value >> 3);
		}

		// ---------------- Third DWORD ----------------

		unsigned  char ttl;
		unsigned  char proto;
		unsigned short checksum;

		// ---------------- Fourth DWORD ----------------

		ipv4_addr src_data;

		// ---------------- Fifth DWORD ----------------

		ipv4_addr dst_data;

        // ----------------------------------------------
        #pragma warning(push)
        #pragma warning(disable:4200)
        unsigned char data[];
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

        static unsigned short countCheckSum(char *p1, int iSize)
        {
            unsigned short *usBuf = (unsigned short *)p1;

            unsigned int usChksum = 0;
            while (iSize > 1)
            {
                usChksum += *usBuf++;
                iSize -= sizeof(unsigned short);
            }

            if (iSize)
                usChksum += *(unsigned char*)usBuf;

            usChksum = (usChksum >> 16) + (usChksum & 0xffff);
            usChksum += (usChksum >> 16);

            return (unsigned short)(~usChksum);
        }

		unsigned short recountSum()
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