#include "ConnectorPlugin.h"

using namespace DiplomBukov;

ConnectorPlugin::ConnectorPlugin(IConnectorCreator * creator)
    : ConnectorCreator(creator)
{

}

IConnectorCreator * ConnectorPlugin::getConnectorCreator()
{
    return ConnectorCreator;
}

void ConnectorPlugin::setConnectorCreator(IConnectorCreator * creator)
{
    ConnectorCreator = creator;
}