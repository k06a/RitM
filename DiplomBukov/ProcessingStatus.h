#ifndef PROCESSINGSTATUS_H
#define PROCESSINGSTATUS_H

namespace DiplomBukov
{
	struct ProcessingStatus
	{
		enum ProcessingStatusEnum
		{
			Accepted,
			Rejected
		};

		ProcessingStatusEnum status;
		bool modified;

		ProcessingStatus(ProcessingStatusEnum pse)
			: status(pse), modified(false)
		{
		}

		ProcessingStatus & operator = (const ProcessingStatusEnum & pse)
		{
			status = pse;
            return (*this);
		}

		operator ProcessingStatusEnum() const
		{
			return status;
		}
	};
	// struct ProcessingStatus
}
// namespace DiplomBukov

#endif
