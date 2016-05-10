#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <QString>
#include <QDebug>
#include <QDataStream>

class City {
    QString _name;
public:
    City(){}
    City(QString name) : _name(name){}

    QString name() const;
    void setName(const QString &name);

    bool operator==(const City& rhs) const {
        return _name == rhs.name ();
    }

    bool operator!=(const City& c2) const {
        return _name != c2.name ();
    }

    friend QDebug operator<<(QDebug d, const City& rhs) {
        d << rhs.name ();
        return d;
    }

    friend QDataStream& operator<<(QDataStream &ds, const City& rhs) {
        qDebug() << "is at -< City";
        ds << rhs.name ();
        return ds;
    }

//    define bool operator!=(Animal const& a1, Animal const& a2) { a1.weight != a2.weight; }
};

enum TRANSPORT_MEANS { ROAD, TRAIN, AIR };

class Transport
{
protected:
    int _cost;
    TRANSPORT_MEANS _transportMeans;

public:
    Transport(){}
    int cost() const;
    TRANSPORT_MEANS transportMeans() const;
    virtual QString tpMeansString() const {
        QString d = "";
        switch(transportMeans ()){
            case TRANSPORT_MEANS::AIR:
                d = "Air";
                break;
            case TRANSPORT_MEANS::ROAD:
                d = "Road";
                break;
            case TRANSPORT_MEANS::TRAIN:
                d = "Train";
                break;
        }
        return d;
    }

    bool operator==(const TRANSPORT_MEANS &rhs){
        return _transportMeans == rhs;
    }
    bool operator==(const Transport &rhs){
        return _transportMeans == rhs.transportMeans ();
    }

    friend QDebug operator<<(QDebug d, const Transport& rhs) {
        switch(rhs.transportMeans ()){
            case TRANSPORT_MEANS::AIR:
                d << "Air";
                break;
            case TRANSPORT_MEANS::ROAD:
                d << "Road";
                break;
            case TRANSPORT_MEANS::TRAIN:
                d << "Train";
                break;
        }
        return d;
    }

    friend QDataStream& operator<<(QDataStream &ds, const Transport& rhs){
        qDebug() << "is at >-< transport";
        ds << rhs.transportMeans ();
        ds << rhs.cost ();
        return ds;
    }
};


class Road : public Transport
{
public:
    Road(){_transportMeans = TRANSPORT_MEANS::ROAD;}
    Road(int cost){
        _transportMeans = TRANSPORT_MEANS::ROAD;
        _cost = cost;
    }
    QString tpMeansString () const {
        return "Road";
    }
};

class Train : public Transport
{
public:
    Train(){_transportMeans = TRANSPORT_MEANS::TRAIN;}
    Train(int cost){
        _transportMeans = TRANSPORT_MEANS::TRAIN;
        _cost = cost;
    }
    QString tpMeansString () const { return "Train"; }
};

class Air : public Transport
{
public:
    Air(){_transportMeans = TRANSPORT_MEANS::AIR;}
    Air(int cost){
        _transportMeans = TRANSPORT_MEANS::AIR;
        _cost = cost;
    }
    QString tpMeansString () const { return "Air"; }
};

#endif // TRANSPORT_H
