#include "TcpFlagsProcessor.h"

using namespace DiplomBukov;

TcpFlagsProcessor::TcpFlagsProcessor(IProcessorPtr Connector)
    : connectionStatus(CLOSED)
    , clientExpextedFlags()
    , serverExpextedFlags()
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpFlagsProcessor::CreateCopy() const
{
    return IProcessorPtr(new TcpFlagsProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus TcpFlagsProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    
    tcp_header::flags_struct & expectedFlags =
        (packet->direction() == IPacket::ClientToServer)
        ? clientExpextedFlags : serverExpextedFlags;
    
    switch (connectionStatus)
    {
        case CLOSED:
    	    break;
        case LISTEN:
            break;
        case SYN_SENT:
            break;
        case SYN_RECEIVED:
            break;
        case ESTABLISHED:
            break;
        case FIN_WAIT_1:
            break;
        case CLOSE_WAIT:
            break;
        case FIN_WAIT_2:
            break;
        case LAST_ACK:
            break;
        case TIME_WAIT:
            break;
        case CLOSING:
            break;
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpFlagsProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    memset(tcp, 0, sizeof(tcp_header));

    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(Protocol::TCP, packet, offset);

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
