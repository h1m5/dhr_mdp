#include "arcitem.h"
#include <QPen>
#include <QDebug>
#include <QPainter>

ArcItem::ArcItem(QString label, NodeItem *source, NodeItem *destionation)
//    : _source(source),
//      _destination(destionation)
{
    _source = source;
    _destination = destionation;
    _label = label;
    _lineItem = new QGraphicsLineItem(this);
    _lineItem->setPen (QPen(Qt::black, 2, Qt::SolidLine));
    setFlag (ItemIsMovable);
    _pen = QPen(Qt::darkMagenta);
    _pen.setWidth (3);
}

QRectF ArcItem::boundingRect () const
{
    qreal extra = (_lineItem-> pen().width() + 20) / 2.0;

    return QRectF(_lineItem->line().p1(), QSizeF(_lineItem->line().p2().x() - _lineItem->line().p1().x(),
                                      _lineItem->line().p2().y() - _lineItem->line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath ArcItem::shape () const
{
    QPainterPath path = _lineItem->shape();
    path.addPolygon(arrowHead);
    return path;
}

void ArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF sourcePos = _source->pos ();
    sourcePos += _source->boundingRect ().center ();
    QPointF destPos = _destination->pos ();
    destPos += _destination->boundingRect ().center ();
    //    _lineItem->setLine (QLineF(sourcePos, destPos));
    //    painter->drawLine(line);

    QPainterPath path;
    path.moveTo(sourcePos);
//    path.lineTo(20, 30);
    QPointF midPoint = (sourcePos + destPos)/2;
    path.cubicTo (midPoint, destPos + sourcePos, destPos);
//    path.cubicTo(80, 0, 50, 50, 80, 80);


    painter->setPen (_pen);
//    painter->setBrush (Qt::HorPattern);
    painter->drawPath (path);
    QLineF line(sourcePos, destPos);
    painter->drawText ((midPoint + destPos)/2, _label);

    //    _lineItem->setLine (_source->x (),_source->y (),_destination->x (),_destination->y ());
}

void ArcItem::Update()
{
    update();
}

void ArcItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void ArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    update();
//    QGraphicsItem::mousePressEvent (event);
}

void ArcItem::setPenColour(QColor colour)
{
    _pen.setColor (colour);
}

void ArcItem::resetPen()
{
    _pen.setColor (Qt::darkMagenta);
}

