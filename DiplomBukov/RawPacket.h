#ifndef RAWPACKET_H
#define RAWPACKET_H

#include <vector>
#include "IPacket.h"
#include "i64u64.h"
#include "mac_header.h"

namespace DiplomBukov
{
    class RawPacket : public IPacket
    {
        unsigned id_;
        unsigned time_;
        PacketPolicy status_;
        unsigned real_size;
        std::vector<u8> data_;
        IAdapter * adapter_;
        mac_addr src_mac_addr;
        mac_addr dst_mac_addr;

    public:
        RawPacket(int size = 0);
        RawPacket(u8 * ptr, int size);

        virtual void setId(unsigned id);
        virtual unsigned id() const;

        virtual void setTime(unsigned secs);
        virtual unsigned time() const;

        virtual void setStatus(PacketPolicy status);
        virtual PacketPolicy status() const;

        virtual void setData(u8 * ptr, unsigned size);
        virtual u8 & operator [] (unsigned index);
        virtual unsigned size() const;
        virtual u8 * data();

        virtual void setRealSize(unsigned size);
        virtual unsigned realSize() const;

        virtual void setAdapter(IAdapter * ad);
        virtual IAdapter * adapter() const;

        virtual mac_addr & src_mac();
        virtual mac_addr & dst_mac();
    };
    // class RawPacket
}
// namespace DiplomBukov

#endif // RAWPACKET_H