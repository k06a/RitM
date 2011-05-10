#include <string>
#include <iostream>

#include "ConsoleOptionWalker.h"
#include "IProcessor.h"
#include "IAdapter.h"
#include "Log.h"

using namespace DiplomBukov;

ConsoleOptionWalker::ConsoleOptionWalker()
{

}

void ConsoleOptionWalker::visit(AdapterPtr adap)
{
    OptionPtr opt = adap->getOptions();
    if (opt != NULL)
    {
        std::cout << "[ Options of adapter \""
                  << adap->getProcessorName()
                  << "\" ]" << std::endl;
        opt->visitMe(shared_from_this());
        std::cout << std::endl;
    }
}

void ConsoleOptionWalker::visit(ProcessorPtr proc)
{
    OptionPtr opt = proc->getOptions();
    if (opt != NULL)
    {
        std::cout << "[ Options of processor \""
                  << proc->getProcessorName()
                  << "\" ]" << std::endl;
        opt->visitMe(shared_from_this());
        std::cout << std::endl;
    }
}

void ConsoleOptionWalker::visit(CheckOptionPtr opt)
{
    std::cout << opt->getName() << " [y/n]: ";
    std::string ans;
    std::cin >> ans;
    opt->setChecked((ans == "y") || (ans == "Y"));
}

void ConsoleOptionWalker::visit(SwitchOptionPtr opt)
{
    std::cout << opt->getName() << ":" << std::endl;
    for (int i = 0; i < opt->getTextItems_size(); i++)
        std::cout << i+1 << ". " << opt->getTextItems_item(i) << std::endl;

    int ans;
    do 
    {
        std::cout << "Select one value (1-"
                  << opt->getTextItems_size()
                  << "): ";
        std::cin >> ans;
    } while ((ans < 1) || (ans > opt->getTextItems_size()));

    opt->setSelectedIndex(ans);
}

void ConsoleOptionWalker::visit(ComboOptionPtr opt)
{
    SwitchOptionPtr p = SharedPointerCast<SwitchOption>(shared_from_this());
    visit(p);
}

void ConsoleOptionWalker::visit(IntOptionPtr opt)
{
    i64 ans = 0;
    do 
    {
        std::cout << "Enter integer value for \""
                  << opt->getName() << "\" ("
                  << opt->minValue() << "-"
                  << opt->maxValue() << "): ";
        std::cin >> ans;
    } while ((ans < opt->minValue()) || (ans > opt->maxValue()));

    opt->setIntValue(ans);
}

void ConsoleOptionWalker::visit(TextLineOptionPtr opt)
{
    std::cout << "Enter string value for \""
              << opt->getName() << "\": ";
    std::string ans;
    std::getline(std::cin, ans, '\n');
    opt->setText(ans.c_str());
}

void ConsoleOptionWalker::visit(FileOpenOptionPtr opt)
{
    std::cout << "Enter filename to open for \""
        << opt->getName() << "\": ";
    std::string ans;
    std::getline(std::cin, ans, '\n');
    opt->setFilename(ans.c_str());
}

void ConsoleOptionWalker::visit(FileSaveOptionPtr opt)
{
    std::cout << "Enter filename to save for \""
        << opt->getName() << "\": ";
    std::string ans;
    std::getline(std::cin, ans, '\n');
    opt->setFilename(ans.c_str());
}

void ConsoleOptionWalker::visit(GroupOptionPtr opt)
{
    for (int i = 0; i < opt->options_size(); i++)
        opt->options_item(i)->visitMe(shared_from_this());
}

void ConsoleOptionWalker::visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt)
{
    opt->left->visitMe(shared_from_this());
    opt->right->visitMe(shared_from_this());
}

void ConsoleOptionWalker::visit(OptionPtr opt)
{
    LogLine() << "В обходчике опций ConsoleOptionWalker не реализован обход этого типа опций" << '\n';
}