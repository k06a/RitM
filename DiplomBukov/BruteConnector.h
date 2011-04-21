#ifndef BRUTECONNECTOR_H
#define BRUTECONNECTOR_H

#include <deque>

#include "AbstractConnector.h"

namespace DiplomBukov
{
	class BruteConnector : public AbstractConnector
	{
    public:
        BruteConnector();
        BruteConnector(const MyDeque & d);
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
    };
	// class BruteConnector

    typedef SharedPointer<BruteConnector>::Type BruteConnectorPtr;
}
// namespace DiplomBukov

#endif // BRUTECONNECTOR_H