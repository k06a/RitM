#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace DiplomBukov
{
	#pragma pack(push,1)
	struct Protocol
	{
	private:
		unsigned short value;

	public:
		enum ProtocolEnum
		{
			None = 0,
			IP = 0x0800,
			TCP = 6,
			UDP = 18
		};
		
		Protocol(ProtocolEnum proto = None)
			: value((unsigned short)proto)
		{
		}

		operator ProtocolEnum()
		{
			return (ProtocolEnum)value;
		}
	};
	#pragma pack(pop)
	// struct Protocol
}
// namespace DiplomBukov

#endif // PROTOCOL_H