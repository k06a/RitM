#include "AdapterPlugin.h"

using namespace DiplomBukov;

AdapterPlugin::AdapterPlugin(IAdapterCreator * creator)
    : adapterCreator(creator)
{

}

IAdapterCreator * AdapterPlugin::getAdapterCreator()
{
    return adapterCreator;
}

void AdapterPlugin::setAdapterCreator(IAdapterCreator * creator)
{
    adapterCreator = creator;
}