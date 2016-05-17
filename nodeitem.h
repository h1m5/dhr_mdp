#ifndef NODEITEM_H
#define NODEITEM_H
#include <QString>
#include <QPainter>
//#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QPoint>
#include "publisher.h"

class NodeItem : public QGraphicsItem, public Publisher
{
public:
    NodeItem(QString text = "");

    QRectF boundingRect () const;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool pressed;

    void notifySubscribers();

    QString text() const;

    void setText(const QString &text);

protected:
    QString _text;
    void mousePressEvent (QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);

    QPointF _lastPos;
};

#endif // NODEITEM_H
