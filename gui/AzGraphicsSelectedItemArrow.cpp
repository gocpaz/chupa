#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtMath>
#include <QMouseEvent>

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
    res << QPoint(0,0)
        << QPoint(8,9)      << QPoint(2,9)     << QPoint(2,12)  << QPoint(8,12)
        << QPoint(0,20);
    for (int i = 4; i > 0; --i) { //mirror half arrow
       res << QPoint(res[i].x() * -1,res[i].y());
    }
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
AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(AzGraphicsView *view):
    mArrows(ARROWS_COUNT),mIsShowing(false),mMouseButton(Qt::NoButton) {
    this->mView = view;
    this->mSideLight = NotArrow;
    this->mActiveTransformArrow = NotArrow;
#ifdef VIEW_PAINT_RECT_DEBUG
    mPaintCount = 0;
#endif
   // connect(scene,SIGNAL(selectionChanged()),this,SLOT(itemSelectionChanged()));
}

/*!
 * \brief AzGraphicsSelectedItemArrow::itemRect
 * \param item
 * \return Возвращает координаты и размеры итема в системе координат view
 */

QRect AzGraphicsSelectedItemArrow::itemRect(QGraphicsItem *item) const {
    return mView->mapFromScene(item->sceneBoundingRect()).boundingRect();
}

/*!
 * \brief AzGraphicsSelectedItemArrow::calcArrows
 * \param rect
 * Вычисляет положени стрелок в координатах view
 */
void AzGraphicsSelectedItemArrow::calcArrows(const QRect& rect) {
   QPoint coordArrow;
   for (int i = 0; i < ARROWS_COUNT; ++i) {
       switch (i) {
       case NPos:
           coordArrow = QPoint(rect.center().x(),rect.top());
           break;
       case SPos:
           coordArrow = QPoint(rect.center().x(),rect.bottom());
           break;
       case EPos:
           coordArrow = QPoint(rect.right(),rect.center().y());
           break;
       case WPos:
           coordArrow = QPoint(rect.left(),rect.center().y());
           break;
       case NEPos:
           coordArrow = QPoint(rect.right(),rect.top());
           break;
       case NWPos:
           coordArrow = QPoint(rect.left(),rect.top());
           break;
       case SEPos:
           coordArrow = QPoint(rect.right(),rect.bottom());
           break;
       case SWPos:
           coordArrow = QPoint(rect.left(),rect.bottom());
           break;
       case NotArrow:
         break;
       };

       QPolygon res = arrowsPolygon[i];
       res.translate(coordArrow); // Перемещает полигон согласно координат

     mArrows[i] = res;
   }

   //calc bounding rect arrow
   QPoint point = QPoint(mArrows[WPos].boundingRect().left(),mArrows[NPos].boundingRect().top());
   mBoundingRect.setTopLeft(point);
   point = QPoint(mArrows[EPos].boundingRect().right(),mArrows[SPos].boundingRect().bottom());
   mBoundingRect.setBottomRight(point);

}

void AzGraphicsSelectedItemArrow::calcArrows() {
    QGraphicsItem *item = selectedItem();
    if (item == 0)
        return;
    QRect rect = itemRect(item);
    calcArrows(rect);
}



/*!
 * \brief AzGraphicsSelectedItemArrow::show Показывает стрелочки
 * \param painter
 */
