#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtMath>

QPolygon drPoligon(){
   QPolygon poligon;
   poligon << QPoint(0,0)
           << QPoint(7,6)
           << QPoint(2.5,6)
           << QPoint(2.5,15)
           << QPoint(-2.5,15)
           << QPoint(-2.5,6)
           << QPoint(-7,6);
   return poligon;
}

static const QPolygon polygon = drPoligon();

AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(QGraphicsScene *scene){
    this->mScene = scene;
}

QPolygon AzGraphicsSelectedItemArrow::arrowPolygon(SideLight trPos){

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
   for (int i = 0; i < polygon.size();++i) {
    qreal x = polygon.point(i).x()*degCos + polygon.point(i).y()*degSin;
    qreal y = -polygon.point(i).x()*degSin + polygon.point(i).y()*degCos;
    res << QPoint(x,y);
   }
   QPoint point = arrowPos(trPos); // Возвращает координаты
   res.translate(point); // Растанавливает полигони согласно координат
   return res;
}

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *){

    if (!isHasSelectedItem())
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

    QGraphicsItem *item = selectedItem();
    if (item == 0)
        return QPoint();

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

QGraphicsItem* AzGraphicsSelectedItemArrow::selectedItem()const {
//    if(isHasSelectedItem()) {
//        return  mScene->selectedItems()[0];
//    }
//    return 0;
    return isHasSelectedItem() ?  mScene->selectedItems()[0] : 0;
}
void AzGraphicsSelectedItemArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    mScene->mouseMoveEvent(event);
}

