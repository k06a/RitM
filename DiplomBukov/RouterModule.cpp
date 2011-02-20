#include "RouterModule.h"

using namespace DiplomBukov;

RouterModule::RouterModule(IRouter * router)
    : baseRouter(router)
{
}

IRouter * RouterModule::getBaseRouter()
{
    return baseRouter;
}

void RouterModule::setBaseRouter(IRouter * router)
{
    baseRouter = router;
}