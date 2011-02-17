#include "AdapterModule.h"

using namespace DiplomBukov;

AdapterModule::AdapterModule(IAdapter * adapter)
: baseAdapter_(adapter)
{
}

IAdapter * AdapterModule::baseAdapter()
{
    return baseAdapter_;
}

void AdapterModule::setBaseAdapter(IAdapter * adapter)
{
    baseAdapter_ = adapter;
}