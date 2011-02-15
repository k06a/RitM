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

            // Network layer
			IP  = 0x0008,
            ARP = 0x0608,

            // Transport Layer
			TCP = 6,
			UDP = 18
		};
		
		Protocol(ProtocolEnum proto = None)
			: value((unsigned short)proto)
		{
		}

        Protocol & operator = (ProtocolEnum proto)
        {
            value = (unsigned short)proto;
            return (*this);
        }

        Protocol & operator = (unsigned short proto)
        {
            value = proto;
            return (*this);
        }

		operator ProtocolEnum()
		{
			return (ProtocolEnum)value;
		}

        bool operator < (const Protocol & proto) const
        {
            return value < proto.value;
        }

        bool operator == (const Protocol & proto) const
        {
            return value == proto.value;
        }
	};
	#pragma pack(pop)
	// struct Protocol
}
// namespace DiplomBukov

#endif // PROTOCOL_H