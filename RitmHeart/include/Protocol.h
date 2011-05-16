#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

namespace DiplomBukov
{
    #pragma pack(push,1)
    struct Protocol
    {
        std::string name;
        int code;

        enum PhysicalLayer
        {
            None = 0,
            Ethernet_II = 0x0001
        };

        static std::string text(PhysicalLayer pro)
        {
            switch (pro)
            {
                case Ethernet_II:   return "Ethernet_II";
                default:            return "";
            }
        }

        enum NetworkLayer
        {
            IPv4 = 0x0800,
            IPv6 = 0x86DD,
            ARP  = 0x0806
        };

        static std::string text(NetworkLayer pro)
        {
            switch (pro)
            {
                case IPv4:  return "IPv4";
                case IPv6:  return "IPv6";
                case ARP:   return "ARP";
                default:    return "";
            }
        }

        enum TransportLayer
        {
            TCP  = 0x06,
            UDP  = 0x11,
            ICMP = 0x01
        };

        static std::string text(TransportLayer pro)
        {
            switch (pro)
            {
                case TCP:   return "TCP";
                case UDP:   return "UDP";
                case ICMP:  return "ICMP";
                default:    return "";
            }
        }

        Protocol(PhysicalLayer proto = None)
            : name(text(proto)), code(proto)
        {
        }

        Protocol(NetworkLayer proto)
            : name(text(proto)), code(proto)
        {
        }

        Protocol(TransportLayer proto)
            : name(text(proto)), code(proto)
        {
        }

        Protocol(const std::string & name, int proto)
            : name(name), code(proto)
        {
        }

        Protocol(const Protocol & proto)
            : name(proto.name), code(proto.code)
        {
        }

        Protocol & operator = (const Protocol & proto)
        {
            name = proto.name;
            code = proto.code;
            return (*this);
        }

        bool operator < (const Protocol & proto) const
        {
            return (name < proto.name);
        }

        bool operator == (const Protocol & proto) const
        {
            return (name == proto.name);
        }

        bool operator != (const Protocol & proto) const
        {
            return (name != proto.name);
        }

        bool operator == (const std::string & proto) const
        {
            return (name == proto);
        }
    };
    #pragma pack(pop)
    // struct Protocol
}
// namespace DiplomBukov

#endif // PROTOCOL_H