#ifndef MAC_HEADER_H
#define MAC_HEADER_H

namespace DiplomBukov
{
	#pragma pack(push,1)
	struct mac_addr
	{
		union
		{
			unsigned  char bytes[6];
			unsigned short words[3];
		};
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