#ifndef TEXTLINEOPTION_H
#define TEXTLINEOPTION_H

#include <deque>
#include <string>

#include "IOption.h"

namespace DiplomBukov
{
    class TextLineOption : public IOption
    {
        std::string label;
        std::string textline;

    public:
        TextLineOption(const std::string & text = "");
        virtual IOptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & text);

        const std::string & getText() const;
        void setText(const std::string & text);
    };
    // class TextLineOption
}
// namespace DiplomBukov

#endif // TEXTLINEOPTION_H