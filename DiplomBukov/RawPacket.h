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
        unsigned real_size;
        std::vector<u8> data;
        IAdapter * adapter_;
        mac_addr src_mac_addr;
        mac_addr dst_mac_addr;

    public:
        RawPacket(int size = 0);
        RawPacket(u8 * ptr, int size);

        virtual void setId(unsigned id) = 0;
        virtual unsigned id() const = 0;

        virtual void setTime(unsigned secs) = 0;
        virtual unsigned time() const = 0;

        virtual void setData(u8 * ptr, unsigned size) = 0;
        virtual u8 & operator [] (unsigned index) = 0;
        virtual unsigned size() const = 0;

        virtual void setRealSize(unsigned size) = 0;
        virtual unsigned realSize() const = 0;

        virtual void setAdapter(IAdapter * ad) = 0;
        virtual IAdapter * adapter() const = 0;

        virtual mac_addr & src_mac() = 0;
        virtual mac_addr & dst_mac() = 0;
    };
    // class RawPacket
}
// namespace DiplomBukov

#endif // RAWPACKET_H