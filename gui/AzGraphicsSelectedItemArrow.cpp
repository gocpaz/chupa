#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>

const Qt::GlobalColor normalColor =  Qt::yellow;
const Qt::GlobalColor selectedColor = Qt::green;

QPolygonF drPoligon(){
   QPolygonF poligon;
   poligon << QPointF(0,0)
           << QPointF(8,9)
           << QPointF(3,9)
           << QPointF(3,12)
           << QPointF(8,12)
           << QPointF(0,21)
           << QPointF(-8,12)
           << QPointF(-3,12)
           << QPointF(-3,9)
           << QPointF(-8,9);
   return poligon;
}

static const QPolygonF polygon = drPoligon();

/*!
 * \brief AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow
 * \param scene
 */
AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(QGraphicsScene *scene){
    this->mScene = scene;
    this->mSideLight = NotArrow;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::arrowPolygon
 * \param trPos
 * \return
 */
QPolygonF AzGraphicsSelectedItemArrow::arrowPolygon(SideLight trPos){

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
      return QPolygonF();
   }

   qreal degCos = qCos(qDegreesToRadians(degreesAngl));
   qreal degSin = qSin(qDegreesToRadians(degreesAngl));

   QPolygonF res;
   for (int i = 0; i < polygon.size();++i) {
      qreal x = polygon[i].x() *degCos + polygon[i].y()*degSin;
      qreal y = -polygon[i].x()*degSin + polygon[i].y()*degCos;
      res << QPointF(x,y);
   }
   QPointF point = arrowPos(trPos); // Возвращает координаты
   res.translate(point); // Растанавливает полигони согласно координат
   return res;
}
/*!
 * \brief AzGraphicsSelectedItemArrow::show
 * \param painter
 */
void AzGraphicsSelectedItemArrow::show(QPainter *painter){

    if (!isHasSelectedItem())
        return;

    QPolygonF polygone;

    painter->save();
   // painter->setBrush(Qt::black);
    QPen pen;
    pen.setWidth(0);
    painter->setPen(pen);
    for (int i = 0; i < 8; ++i) {
        polygone = arrowPolygon((SideLight)i); // Возвращает положение стрелки
        if (mSideLight == i) {
            painter->setBrush(selectedColor);
        } else {
            painter->setBrush(normalColor);
        }
        painter->drawPolygon(polygone);// Рисует полигоны
    }
    painter->restore();

}

/*!
 * \brief AzGraphicsSelectedItemArrow::arrowPos Возвращает полигон
 * \param aPos - позиция стрелки вокруг выделенного объекта
 * \return полигон
 * Возращает полигон в виде стрелки
 */
QPointF AzGraphicsSelectedItemArrow::arrowPos(SideLight aPos) const {

    QGraphicsItem *item = selectedItem();
    if (item == 0)
        return QPointF();

    QPointF posi = item->pos();
    QRectF bound = item->boundingRect();
    QPointF coordinateArrow;
    switch (aPos) {
    case Npos:
        coordinateArrow =  QPointF(posi.x()+bound.center().x(),posi.y());
        break;
    case Spos:
        coordinateArrow =  QPointF(posi.x()+bound.center().x(),posi.y()+bound.height());
        break;
    case Epos:
        coordinateArrow =  QPointF(posi.x()+bound.width(),posi.y()+bound.center().y());
        break;
    case Wpos:
        coordinateArrow =  QPointF(posi.x(),posi.y()+bound.center().y());
        break;
    case NEpos:
        coordinateArrow =  QPointF(posi.x()+bound.width(),posi.y());
        break;
    case NWpos:
        coordinateArrow =  QPointF(posi.x(),posi.y());
        break;
    case SEpos:
        coordinateArrow =  QPointF(posi.x()+bound.width(),posi.y()+bound.height());
        break;
    case SWpos:
        coordinateArrow =  QPointF(posi.x(),posi.y()+bound.height());
        break;
    case NotArrow:
      break;
    };
    return coordinateArrow;
}
/*!
 * \brief AzGraphicsSelectedItemArrow::selectedItem
 * \return
 */
QGraphicsItem* AzGraphicsSelectedItemArrow::selectedItem()const {
//    if(isHasSelectedItem()) {
//        return  mScene->selectedItems()[0];
//    }
//    return 0;
    return isHasSelectedItem() ?  mScene->selectedItems()[0] : 0;
}
/*!
 * \brief AzGraphicsSelectedItemArrow::mouseMoveEvent
 * \param event
 */
void AzGraphicsSelectedItemArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (!isHasSelectedItem())
        return;

    SideLight res = containsPoint(event);
    if(mSideLight == NotArrow){
        if (res != NotArrow) {
            mSideLight = res;
            hoverEnterEvent();
        }
    } else {
        if(res == NotArrow){
           mSideLight = res;
           hoverLeaveEvent();
        }
    }
}

/*!
 * \brief AzGraphicsSelectedItemArrow::containsPoint
 * \param event
 * \return
 */
AzGraphicsSelectedItemArrow::SideLight AzGraphicsSelectedItemArrow::containsPoint(QGraphicsSceneMouseEvent *event) {
    QPolygonF polygone;
    for (int i = 0; i<8; ++i) {
        polygone = arrowPolygon( (SideLight)i);
        QPointF mousePos(event->scenePos().x(),event->scenePos().y());
        if (polygone.containsPoint(mousePos,Qt::OddEvenFill)) //Возвращает истину, если данная точка находится внутри многоугольника в противном случае возвращает ложь.
            return  (SideLight)i;
    }
    return NotArrow;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::hoverEnterEvent
 */
void AzGraphicsSelectedItemArrow::hoverEnterEvent(){
    qDebug() <<"Enter" << mSideLight;
    //QRectF pos = boundingRectF();
    mScene->update();
}

/*!
 * \brief AzGraphicsSelectedItemArrow::hoverLeaveEvent
 */
void AzGraphicsSelectedItemArrow::hoverLeaveEvent(){
    qDebug() <<"Leave" << mSideLight;
    mScene->update();
}
