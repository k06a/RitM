#include "AdapterModule.h"

using namespace DiplomBukov;

AdapterModule::AdapterModule(IAdapter * adapter)
    : baseAdapter(adapter)
{
}

IAdapter * AdapterModule::getBaseAdapter()
{
    return baseAdapter;
}

void AdapterModule::setBaseAdapter(IAdapter * adapter)
{
    baseAdapter = adapter;
}