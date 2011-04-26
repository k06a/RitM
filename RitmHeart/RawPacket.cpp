#include "RawPacket.h"
#include <algorithm>

using namespace DiplomBukov;

RawPacket::RawPacket(int size)
    : id_(0), time_(0), status_(Accepted)
    , real_size(size), data_(size)
    , direction_(Unknown)

{
}

RawPacket::RawPacket(const u8 * ptr, int size)
    : id_(0), time_(0), status_(Accepted)
    , real_size(size), data_(ptr, ptr+size)
    , direction_(Unknown)
{
}

RawPacket::RawPacket(const RawPacket & packet)
    : id_(packet.id_), time_(packet.time_), status_(packet.status_)
    , real_size(packet.real_size), data_(packet.data_)
    , direction_(packet.direction_)
    , processors_(packet.processors_)
    , protocols_(packet.protocols_)
    , src_mac_addr(packet.src_mac_addr)
    , dst_mac_addr(packet.dst_mac_addr)
    , format_(packet.format_)
{
}


PacketPtr RawPacket::CreateCopy() const
{
    return PacketPtr(new RawPacket(*this));
}

void RawPacket::setId(unsigned id)
{
    id_ = id;
}

unsigned RawPacket::id() const
{
    return id_;
}

u64 RawPacket::time() const
{
    return time_;
}

void RawPacket::setTime(unsigned secs, unsigned usec)
{
    time_ = (((u64)secs) << 32) | usec;
}

void RawPacket::setTime(u64 usecs)
{
    time_ = usecs;
}

void RawPacket::setStatus(PacketPolicy status)
{
    status_ = status;
}

IPacket::PacketPolicy RawPacket::status() const
{
    return status_;
}

void RawPacket::setData(u8 * ptr, unsigned size)
{
    std::vector<u8> tmp(ptr, ptr+size);
    data_.swap(tmp);
    setRealSize(size);
}

u8 & RawPacket::operator [] (unsigned index)
{
    return data_[index];
}

unsigned RawPacket::size() const
{
    return data_.size();
}

void RawPacket::resize(unsigned sz)
{
    data_.resize(sz);
    setRealSize(sz);
}

std::vector<u8>::iterator RawPacket::dataBegin()
{
    return data_.begin();
}

std::vector<u8>::iterator RawPacket::dataEnd()
{
    return data_.end();
}

void RawPacket::push_front(int length)
{
    std::vector<u8> tmp(data_.size() + length, 0);
    std::copy(data_.begin(), data_.end(), tmp.begin() + length);
    data_.swap(tmp);
}

void RawPacket::erase(int p1, int p2)
{
    data_.erase(data_.begin() + p1,
                data_.begin() + p2);
    setRealSize(realSize() - (p2-p1));
}

void RawPacket::insert(int p, u8 * data, int size)
{
    data_.insert(data_.begin() + p,
                 data, data + size);
    setRealSize(realSize() + size);
}

void RawPacket::setRealSize(unsigned size)
{
    real_size = size;
}

unsigned RawPacket::realSize() const
{
    return real_size;
}

void RawPacket::setAdapter(IAdapter * ad)
{
    adapter_ = ad;
}

IAdapter * RawPacket::adapter() const
{
    return adapter_;
}

void RawPacket::addProcessor(ProcessorPtr pro)
{
    processors_.push_back(pro);
}

ProcessorPtr RawPacket::processorBefore(ProcessorPtr pro) const
{
    std::deque<ProcessorPtr>::const_iterator it = 
        std::find(processors_.begin(), processors_.end(), pro);
    if (it == processors_.begin() || it == processors_.end())
        return ProcessorPtr();
    return *(--it);
}

bool RawPacket::haveProcessor(ProcessorPtr proc) const
{
    std::deque<ProcessorPtr>::const_iterator it =
        std::find(processors_.begin(), processors_.end(), proc);
    return (it != processors_.end());
}

void RawPacket::addProtocol(Protocol pro)
{
    protocols_.push_back(pro);
}

const std::deque<Protocol> & RawPacket::protocols() const
{
    return protocols_;
}

void RawPacket::setDirection(IPacket::Direction dir)
{
    direction_ = dir;
}

IPacket::Direction RawPacket::direction() const
{
    return direction_;
}

bool RawPacket::swapDirection()
{
    if (direction_ == IPacket::Unknown)
        return false;

    if (direction_ == IPacket::ClientToServer)
        direction_ = IPacket::ServerToClient;
    else
        direction_ = IPacket::ClientToServer;

    std::swap(src_mac_addr, dst_mac_addr);

    return true;
}

const mac_addr & RawPacket::srcMac() const
{
    return src_mac_addr;
}

const mac_addr & RawPacket::dstMac() const
{
    return dst_mac_addr;
}

const Protocol::NetworkLayer & RawPacket::format() const
{
    return format_;
}

void RawPacket::setSrcMac(const mac_addr & src)
{
    src_mac_addr = src;
}

void RawPacket::setDstMac(const mac_addr & dst)
{
    dst_mac_addr = dst;
}

void RawPacket::setFormat(const Protocol::NetworkLayer & layer)
{
    format_ = layer;
}
