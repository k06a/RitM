#ifndef IPV4DEFRAGPROCESSOR_H
#define IPV4DEFRAGPROCESSOR_H

#include <vector>
#include <algorithm>
#include "CommonInclude.h"
#include "RawPacket.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    struct DefragPacket
    {
        PacketPtr pack;
        std::vector<bool> mask;
        bool lastFragmentReceived;

        DefragPacket()
            : pack(new RawPacket(65536))
            , mask(65536, false)
            , lastFragmentReceived(false)
        {
        }

        void append(int offset, u8 * ptr, int size, bool flag_mf)
        {
            std::copy(pack->data().begin() + offset, pack->data().end(), ptr);
            std::fill(mask.begin()+offset, mask.begin()+offset+size, true);

            if (!flag_mf)
                lastFragmentReceived = true;
            if ((unsigned)offset + size > pack->realSize())
                pack->setRealSize(offset + size);
        }

        bool finished()
        {
            if (!lastFragmentReceived)
                return false;
            return (std::find(mask.begin(), mask.end(), false) != mask.end());
        }
    };

	class Ipv4Defragger : public AbstractProcessor
	{
        DefragPacket * fullPacket;
        int ipDataOffset;

	public:
		Ipv4Defragger(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class Ipv4Defragger

    typedef SmartPointer<Ipv4Defragger>::Type Ipv4DefraggerPtr;
}
// namespace DiplomBukov

#endif // IPV4DEFRAGPROCESSOR_H