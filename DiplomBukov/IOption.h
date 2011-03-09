#ifndef IOPTION_H
#define IOPTION_H

namespace DiplomBukov
{
    class IOption
    {
    public:
        virtual ~IOption() {}

        virtual std::string getName() = 0;
        virtual void setName(std::string text) = 0;
    };
}

#endif // IOPTION_H