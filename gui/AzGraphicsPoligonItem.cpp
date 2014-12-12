#include "AzGraphicsPoligonItem.h"
#include <QBrush>
#include <QDebug>

AzGraphicsPoligonItem::AzGraphicsPoligonItem(QGraphicsItem * parent):QGraphicsPolygonItem(parent) {
    QPolygon poligon;
    poligon << QPoint(0,0)
            << QPoint(27,16)
            << QPoint(12,16)
            << QPoint(12,115)
            << QPoint(-25,115)
            << QPoint(-25,16)
            << QPoint(-17,16);
    setPolygon(poligon);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setAcceptHoverEvents(true);
}

QVariant AzGraphicsPoligonItem::itemChange(GraphicsItemChange change, const QVariant & value) {
    return QGraphicsPolygonItem::itemChange(change,value);
}


void AzGraphicsPoligonItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
    setBrush(Qt::yellow);
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void AzGraphicsPoligonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
    setBrush(Qt::white);
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}
