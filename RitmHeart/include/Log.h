#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>
#include <sstream>
#include "CommonInclude.h"

namespace DiplomBukov
{
    class ILogPrinter
    {
    public:
        ~ILogPrinter() {}

        virtual void printString(const char * str) = 0;
    };
    // class ILogPrinter

    class RITM_API Log
    {
        static ILogPrinter * m_printer;

    private:
        Log();

    public:
        static void setLogPrinter(ILogPrinter * printer);
        static ILogPrinter * logPrinter();

        static void printString(const std::string & str);
    };
    // class Log

    class RITM_API LogLineType
        : public std::ostringstream
    {
    public:
        ~LogLineType();
    };
    // LogLine()


    #define LogLine() (static_cast<LogLineType&>(LogLineType()))
}
// namespace DiplomBukov

#endif // LOG_H
