#include "nodeitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "subscriber.h"

NodeItem::NodeItem(QString text)
{
    pressed = false;
    _text = text;
    setFlag (ItemIsMovable);
//    this->setFlag (ItemIsFocusable);
}

QRectF NodeItem::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    if(pressed)
    {
        QPen pen(Qt::red, 3);
        QBrush brush(Qt::yellow);
        painter->setPen(pen);
        painter->setBrush (brush);
        painter->drawRect(rect);
    }
    else
    {
        QPen pen(Qt::darkCyan, 3);
        QBrush brush(Qt::yellow);
        painter->setPen(pen);
        painter->setBrush (brush);
        painter->drawEllipse(rect);
    }
    QString text = _text;
    text.truncate (6);
    painter->drawText (boundingRect (), Qt::AlignCenter, text);
}

void NodeItem::notifySubscribers()
{
    foreach(Subscriber* s, _subscribers)
    {
        s->Update();
    }
}

QString NodeItem::text() const
{
    return _text;
}

void NodeItem::setText(const QString &text)
{
    _text = text;
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent (event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent (event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    this->scene ()->update ();
    notifySubscribers ();
    QGraphicsItem::mouseMoveEvent (event);
}

