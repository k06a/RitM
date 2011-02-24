#ifndef IPACKET_H
#define IPACKET_H

#include <memory>
#include "CommonInclude.h"
#include "i64u64.h"
#include "mac_header.h"

namespace DiplomBukov
{
    class IPacket
    {
    public:
        virtual ~IPacket() {}

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
    // class IPacket

    typedef std::tr1::shared_ptr<IPacket> IPacketPtr;
}
// namespace DiplomBukov

#endif // IPACKET_H
