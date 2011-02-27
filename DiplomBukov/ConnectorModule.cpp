#include "ConnectorModule.h"

using namespace DiplomBukov;

ConnectorModule::ConnectorModule(IConnectorPtr Connector)
    : baseConnector(Connector)
{
}

IConnectorPtr ConnectorModule::getBaseConnector()
{
    return baseConnector;
}

void ConnectorModule::setBaseConnector(IConnectorPtr Connector)
{
    baseConnector = Connector;
}