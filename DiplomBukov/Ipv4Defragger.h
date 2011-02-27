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
        IPacketPtr pack;
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
            memcpy(pack->data() + offset, ptr, size);
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
		Ipv4Defragger(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
	};
	// class Ipv4Defragger
}
// namespace DiplomBukov

#endif // IPV4DEFRAGPROCESSOR_H