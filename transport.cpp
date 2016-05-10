#include "transport.h"

void Transport::setTransportMeans(int transportMeans)
{
    _transportMeans = TRANSPORT_MEANS(transportMeans);
}

int Transport::cost() const
{
    return _cost;
}

TRANSPORT_MEANS Transport::transportMeans() const
{
    return _transportMeans;
}


QString City::name() const
{
    return _name;
}

void City::setName(const QString &name)
{
    _name = name;
}
