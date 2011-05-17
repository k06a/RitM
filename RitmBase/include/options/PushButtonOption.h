#ifndef PUSHBUTTONOPTION_H
#define PUSHBUTTONOPTION_H

#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace RitM
{
    class RITM_API IPushButtonOptionListener
    {
    public:
        virtual ~IPushButtonOptionListener() {}

        virtual void clicked() = 0;
    };
    // IPushButtonOptionListener

    //

    class RITM_API PushButtonOption
        : public EnableSharedFromThis<PushButtonOption>::Type
        , public IOption
    {
        std::string label;
        PushButtonOptionListenerPtr listener;

    public:
        PushButtonOption(const char * name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * text);

        virtual void visitMe(OptionWalkerPtr walker);

        PushButtonOptionListenerPtr getListener();
        void setListener(PushButtonOptionListenerPtr someone);

        void click();

        virtual std::string saveToString(int level);
        virtual void loadFromString(std::string str, int level = 0);
    };
    // class PushButtonOption
}
// namespace RitM

#endif // PUSHBUTTONOPTION_H