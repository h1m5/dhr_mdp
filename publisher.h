#ifndef PUBLISHER_H
#define PUBLISHER_H
#include <QLinkedList>
#include <QVector>

class Subscriber;
class Publisher
{
public:
    Publisher(){}
    void subscribe(Subscriber* _subscriber)
    {
        _subscribers << _subscriber;
    }

    void unsubscribe(Subscriber* _subscriber)
    {
        _subscribers.removeOne (_subscriber);
    }

    virtual void notifySubscribers() = 0;

protected:
    QLinkedList<Subscriber*> _subscribers;
};

#endif // PUBLISHER_H
