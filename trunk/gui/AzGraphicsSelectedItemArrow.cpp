#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>

#include "AzGraphicsView.h"

const Qt::GlobalColor normalColor =  Qt::yellow;
const Qt::GlobalColor selectedColor = Qt::green;

#define ARROWS_COUNT 8

class ArrowsPolygon {
public:
    ArrowsPolygon();
    const QPolygon& operator[] (int i) const	{ return mPolygons[i]; }
private:
    static QPolygon originalPolygon();
    void calcPolygons();
    QVector<QPolygon> mPolygons;
};

const static ArrowsPolygon arrowsPolygon;

ArrowsPolygon::ArrowsPolygon():mPolygons(ARROWS_COUNT)  {
    calcPolygons();
}

QPolygon ArrowsPolygon::originalPolygon() {
    QPolygon res;
    res << QPoint(0,0)      << QPoint(8,9)      << QPoint(2,9)     << QPoint(2,12)  << QPoint(8,12)
        << QPoint(0,20)     << QPoint(-8,12)    << QPoint(-2,12)   << QPoint(-2,9)  << QPoint(-8,9);
    return res;
}



/*!
 * \brief ArrowsPolygon::calcPolygons
 * Вычисляем все стрелочки, а точнее их положение относительно выделеного объекта
 */
void ArrowsPolygon::calcPolygons() {
    const int angleCd[] = {180,135,90,45,0,-45,-90,-135}; //соотвествие углов относительно севера
    qreal degreesAngl;
    for (int i = 0; i < ARROWS_COUNT; ++i) {
      degreesAngl  = angleCd[i]; //угол поворота
      const qreal radDegree = qDegreesToRadians(degreesAngl);
      const qreal degCos = qCos(radDegree);
      const qreal degSin = qSin(radDegree);

      const QPolygon polygon = originalPolygon();
      const int polygonSize = polygon.size();
      for (int j = 0; j < polygonSize; ++j) { //вычисляем стрелочки
          const qreal x = polygon[j].x() *degCos + polygon[j].y()*degSin;
          const qreal y = -polygon[j].x()*degSin + polygon[j].y()*degCos;
          mPolygons[i] << QPoint(x,y);
      }
    }
}


/*!
 * \brief AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow
 * \param view
 */
AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(AzGraphicsView *view): QObject(0),mArrows(ARROWS_COUNT),mIsShowing(false) {
    this->mView = view;
    this->mSideLight = NotArrow;
   // connect(scene,SIGNAL(selectionChanged()),this,SLOT(itemSelectionChanged()));
}


void AzGraphicsSelectedItemArrow::calcArrows() {

    QGraphicsItem *item = selectedItem();
    if (item == 0)
        return;

    QPoint pos = mView->mapFromScene(item->pos());
    QRect bound = mView->mapFromScene(item->boundingRect()).boundingRect();
    QPoint coordArrow;
    int leftX = pos.x();
    int rightX = leftX + bound.width();
    int midX = leftX + (rightX - leftX) / 2;

    int topY = pos.y();
    int bottomY = topY + bound.height();
    int midY = topY + (bottomY - topY) / 2;


   for (int i = 0; i < ARROWS_COUNT; ++i) {

       switch (i) {
       case Npos:
           coordArrow = QPoint(midX,topY);
           break;
       case Spos:
           coordArrow = QPoint(midX,bottomY);
           break;
       case Epos:
           coordArrow = QPoint(rightX,midY);
           break;
       case Wpos:
           coordArrow = QPoint(leftX,midY);
           break;
       case NEpos:
           coordArrow = QPoint(rightX,topY);
           break;
       case NWpos:
           coordArrow = QPoint(leftX,topY);
           break;
       case SEpos:
           coordArrow = QPoint(rightX,bottomY);
           break;
       case SWpos:
           coordArrow = QPoint(leftX,bottomY);
           break;
       case NotArrow:
         break;
       };

       QPolygon res = arrowsPolygon[i];
       res.translate(coordArrow); // Перемещает полигон согласно координат

     mArrows[i] = res;
   }
}

