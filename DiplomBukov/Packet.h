#ifndef PACKET_H
#define PACKET_H

namespace DiplomBukov
{
	struct Packet
	{
        static const int Ipv4PacketSize = 65536+14;
            
		enum PacketPolicy
		{
			Accepted,
			Rejected
		};

		unsigned id;
		unsigned time;
        unsigned size;
        unsigned real_size;
        bool lastFragmentReceived;
        PacketPolicy status;
        unsigned char * data;
        unsigned char * mask;

        Packet(int size = Ipv4PacketSize, bool masked = false)
            : id(0), time(0), size(size), real_size(size)
            , status(Accepted), data(NULL), mask(NULL)
            , lastFragmentReceived(false)
        {
            data = new unsigned char [size];
            if (masked)
            {
                mask = new unsigned char [size];
                memset(mask, 0, size);
                real_size = 0;
            }
        }

        void reinitFrom(Packet & packet)
        {
            delete data;
            delete mask;
            memcpy(this, &packet, sizeof(Packet));
            packet.data = NULL;
            packet.mask = NULL;
        }

        ~Packet()
        {
            delete data;
            delete mask;
            data = NULL;
            mask = NULL;
        }

        void destroy()
        {
            delete this;
        }

        void append(int offset, unsigned char * ptr, int sz, bool flag_mf)
        {
            memcpy(data + offset, ptr, sz);
            memset(mask + offset,   1, sz);
            if (!flag_mf)
                lastFragmentReceived = true;
            if ((unsigned)offset + sz > real_size)
                real_size = offset + sz;
        }

        bool finished()
        {
            if (mask == NULL)
                return true;
            if (!lastFragmentReceived)
                return false;
            return (memchr(mask, 0, real_size) == NULL);
        }
	};
	// struct Packet
}
// namespace DiplomBukov

#endif
