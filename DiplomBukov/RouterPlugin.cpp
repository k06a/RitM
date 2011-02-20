#include "RouterPlugin.h"

using namespace DiplomBukov;

RouterPlugin::RouterPlugin(IRouterCreator * creator)
    : routerCreator(creator)
{

}

IRouterCreator * RouterPlugin::getRouterCreator()
{
    return routerCreator;
}

void RouterPlugin::setRouterCreator(IRouterCreator * creator)
{
    routerCreator = creator;
}