#ifndef RAWPACKET_H
#define RAWPACKET_H

#include <deque>
#include <vector>
#include "IPacket.h"
#include "i64u64.h"
#include "network/mac_header.h"

namespace DiplomBukov
{
    class RawPacket : public IPacket
    {
        unsigned id_;
        u64 time_;
        PacketPolicy status_;
        unsigned real_size;
        std::vector<u8> data_;
        IAdapter * adapter_;
        std::deque<ProcessorPtr> processors_;
        std::deque<Protocol> protocols_;
        Direction direction_;
        Protocol::NetworkLayer format_;
        mac_addr src_mac_addr;
        mac_addr dst_mac_addr;

    public:
        RawPacket(int size = 0);
        RawPacket(const u8 * ptr, int size);
        RawPacket(const RawPacket & packet);

        virtual PacketPtr CreateCopy() const;

        virtual unsigned id() const;
        virtual void setId(unsigned id);

        virtual u64 time() const;
        virtual void setTime(unsigned secs, unsigned usec);
        virtual void setTime(u64 usecs);


        virtual PacketPolicy status() const;
        virtual void setStatus(PacketPolicy status);

        virtual void setData(u8 * ptr, unsigned size);
        virtual u8 & operator [] (unsigned index);
        virtual unsigned size() const;
        virtual void resize(unsigned sz);
        virtual std::vector<u8>::iterator dataBegin();
        virtual std::vector<u8>::iterator dataEnd();
        virtual void push_front(int length);
        virtual void erase(int p1, int p2);
        virtual void insert(int p, u8 * data, int size);

        virtual unsigned realSize() const;
        virtual void setRealSize(unsigned size);

        virtual IAdapter * adapter() const;
        virtual void setAdapter(IAdapter * ad);

        virtual void addProcessor(ProcessorPtr pro);
        virtual ProcessorPtr processorBefore(ProcessorPtr pro) const;
        virtual bool haveProcessor(ProcessorPtr proc) const;

        virtual const std::deque<Protocol> & protocols() const;
        virtual void addProtocol(Protocol pro);

        virtual Direction direction() const;
        virtual void setDirection(Direction dir);
        virtual bool swapDirection();

        virtual const mac_addr & srcMac() const;
        virtual void setSrcMac(const mac_addr & src);
        
        virtual const mac_addr & dstMac() const;
        virtual void setDstMac(const mac_addr & dst);
        
        virtual const Protocol::NetworkLayer & format() const;
        virtual void setFormat(const Protocol::NetworkLayer & layer);
    };
    // class RawPacket

    typedef SharedPointer<RawPacket>::Type RawPacketPtr;
}
// namespace DiplomBukov

#endif // RAWPACKET_H