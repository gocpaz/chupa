#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtMath>


AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(QGraphicsScene *scene){
    this->mScene = scene;
}

QPolygon AzGraphicsSelectedItemArrow::arrowPolygon(SideLight trPos){
    QPolygon poligon;
    poligon << QPoint(0,0)
            << QPoint(7,6)
            << QPoint(2.5,6)
            << QPoint(2.5,15)
            << QPoint(-2.5,15)
            << QPoint(-2.5,6)
            << QPoint(-7,6);

    qreal degreesAngl;

   switch (trPos) {
   case Npos:
       degreesAngl = 180.0;
       break;
   case NEpos:
       degreesAngl = 135.0;
       break;
   case Epos:
       degreesAngl = 90;
       break;
   case SEpos:
       degreesAngl = 45.0;
       break;
   case SWpos:
       degreesAngl = -45;
       break;
   case Wpos:
       degreesAngl = -90;
       break;
   case NWpos:
       degreesAngl = -135;
       break;
   case Spos:
       degreesAngl = 0;
       break;
   default:
      return QPolygon();
   }

   qreal degCos = qCos(qDegreesToRadians(degreesAngl));
   qreal degSin = qSin(qDegreesToRadians(degreesAngl));

   QPolygon res;
   for (int i = 0; i < poligon.size();++i) {
    qreal x = poligon.point(i).x()*degCos + poligon.point(i).y()*degSin;
    qreal y = -poligon.point(i).x()*degSin + poligon.point(i).y()*degCos;
    res << QPoint(x,y);
   }
   QPoint point = arrowPos(trPos); // Возвращает координаты
   res.translate(point); // Растанавливает полигони согласно координат
   return res;
}

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *){

    if (mScene->selectedItems().size() == 0)
        return;

    QPolygon polygone;

    painter->save();
    painter->setBrush(Qt::yellow);
    for (int i = 0; i < 8; ++i) {
        polygone = arrowPolygon((SideLight)i); // Возвращает положение стрелки
        painter->drawPolygon(polygone); // Рисует полигоны
    }
    painter->restore();

}

QPoint AzGraphicsSelectedItemArrow::arrowPos(SideLight aPos) const {
    QList<QGraphicsItem*> items = mScene->selectedItems();
    if (items.size() == 0)
        return QPoint();
    QGraphicsItem *item = items[0];
    QPointF posi = item->pos();
    QRectF bound = item->boundingRect();
    QPoint coordinateArrow;
    switch (aPos) {
    case Npos:
        coordinateArrow =  QPoint(posi.x()+bound.center().x(),posi.y());
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
        break;
    case NWpos:
        coordinateArrow =  QPoint(posi.x(),posi.y());
        break;
    case SEpos:
        coordinateArrow =  QPoint(posi.x()+bound.width(),posi.y()+bound.height());
        break;
    case SWpos:
        coordinateArrow =  QPoint(posi.x(),posi.y()+bound.height());
        break;
    case NotArrow:
      break;
    };
    return coordinateArrow;
}
bool AzGraphicsSelectedItemArrow::isHasSelectedItem(){

}

QGraphicsItem* AzGraphicsSelectedItemArrow::selectedItem(){

}