/*!
 * \brief AzGraphicsSelectedItemArrow::show Показывает стрелочки
 * \param painter
 */
void AzGraphicsSelectedItemArrow::show(QPainter *painter,const QRectF rect){
    qDebug() << rect;
    if (!isHasSelectedItem()) {
        return;
    }


//    qDebug() << selectedItem()->boundingRect();

    calcArrows();
//    if (!rect.intersects(boundingRect()))
//        return;

    painter->save();

    painter->setWorldMatrixEnabled(false);


    //test.setMatrix(1,0,0,0,1,0,trans.m31(),trans.m32(),1);

    QPen pen;
    pen.setWidth(0);
    painter->setPen(pen);

    for (int i = 0; i < ARROWS_COUNT; ++i) {
        if (mSideLight == i) {
            painter->setBrush(selectedColor);
        } else {
            painter->setBrush(normalColor);
        }
        painter->drawPolygon(mArrows[i]); // Рисует полигоны
    }
    painter->restore();
}


bool AzGraphicsSelectedItemArrow::isHasSelectedItem()const {
    return mView ? mView->scene()->selectedItems().size() > 0 : false;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::selectedItem
 * \return возвращает выделенный имтем. 0 - если ничего не выделенно
 */
QGraphicsItem* AzGraphicsSelectedItemArrow::selectedItem()const {
    return isHasSelectedItem() ?  mView->scene()->selectedItems()[0] : 0;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::mouseMoveEvent
 * \param event
 */
void AzGraphicsSelectedItemArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (!isHasSelectedItem())
        return;

    CardinalDirection res = containsPoint(event);
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
AzGraphicsSelectedItemArrow::CardinalDirection AzGraphicsSelectedItemArrow::containsPoint(QGraphicsSceneMouseEvent *event) {
    QPolygonF polygone;
    for (int i = 0; i<ARROWS_COUNT; ++i) {
        QPoint mousePos(event->scenePos().x(),event->scenePos().y());
        if (mArrows[i].containsPoint(mousePos,Qt::OddEvenFill)) //Возвращает истину, если данная точка находится внутри многоугольника в противном случае возвращает ложь.
            return (CardinalDirection)i;
    }
    return NotArrow;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::hoverEnterEvent
 */
void AzGraphicsSelectedItemArrow::hoverEnterEvent(){
    qDebug() <<"Enter" << mSideLight;
    //QRectF pos = boundingRectF();
   // mScene->update();
}


/*!
 * \brief AzGraphicsSelectedItemArrow::hoverLeaveEvent
 */
void AzGraphicsSelectedItemArrow::hoverLeaveEvent(){
    qDebug() <<"Leave" << mSideLight;
    //mScene->update();
}


/*!
 * \brief AzGraphicsSelectedItemArrow::itemSelectionChanged
 */
void AzGraphicsSelectedItemArrow::selectionChanged() {
    if (!mView->scene())
        return;
    if (mIsShowing) { //убираем старые стрелы
       // mView->scene()->update(sceneBoundingRect());
       mView->scene()->invalidate(sceneBoundingRect(),QGraphicsScene::ForegroundLayer);
    }
    if (isHasSelectedItem()) {
        calcArrows();
        //mView->scene()->update(sceneBoundingRect());
        mView->scene()->invalidate(sceneBoundingRect(),QGraphicsScene::ForegroundLayer);
        mIsShowing = true;
    } else {
        mIsShowing = false;
    }
}

QRect AzGraphicsSelectedItemArrow::boundingRect() const {
   QRect res;
   QPoint point = QPoint(mArrows[Wpos].boundingRect().left(),mArrows[Npos].boundingRect().top());
   res.setTopLeft(point);
   point = QPoint(mArrows[Epos].boundingRect().right(),mArrows[Spos].boundingRect().bottom());
   res.setBottomRight(point);
   return res;
}

QRectF AzGraphicsSelectedItemArrow::sceneBoundingRect() const {
    return mView->mapToScene(boundingRect()).boundingRect();
}
