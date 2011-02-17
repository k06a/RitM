#include "RouterModule.h"

using namespace DiplomBukov;

RouterModule::RouterModule(IRouter * router)
: baseRouter_(router)
{
}

IRouter * RouterModule::baseRouter()
{
    return baseRouter_;
}

void RouterModule::setBaseRouter(IRouter * router)
{
    baseRouter_ = router;
}