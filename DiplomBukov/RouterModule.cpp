#include "RouterModule.h"

using namespace DiplomBukov;

RouterModule::RouterModule(IRouterPtr router)
    : baseRouter(router)
{
}

IRouterPtr RouterModule::getBaseRouter()
{
    return baseRouter;
}

void RouterModule::setBaseRouter(IRouterPtr router)
{
    baseRouter = router;
}