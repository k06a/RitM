#ifndef IPACKET_H
#define IPACKET_H

#include <deque>
#include <memory>
#include <vector>
#include "CommonInclude.h"
#include "network/mac_header.h"

namespace DiplomBukov
{
    class IPacket
    {
    public:
        enum Direction
        {
            Unknown,
            ClientToServer,
            ServerToClient
        };

        enum PacketPolicy
        {
            Accepted,
            Rejected
        };

        virtual ~IPacket() {}

        virtual PacketPtr CreateCopy() const = 0;

        virtual unsigned id() const = 0;
        virtual void setId(unsigned id) = 0;

        virtual u64 time() const = 0;
        virtual void setTime(unsigned secs, unsigned usec) = 0;
        virtual void setTime(u64 usecs) = 0;

        virtual PacketPolicy status() const = 0;
        virtual void setStatus(PacketPolicy status) = 0;

        virtual void setData(u8 * ptr, unsigned size) = 0;
        virtual u8 & operator [] (unsigned index) = 0;
        virtual unsigned size() const = 0;
        virtual void resize(unsigned sz) = 0;
        virtual std::vector<u8>::iterator dataBegin() = 0;
        virtual std::vector<u8>::iterator dataEnd() = 0;
        virtual void push_front(int length) = 0;
        virtual void erase(int p1, int p2) = 0;
        virtual void insert(int p, u8 * data, int size) = 0;

        virtual unsigned realSize() const = 0;
        virtual void setRealSize(unsigned size) = 0;
        
        virtual IAdapter * adapter() const = 0;
        virtual void setAdapter(IAdapter * ad) = 0;
        
        virtual void addProcessor(ProcessorPtr pro) = 0;
        virtual ProcessorPtr processorBefore(ProcessorPtr pro) const = 0;
        virtual bool haveProcessor(ProcessorPtr proc) const = 0;
        
        virtual const std::deque<Protocol> & protocols() const = 0;
        virtual void addProtocol(Protocol pro) = 0;
        
        virtual Direction direction() const = 0;
        virtual void setDirection(Direction dir) = 0;
        virtual bool swapDirection() = 0;

        virtual const mac_addr & srcMac() const = 0;
        virtual void setSrcMac(const mac_addr & src) = 0;
        
        virtual const mac_addr & dstMac() const = 0;
        virtual void setDstMac(const mac_addr & dst) = 0;
        
        virtual const Protocol::NetworkLayer & format() const = 0;
        virtual void setFormat(const Protocol::NetworkLayer & layer) = 0;
    };
    // class IPacket

    typedef std::tr1::shared_ptr<IPacket> PacketPtr;
}
// namespace DiplomBukov

#endif // IPACKET_H
