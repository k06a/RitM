#include "TcpFlagsProcessor.h"
#include <algorithm>

using namespace DiplomBukov;

TcpFlagsProcessor::TcpFlagsProcessor(IProcessorPtr Connector)
    : connectionStatus(CLOSED)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpFlagsProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new TcpFlagsProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TcpFlagsProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    
    typedef tcp_header::flags_struct FLAGS;

    switch (connectionStatus)
    {
        case CLOSED:
            if ((tcp->flags.haveFlags(FLAGS::SYN)) &&
                (packet->direction() == IPacket::ClientToServer))
            {
                connectionStatus = FIRST_SYN;
                if (packet->prevProcessor(Self) != NULL)
                    packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;

        case FIRST_SYN:
            if ((tcp->flags.haveFlags(FLAGS::ACK)) &&
                (packet->direction() == IPacket::ServerToClient))
            {
                connectionStatus = FIRST_ACK;
            } else
                break;
            //break;

        case FIRST_ACK:
            if ((tcp->flags.haveFlags(FLAGS::SYN)) &&
                (packet->direction() == IPacket::ServerToClient))
            {
                connectionStatus = SECOND_SYN;

                if (tcp->flags.haveFlags(FLAGS::SYN + FLAGS::ACK))
                {
                    tcp->flags = FLAGS::SYN + FLAGS::ACK;
                    if (packet->prevProcessor(Self) != NULL)
                        packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
                }
                else
                {
                    tcp->flags = FLAGS::ACK;
                    if (packet->prevProcessor(Self) != NULL)
                        packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);

                    tcp->flags = FLAGS::SYN;
                    if (packet->prevProcessor(Self) != NULL)
                        packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
                }
            }
            break;

        case SECOND_SYN:
            if ((tcp->flags.haveFlags(FLAGS::ACK)) &&
                (packet->direction() == IPacket::ClientToServer))
            {
                connectionStatus = ESTABLISHED;

                if (packet->prevProcessor(Self) != NULL)
                    packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;

        case ESTABLISHED:
            if (tcp->flags.haveFlags(FLAGS::FIN))
            {
                connectionStatus = PRELAST_FIN;
                break;
            }

            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                if (nextProcessor != NULL)
                    nextProcessor->forwardProcess(proto, packet, offset);
            }
            break;

        case PRELAST_FIN:
            {
                if (tcp->flags.haveFlags(FLAGS::ACK))
                {
                    connectionStatus = PRELAST_ACK;
                }
            }
            break;

        case PRELAST_ACK:
            {
                if (tcp->flags.haveFlags(FLAGS::FIN))
                {
                    connectionStatus = LAST_FIN;
                }
            }
            break;

        case LAST_FIN:
            {
                if (tcp->flags.haveFlags(FLAGS::ACK))
                {
                    connectionStatus = CLOSED;
                }
            }
            break;
    }

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpFlagsProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TcpFlagsProcessor::getProcessorName()
{
    return "TcpFlagsProcessor";
}

Protocol TcpFlagsProcessor::getProtocol()
{
    return Protocol::TCP;
}
