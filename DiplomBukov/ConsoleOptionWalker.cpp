#include "ConsoleOptionWalket.h"
#include "IProcessor.h"

using namespace DiplomBukov;

void ConsoleOptionWalker::visit(ProcessorPtr proc)
{
    OptionPtr opt = proc->getOptions();
}

void ConsoleOptionWalker::visit(CheckOptionPtr opt)
{

}

void ConsoleOptionWalker::visit(SwitchOptionPtr opt)
{

}

void ConsoleOptionWalker::visit(IntOptionPtr opt)
{

}

void ConsoleOptionWalker::visit(GroupOptionPtr opt)
{

}

void ConsoleOptionWalker::visit(ListOptionPtr<OptionPtr,OptionPtr> opt)
{

}

void ConsoleOptionWalker::visit(OptionPtr opt)
{

}