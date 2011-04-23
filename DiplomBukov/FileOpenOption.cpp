#include "FileOpenOption.h"

using namespace DiplomBukov;

FileOpenOption::FileOpenOption(const std::string & ext,
                               const std::string & label)
    : label(label), ext(ext), filename("")
{
}

OptionPtr FileOpenOption::CreateCopy() const
{
    FileOpenOptionPtr ptr(new FileOpenOption(filename));
    ptr->setFilename(getFilename());
    return ptr;
}

const std::string & FileOpenOption::getName() const
{
    return label;
}

void FileOpenOption::setName(const std::string & text)
{
    label = text;
}

void FileOpenOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

const std::string & FileOpenOption::getFilename() const
{
    return filename;
}

void FileOpenOption::setFilename(const std::string & text)
{
    filename = text;
}
