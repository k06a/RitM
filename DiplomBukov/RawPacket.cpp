#include "RawPacket.h"

using namespace DiplomBukov;

RawPacket::RawPacket(int size)
    : id_(0), time_(0), status_(Accepted)
    , real_size(size), data_(size)
    , direction_(Unknown)

{
}

RawPacket::RawPacket(u8 * ptr, int size)
    : id_(0), time_(0), status_(Accepted)
    , real_size(size), data_(ptr, ptr+size)
    , direction_(Unknown)
{
}

void RawPacket::setId(unsigned id)
{
    id_ = id;
}

unsigned RawPacket::id() const
{
    return id_;
}

void RawPacket::setTime(unsigned secs)
{
    time_ = secs;
}

unsigned RawPacket::time() const
{
    return time_;
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
    std::deque<u8> tmp(ptr, ptr+size);
    data_.swap(tmp);    
}

u8 & RawPacket::operator [] (unsigned index)
{
    return data_[index];
}

unsigned RawPacket::size() const
{
    return data_.size();
}

u8 * RawPacket::data()
{
    return &data_[0];
}

void RawPacket::push_front(int length)
{
    for (int i = 0; i < length; i++)
        data_.push_front(0);
}

void RawPacket::setRealSize(unsigned size)
{
    real_size = size;
}

unsigned RawPacket::realSize() const
{
    return real_size;
}

void RawPacket::setAdapter(IAdapterPtr ad)
{
    adapter_ = ad;
}

IAdapterPtr RawPacket::adapter() const
{
    return adapter_;
}

void RawPacket::addProcessor(IProcessorPtr pro)
{
    processors_.push_back(pro);
}

const std::deque<IProcessorPtr> & RawPacket::processors() const
{
    return processors_;
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

mac_addr & RawPacket::src_mac()
{
    return src_mac_addr;
}

mac_addr & RawPacket::dst_mac()
{
    return dst_mac_addr;
}