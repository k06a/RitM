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
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
    };
	// class BruteConnector
}
// namespace DiplomBukov

#endif // BRUTECONNECTOR_H