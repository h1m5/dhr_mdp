#include "transport.h"

void Transport::setTransportMeans(int transportMeans)
{
    _transportMeans = TRANSPORT_MEANS(transportMeans);
}

int Transport::cost() const
{
    return _cost;
}

Transport Transport::create(QString type, int cost){
    if(type == "Road") return Road(cost);
    if(type == "Air") return Air(cost);
    if(type == "Train") return Train(cost);
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
