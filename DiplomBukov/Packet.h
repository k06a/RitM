#ifndef PACKET_H
#define PACKET_H

namespace DiplomBukov
{
	struct Packet
	{
		enum PacketPolicy
		{
			Accepted,
			Rejected
		};

		unsigned id;
		unsigned time;
		unsigned size;
		PacketPolicy status;
		unsigned char data[65536+14];

		void * chanel;
		void * network;
		void * transport;
		void * session;
	};
	// struct Packet
}
// namespace DiplomBukov

#endif
