#ifndef IPV4_HEADER_H
#define IPV4_HEADER_H

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
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct ipv4_header
	{
		// ---------------- Fisrt DWORD ----------------

		unsigned char version:4;
		unsigned char hdr_len:4;

		struct {
			unsigned char dscp:6;
			unsigned char ect:1;
			unsigned char ce:1;
		} dsfield;

		boolib::util::BigEndian<unsigned short> totalLength;

		// ---------------- Second DWORD ----------------

		unsigned short identificator;

		unsigned short flag_rb:1;
		unsigned short flag_df:1;
		unsigned short flag_mf:1;
	private:
		unsigned short frag_offset:13;
	public:
		unsigned short fragmentOffset() {
			return (frag_offset << 3);
		}
		void setFragmentOffset(unsigned short value) {
			frag_offset = (value >> 3);
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

		static unsigned short countCheckSum(char * buffer, unsigned short length)
		{
			unsigned int sum = 0;

			for (char * end = buffer + length; buffer < end; buffer+=2)
				sum += (*buffer<<8) | *(buffer+1);

			while (sum >> 16)
				sum = (sum & 0xffff) + (sum >> 16);

			return ~((sum >> 8) | (sum << 8));
		}

		unsigned short recountSum()
		{
			checksum = countCheckSum((char*)this, size());
			return checksum;
		}

		ipv4_header()
			: version(4)
			, hdr_len(5)
		{
		}

		virtual int size() const
		{
			return hdr_len*4;
		}
	};
	#pragma pack(pop)
}
// namespace DiplomBukov

#endif // IPV4_HEADER_H