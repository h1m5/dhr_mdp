#ifndef ARCITEM_H
#define ARCITEM_H
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include "subscriber.h"
#include "nodeitem.h"

class ArcItem : public QGraphicsItem, public Subscriber
{
public:
    ArcItem(QString label, NodeItem *source, NodeItem *destionation);

//    QRectF boundingRect () const{ return QRectF(0,0,0,0);}
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void Update();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    NodeItem *_source;
    NodeItem *_destination;

    void setPenColour(QColor colour);
    void resetPen();

protected:
    QString _label;
    QGraphicsLineItem *_lineItem;

    void mousePressEvent (QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    QPolygonF arrowHead;
    QPen _pen;
};

#endif // ARCITEM_H
