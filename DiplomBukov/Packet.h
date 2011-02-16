#ifndef PACKET_H
#define PACKET_H

namespace DiplomBukov
{
	struct Packet
	{
        enum PacketType
        {
            Ipv4Packet = 65536+14,
            IpFragment = 1514
        };

		enum PacketPolicy
		{
			Accepted,
			Rejected
		};

		unsigned id;
		unsigned time;
		unsigned size;
		PacketPolicy status;
        unsigned char * data;
        unsigned char * mask;

        Packet(PacketType packetType = Ipv4Packet, int id = 0, int size = 0)
            : id(id), time(0), size(size)
            , status(Accepted), data(NULL), mask(NULL)
        {
            data = new unsigned char [packetType];
        }

        Packet(bool maskedPacket)
            : id(id), time(0), size(size)
            , status(Accepted), data(NULL), mask(NULL)
        {
            data = new unsigned char [Ipv4Packet];
            mask = new unsigned char [Ipv4Packet];
            memset(mask, 0, Ipv4Packet);
        }

        int append(int offset, unsigned char * ptr, int sz)
        {
            memcpy(data + offset, ptr, sz);
        }
	};
	// struct Packet
}
// namespace DiplomBukov

#endif
