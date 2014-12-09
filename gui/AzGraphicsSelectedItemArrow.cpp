#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>


AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(QGraphicsScene *scene):mScene(scene){}


QPolygon AzGraphicsSelectedItemArrow::arrowPolygon(){
    QPolygon poligon;
    poligon << QPoint(0,0)
            << QPoint(5,6)
            << QPoint(2,6)
            << QPoint(2,15)
            << QPoint(-2,15)
            << QPoint(-2,6)
            << QPoint(-5,6);
    return poligon;
}

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *){
   //QPoint point(arrowPos(Npos));
    QPoint point(arrowPos(SWpos));
    QPolygon polygone = arrowPolygon();
    polygone.translate(point);
    painter->drawPolygon(polygone);
    //qDebug() <<draw ;
}

QPoint AzGraphicsSelectedItemArrow::arrowPos(SideLight aPos) const {
    QList<QGraphicsItem*> items = mScene->selectedItems();
    if (items.size() == 0)
        return QPoint();
    QGraphicsItem *item = items[0];
    QPointF posi = item->pos();
    QRectF bound = item->boundingRect();
    QPoint coordinateArrow;
    QRectF boundArrow = arrowPolygon().boundingRect();
    switch (aPos) {
    case Npos:
        coordinateArrow =  QPoint(posi.x()+bound.center().x(),posi.y()- boundArrow.height());
        break;
    case Spos:
        coordinateArrow =  QPoint(posi.x()+bound.center().x(),posi.y()+bound.height());
        break;
    case Epos:
        coordinateArrow =  QPoint(posi.x()+bound.width(),posi.y()+bound.center().y());
        break;
    case Wpos:
        coordinateArrow =  QPoint(posi.x(),posi.y()+bound.center().y());
        break;
    case NEpos:
        coordinateArrow =  QPoint(posi.x()+bound.width(),posi.y());
    case NWpos:
        coordinateArrow =  QPoint(posi.x(),posi.y());
    case SEpos:
        coordinateArrow =  QPoint(posi.x()+bound.width(),posi.y()+bound.height());
    case SWpos:
        coordinateArrow =  QPoint(posi.x(),posi.y()+bound.height());
    case NotArrow:
      break;
    };
    return coordinateArrow;
}

