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

    bool reject = false;
    switch (connectionStatus)
    {
        case CLOSED:
            if ((tcp->flags.haveFlags(FLAGS::SYN)) &&
                (packet->direction() == IPacket::ClientToServer))
            {
                connectionStatus = FIRST_SYN;
                if (packet->prevProcessor(Self) != NULL)
                    packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
                if (nextProcessor != NULL)
                    nextProcessor->forwardProcess(proto, packet, offset);
                reject = true;
            }
            break;

        case FIRST_SYN:
            if ((tcp->flags.haveFlags(FLAGS::SYN + FLAGS::ACK)) &&
                (packet->direction() == IPacket::ServerToClient))
            {
                connectionStatus = SECOND_SYNACK;
                if (packet->prevProcessor(Self) != NULL)
                    packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
                if (nextProcessor != NULL)
                    nextProcessor->forwardProcess(proto, packet, offset);
                reject = true;
            }
            break;

        case SECOND_SYNACK:
            if ((tcp->flags.haveFlags(FLAGS::ACK)) &&
                (packet->direction() == IPacket::ClientToServer))
            {
                connectionStatus = ESTABLISHED;
                if (packet->prevProcessor(Self) != NULL)
                    packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);
                reject = true;
            }
            break;

        case ESTABLISHED:
            if (tcp->flags.haveFlags(FLAGS::FIN))
            {
                connectionStatus = PRELAST_FIN;
                //reject = true;
                break;
            }

            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                if (nextProcessor != NULL)
                    nextProcessor->forwardProcess(proto, packet, offset);
                reject = true;
            }
            break;

        case PRELAST_FIN:
            {
                if (tcp->flags.haveFlags(FLAGS::FIN + FLAGS::ACK))
                {
                    connectionStatus = LAST_FINACK;
                    //reject = true;
                }
            }
            break;

        case LAST_FINACK:
            {
                if (tcp->flags.haveFlags(FLAGS::ACK))
                {
                    connectionStatus = CLOSED;
                    //reject = true;
                }
            }
            break;
    }

    if (reject)
        packet->setStatus(IPacket::Rejected);

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