void AzGraphicsSelectedItemArrow::show(QPainter *painter,const QRectF rect){
    if (!isHasSelectedItem()) {
        return;
    }

    if (mOldRectSelectedItem != itemRect(selectedItem())) { //pos or size change?
        QRect oldBoundingArrowRect = mBoundingRect;
        calcArrows();
        QRect unitedRect = oldBoundingArrowRect.united(mBoundingRect);
        mOldRectSelectedItem = itemRect(selectedItem());
        QRectF sceneRect = mView->mapToScene(unitedRect).boundingRect();
        mView->scene()->invalidate(rect.united(sceneRect),QGraphicsScene::ForegroundLayer);
        return;
    }

	calcArrows();


    QVector<int> arrowsList;
    if (mActiveTransformArrow == NotArrow) {
       arrowsList = containsArrows(mView->mapFromScene(rect).boundingRect());
    } else {
        arrowsList.append(mActiveTransformArrow);
    }
    if (arrowsList.size() == 0)
        return;
    painter->save();
    painter->setWorldMatrixEnabled(false);

    for (int i = 0; i < arrowsList.size(); ++i) {
        if ((arrowsList[i] == mSideLight) || (arrowsList[i] == mActiveTransformArrow) ) {
            painter->setBrush(selectedColor);
        } else {
            painter->setBrush(normalColor);
        }
        painter->drawPolygon(mArrows[arrowsList[i]]);
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
 * \brief AzGraphicsSelectedItemArrow::containsPoint
 * \param
 * \return
 */
AzGraphicsSelectedItemArrow::Side AzGraphicsSelectedItemArrow::containsPoint(const QPoint& point) {
    if (!mBoundingRect.contains(point))
        return NotArrow;
    for (int i = 0; i < ARROWS_COUNT; ++i) {
        if (mArrows[i].boundingRect().contains(point))
            return (Side)i;
    }
    return NotArrow;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::containsArrows
 * \param rec область
 * \return Возвращает массив "стрелок", которые попадают в \a rec
 */
QVector<int> AzGraphicsSelectedItemArrow::containsArrows(const QRect& rec) {
    QVector<int> res;
    for (int i = 0; i < ARROWS_COUNT; ++i) {
        if (rec.intersects(mArrows[i].boundingRect()))
            res.append(i);
    }
    return res;
}

void AzGraphicsSelectedItemArrow::mousePressEvent(QMouseEvent * event) {
    mMouseButton = event->button(); //save button state for mouse move event
    if (mSideLight != NotArrow) {
       mActiveTransformArrow = mSideLight;  //set transform state arrow
       mOldItemRect = itemRect();
       mOffsetMousePos = event->pos().y() - itemRect().bottom(); //позиция относительно стрелки
       //setCursor(mActiveTransformArrow);
       event->setAccepted(false); //блокируем, чтобы не пропадал selected не изменялось
    }
}

void AzGraphicsSelectedItemArrow::mouseReleaseEvent(QMouseEvent *) {
    mMouseButton = Qt::NoButton;
    if (mActiveTransformArrow != NotArrow) {
        mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
        mActiveTransformArrow = NotArrow;
    }
    //setCursor(NotArrow);
}


/*!
 * \brief AzGraphicsSelectedItemArrow::mouseMoveEvent
 * \param event
 */
void AzGraphicsSelectedItemArrow::mouseMoveEvent(QMouseEvent *event){
    if (!isHasSelectedItem())
        return;
    QPoint mousePos = event->pos();
    Side res = containsPoint(mousePos);
    if(mSideLight == NotArrow){
        if (res != NotArrow) {
            mSideLight = res;
            hoverEnterEvent();
        }
    } else {
        if(res == NotArrow){
           hoverLeaveEvent();
           mSideLight = res;
        }
    }

    if (mActiveTransformArrow != AzGraphicsSelectedItemArrow::NotArrow) {
        if (mMouseButton == Qt::LeftButton) {
            int height = selectedItem()->boundingRect().height(); //original height
            int delta = event->y() - (mOldItemRect.y() + height) - mOffsetMousePos; //length from current pos to original pos
            int scaleHeight = delta + height;
            qreal scale = (qreal)(scaleHeight) / (qreal)(height);

            QTransform trans ;
            trans.scale(1,scale);
            selectedItem()->setTransform(trans);
        }
    }
}

/*!
 * \brief Вызывается при заходе на стрелочку
 * Пересовывает стрелку другим цветом на "активную"
 *
 */
void AzGraphicsSelectedItemArrow::hoverEnterEvent(){
    QRectF rect = mView->mapToScene(mArrows[mSideLight].boundingRect()).boundingRect();
    mView->scene()->invalidate(rect,QGraphicsScene::ForegroundLayer);
}


/*!
 * \brief Вызывается при выходе со стрелочки
 *  \a mSideLight указывает на стрелочку, с которой выходим
 */
void AzGraphicsSelectedItemArrow::hoverLeaveEvent(){
    QRectF rect = mView->mapToScene(mArrows[mSideLight].boundingRect()).boundingRect();
    mView->scene()->invalidate(rect,QGraphicsScene::ForegroundLayer);
}


/*!
 * \brief AzGraphicsSelectedItemArrow::itemSelectionChanged
 */
void AzGraphicsSelectedItemArrow::selectionChanged() {
    if (!mView->scene())
        return;
    if (mIsShowing) { //убираем старые стрелы
       mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
    }
    if (isHasSelectedItem()) {
        mOldRectSelectedItem = itemRect(selectedItem());
        calcArrows();
        mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
        mIsShowing = true;
    } else {
        mIsShowing = false;
    }
}

/*!
 * \brief AzGraphicsSelectedItemArrow::itemMoved Перерисовывает стрелы при перемещении
 *
 */
void AzGraphicsSelectedItemArrow::itemMoved(const QPoint&,const QPoint&) {
//#ifdef VIEW_PAINT_RECT_DEBUG
//    mDebugRects.clear();
//#endif
//   // truble in QT ??? При попытке перерисовки только стрелок вылазит глюк в виде остаточных линий
//   QVector <QPolygon> oldArrows = mArrows; //old arrow pos
//   QVector <QRect> united(ARROWS_COUNT); //rect old pos + new pos
//   calcArrows();    //calc new pos
//   for (int i = 0; i < ARROWS_COUNT; ++i) {
//      QRect rect = oldArrows[i].boundingRect().united(mArrows[i].boundingRect());

//#ifdef VIEW_PAINT_RECT_DEBUG
//      mDebugRects.append(mView->mapToScene(rect).boundingRect());
//#endif
//      mView->scene()->invalidate(mView->mapToScene(rect).boundingRect(),QGraphicsScene::ForegroundLayer);
//   }
}

QRectF AzGraphicsSelectedItemArrow::boundingRectToScene() const {
    return mView->mapToScene(mBoundingRect).boundingRect();
}

void AzGraphicsSelectedItemArrow::setCursor(Side arrow) {
    switch (arrow) {
        case NPos:
        case SPos:
            mView->setCursor(Qt::SizeVerCursor);
            break;
        case EPos:
        case WPos:
            mView->setCursor(Qt::SizeHorCursor);
            break;
        case NEPos:
        case SWPos:
            mView->setCursor(Qt::SizeBDiagCursor);
            break;
        case NWPos:
        case SEPos:
            mView->setCursor(Qt::SizeFDiagCursor);
            break;
        default:
           mView->unsetCursor();
   }
}

#ifdef VIEW_PAINT_RECT_DEBUG
void AzGraphicsSelectedItemArrow::showPaintRectDebug(QPainter *painter) {
    for (int i = 0; i < mDebugRects.size();++i) {
        painter->drawRect(mDebugRects[i]);
    }
}
#endif
