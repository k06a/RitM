#include "Log.h"

using namespace RitM;

//#pragma data_seg(".shared")
ILogPrinter * Log::m_printer = NULL;
//#pragma data_seg() 
//#pragma comment(linker, "/SECTION:.shared,RWS")

Log::Log()
{
}

void Log::setLogPrinter(ILogPrinter * printer)
{
    m_printer = printer;
}

ILogPrinter * Log::logPrinter()
{
    return m_printer;
}

void Log::printString(const std::string & str)
{
    if (m_printer == NULL)
        return;
    m_printer->printString(str.c_str());
}

LogLineType::~LogLineType()
{
    Log::printString(str());
}