#ifndef IPROCESSOR_H
#define IPROCESSOR_H

#include "CommonInclude.h"
#include "IPacketProcessor.h"

namespace DiplomBukov
{
    class IProcessor : public virtual IPacketProcessor
	{
	public:
		virtual ~IProcessor() {}
        
        // Cast IPacketProcessor* to IProcessor*
        virtual IProcessor * CreateCopy() const = 0;
		
        virtual Protocol getProtocol() = 0;
        virtual const char * getProcessorName() = 0;

        virtual void setModule(IProcessorModule * module) = 0;
        virtual IProcessorModule * getModule() = 0;
	};
	// class IProcessor
}
// namespace DiplomBukov

#endif // IPROCESSOR_H