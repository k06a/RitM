#include "RawPacket.h"

using namespace DiplomBukov;

RawPacket::RawPacket(int size)
    : id_(0), time_(0)
    , real_size(0), data()
{
}

RawPacket::RawPacket(u8 * ptr, int size)
    : id_(0), time_(0), real_size(0)
    , data(ptr, ptr+size)
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

void RawPacket::setData(u8 * ptr, unsigned size)
{
    std::vector<u8> tmp(ptr, ptr+size);
    data.swap(tmp);    
}

u8 & RawPacket::operator [] (unsigned index)
{
    return data[index];
}

unsigned RawPacket::size() const
{
    return data.size();
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

mac_addr & RawPacket::src_mac()
{
    return src_mac_addr;
}

mac_addr & RawPacket::dst_mac()
{
    return dst_mac_addr;
}