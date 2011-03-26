#include "ConnectorModule.h"

using namespace DiplomBukov;

ConnectorModule::ConnectorModule(ConnectorPtr Connector)
    : baseConnector(Connector)
{
}

ConnectorPtr ConnectorModule::getBaseConnector()
{
    return baseConnector;
}

void ConnectorModule::setBaseConnector(ConnectorPtr Connector)
{
    baseConnector = Connector;
}