#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtMath>
#include <QMouseEvent>

#include <math.h>

#include "AzGraphicsSvgItem.h"
#include "AzGraphicsView.h"

const Qt::GlobalColor normalColor =  Qt::yellow;
const Qt::GlobalColor selectedColor = Qt::green;

const QSize minimumEffectiveTransformSize(10,10); //lost precession (maybe releative?, but visible in small size

#define ARROWS_COUNT 8

class ArrowsPolygons {
public:
    ArrowsPolygons();
    inline const QPolygon& transform(int i) const      { return mTransformPolygons[i]; }
    inline const QPolygon& rotate(int i) const         { return mRotatePolygons[i];    }
private:
    void calcPolygons();
    static QPolygon originalTransformPolygon();
    static QPolygon originalRotatePolygon();
    QVector<QPolygon> mTransformPolygons;
    QVector<QPolygon> mRotatePolygons;
};

const static ArrowsPolygons arrows;

ArrowsPolygons::ArrowsPolygons():mTransformPolygons(ARROWS_COUNT),mRotatePolygons(ARROWS_COUNT) {
    calcPolygons();

}


/*!
 * \brief ArrowsPolygon::calcPolygons
 * Вычисляем все стрелочки, а точнее их положение относительно выделеного объекта в абсооютный координатах
 */
void ArrowsPolygons::calcPolygons() {

    const QPolygon tranformPolygon = originalTransformPolygon();
    const QPolygon rotatePolygon = originalRotatePolygon();

    QTransform trans;
    //calc transform arrow
    mTransformPolygons[AzGraphicsSelectedItemArrow::SPos] = originalTransformPolygon();
    for (int i = 1; i < ARROWS_COUNT; ++i) {
        trans.rotate(-45);
        mTransformPolygons[i] = trans.map(tranformPolygon);
    }

    //calc rotate arrow
    trans.reset();
    mRotatePolygons[AzGraphicsSelectedItemArrow::SEPos] = originalRotatePolygon();
    for (int i = 3; i < ARROWS_COUNT;i+=2) { //90 degree arrow
        trans.rotate(-90);
        mRotatePolygons[i] = trans.map(rotatePolygon);
    }
    const int lenghtArrow = tranformPolygon.boundingRect().height() / 2;
    const int offsetArrow = -rotatePolygon.boundingRect().x() - 1; //SO SORRY ( - 1) - int graphics
    trans.reset(); //horisontal & vert arrow
    trans.translate(-lenghtArrow,offsetArrow);
    mRotatePolygons[AzGraphicsSelectedItemArrow::SPos] = trans.map(mTransformPolygons[AzGraphicsSelectedItemArrow::EPos]);
    trans.reset();
    trans.translate(lenghtArrow,-offsetArrow);
    mRotatePolygons[AzGraphicsSelectedItemArrow::NPos] = trans.map(mTransformPolygons[AzGraphicsSelectedItemArrow::WPos]);
    trans.reset();
    trans.translate(lenghtArrow,-offsetArrow);
    mRotatePolygons[AzGraphicsSelectedItemArrow::EPos] = trans.map(mTransformPolygons[AzGraphicsSelectedItemArrow::SPos]);
    trans.reset();
    trans.translate(-lenghtArrow,offsetArrow);
    mRotatePolygons[AzGraphicsSelectedItemArrow::WPos] = trans.map(mTransformPolygons[AzGraphicsSelectedItemArrow::NPos]);

}

const int tipWidth  = 4; //half size
const int tipHeight = 6;
const int shaftWidth = 1;
const int shaftHeight = 4;

const int shaftRadius = 5;

QPolygon ArrowsPolygons::originalTransformPolygon() {
    QPolygon res;
    res << QPoint(0,0)
        << QPoint(tipWidth,tipHeight) << QPoint(shaftWidth,tipHeight)<< QPoint(shaftWidth,shaftHeight+tipHeight)  << QPoint(tipWidth,shaftHeight+tipHeight)
        << QPoint(0,2*tipHeight+shaftHeight);
    for (int i = 4; i > 0; --i) { //mirror half arrow
       res << QPoint(res[i].x() * -1,res[i].y());
    }
    return res;
}

QPolygon ArrowsPolygons::originalRotatePolygon() {
    QPolygon res;
    res << QPoint(shaftWidth,tipHeight) << QPoint(tipWidth,tipHeight) << QPoint(0,0)
        << QPoint(-tipWidth,tipHeight)<< QPoint(-shaftWidth,tipHeight);

    res << res.last() + QPoint(0,shaftRadius);
    res << res.last() + QPoint(-shaftRadius/2,shaftRadius/2);
    res << res.last() + QPoint(-shaftRadius,0);

    res << res.last() + QPoint(0,-(tipWidth-shaftWidth)); //check point
    res << res.last() + QPoint(-tipHeight,tipWidth);
    res << res.last() + QPoint(tipHeight,tipWidth);
    res << res.last() + QPoint(0,-(tipWidth-shaftWidth));                ;

    int x = res[3].x();
    int y = res[3].y();
    int lenghtX = x - res[8].x();
    int lenghtY = y - res[8].y();

    QTransform trans;
    trans.translate(-x+lenghtX/2,-y + lenghtY/2); //dirty round
    return trans.map(res);
}

/*!
 * \brief AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow
 * \param view
 */
AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow(AzGraphicsView *view):
    mArrows(ARROWS_COUNT) {
    mMouseButton = Qt::NoButton;
    mIsShowing = false;
    this->mView = view;
    this->mSideLight = NotArrow;
    this->mActiveTransformArrow = NotArrow;
    mArrowType = ArrowTransform;
    mPreviosSelectedItem = 0;
   // connect(scene,SIGNAL(selectionChanged()),this,SLOT(itemSelectionChanged()));
}

/*!
 * \brief AzGraphicsSelectedItemArrow::itemRect
 * \param item
 * \return Возвращает координаты и размеры итема в системе координат view
 */

QRect AzGraphicsSelectedItemArrow::itemViewRect(QGraphicsItem *item) const {
    return mView->mapFromScene(item->sceneBoundingRect()).boundingRect();
}


/*!
 * \brief AzGraphicsSelectedItemArrow::calcArrows
 * \param rect
 * Вычисляет положение стрелок в координатах view
 */
void AzGraphicsSelectedItemArrow::calcArrows() {
    QGraphicsItem *item = selectedItem();
    if (item == 0)
        return;
   QRect rect = itemViewRect(item);

   QPoint coordArrow;
   for (int i = 0; i < ARROWS_COUNT; ++i) {
       switch (i) {
           case NPos: coordArrow = QPoint(rect.center().x(),rect.top());    break;
           case SPos: coordArrow = QPoint(rect.center().x(),rect.bottom()); break;
           case EPos: coordArrow = QPoint(rect.right(),rect.center().y());  break;
           case WPos: coordArrow = QPoint(rect.left(),rect.center().y());   break;
           case NEPos:coordArrow = QPoint(rect.right(),rect.top());         break;
           case NWPos:coordArrow = QPoint(rect.left(),rect.top());          break;
           case SEPos:coordArrow = QPoint(rect.right(),rect.bottom());      break;
           case SWPos:coordArrow = QPoint(rect.left(),rect.bottom());       break;
           case NotArrow: break;
       };

       QPolygon res = (mArrowType == ArrowTransform) ? arrows.transform(i): arrows.rotate(i);
       res.translate(coordArrow); // Перемещает полигон согласно координат

     mArrows[i] = res;
   }

   //calc bounding rect arrow
   QPoint point = QPoint(mArrows[WPos].boundingRect().left(),mArrows[NPos].boundingRect().top());
   mBoundingRect.setTopLeft(point);
   point = QPoint(mArrows[EPos].boundingRect().right(),mArrows[SPos].boundingRect().bottom());
   mBoundingRect.setBottomRight(point);

}


/*!
 * \brief AzGraphicsSelectedItemArrow::show Показывает стрелочки
 * \param painter
 */
void AzGraphicsSelectedItemArrow::show(QPainter *painter,const QRectF rect){
    if (!isHasSelectedItem() && !mIsShowing) {
        return;
    }

    painter->drawRect(selectedItem()->sceneBoundingRect());

#ifdef DEBUG_ROTATE
    painter->drawPolygon(mDebugRotatePolygon);

    painter->drawEllipse(mDebugCircle);
#endif

    if (mOldRectSelectedItem != itemViewRect(selectedItem())) { //pos or size change?
        QRect oldBoundingArrowRect = mBoundingRect;
        calcArrows();
        QRect unitedRect = oldBoundingArrowRect.united(mBoundingRect);
        mOldRectSelectedItem = itemViewRect(selectedItem());
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
    mStartMousePos = event->pos();    //save pos for moveMouse
    if (mSideLight != NotArrow) {
        mActiveTransformArrow = mSideLight;  //set transform state arrow
        mStartTransformRect = itemViewRect(selectedItem());
        event->setAccepted(false); //блокируем, чтобы не пропадал selected

        //calc mouse ofset arrow
        QRect rect = itemViewRect(selectedItem());

        switch (mSideLight) {
            case SPos:  mMouseOffset = QPoint(0,rect.bottom() - mStartMousePos.y());break;
            case SEPos: mMouseOffset = QPoint(rect.right() - mStartMousePos.x(),rect.bottom() - mStartMousePos.y()); break;
            case EPos:  mMouseOffset = QPoint(rect.right() - mStartMousePos.x(),0); break;
            case NEPos: mMouseOffset = QPoint(rect.right() - mStartMousePos.x(),rect.top() - mStartMousePos.y()); break;
            case NPos:  mMouseOffset = QPoint(0,rect.top() - mStartMousePos.y()); break;
            case NWPos: mMouseOffset = QPoint(rect.left() - mStartMousePos.x(),rect.top() - mStartMousePos.y()); break;
            case WPos:  mMouseOffset = QPoint(rect.left() - mStartMousePos.x(),0); break;
            case SWPos: mMouseOffset = QPoint(rect.left() - mStartMousePos.x(),rect.bottom() - mStartMousePos.y()); break;
        default:
            mMouseOffset = QPoint(0,0);
        }

        //lost precession
        if (mStartTransformRect.height() < minimumEffectiveTransformSize.height())
            mMouseOffset.setY(0);
        if (mStartTransformRect.width() < minimumEffectiveTransformSize.width())
            mMouseOffset.setX(0);
    }
}

void AzGraphicsSelectedItemArrow::mouseReleaseEvent(QMouseEvent *event) {
    if (mMouseButton == Qt::LeftButton) {
        if (selectedItem() && mStartMousePos == event->pos() && mPreviosSelectedItem == selectedItem())
            mouseClickOnSelectedItemEvent();
            mPreviosSelectedItem = selectedItem();
    }
    mMouseButton = Qt::NoButton;
    mMouseOffset = QPoint(0,0);
    if (mActiveTransformArrow != NotArrow) {
        mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer); //restore color
        mActiveTransformArrow = NotArrow;
        AzGraphicsSvgItem *item = dynamic_cast<AzGraphicsSvgItem*> (selectedItem());
        if (item) {
            item->fixTransofm();
        }
    }
}

void AzGraphicsSelectedItemArrow::setArrowType(ArrowType type) {
    if (mArrowType == type)
        return;
    mIsShowing = false; //clear old arrow
    mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
    mArrowType = type;
    mIsShowing = true; //show new arrow
    calcArrows();
    mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);

}

void AzGraphicsSelectedItemArrow::mouseClickOnSelectedItemEvent() {
    if (mArrowType == ArrowTransform) {
        setArrowType(ArrowRotate);
    } else {
        setArrowType(ArrowTransform);
    }
}

/*!
 * \brief AzGraphicsSelectedItemArrow::mouseMoveEvent
 * \param event
 * Вызывается при передвижении мыши.
 * Выполняет подсветку стрелы и трансформацию итема при нужных условиях
 */
void AzGraphicsSelectedItemArrow::mouseMoveEvent(QMouseEvent *event){
    if (!isHasSelectedItem())
        return;
    QPoint mouseViewPos = event->pos();
    lightArrow(mouseViewPos);

    if (mActiveTransformArrow == AzGraphicsSelectedItemArrow::NotArrow)
        return;
    if (mMouseButton != Qt::LeftButton)
        return;
    const QPoint absPos = mouseViewPos + mMouseOffset;
    if (mArrowType == AzGraphicsSelectedItemArrow::ArrowTransform)
        mouseMoveScale(absPos);
    else
        mouseMoveRotate(absPos);
}


/*!
 * \brief AzGraphicsSelectedItemArrow::scale
 * Масштабирует по стороне итема /a side на новую позицию /a newPos в координатах view.
 * Функция предназначена для итеративного маштабирования
 * \warning - mOldRect должен указывать на первоначальные значения
 * \Note
 * Убрать в приват. Убрать проверку на выделеный итем, должна вызываться из безопастной функции
 */
void AzGraphicsSelectedItemArrow::mouseMoveScale(const QPoint& newPos) {
    if (!isHasSelectedItem())
        return;
    int height = mStartTransformRect.height(); //original height
    int width  = mStartTransformRect.width();  //original width

    int scaleHeight = height; //keep original pos - do not need calculate in some case
    int scaleWidth  = width;
    const QPointF scenePos = mView->mapToScene(newPos);
    QGraphicsItem *item = selectedItem();

    switch (mActiveTransformArrow) {
        case SPos:   scaleHeight = newPos.y() - mStartTransformRect.y();    break;
        case SEPos:  scaleWidth  = newPos.x() - mStartTransformRect.x();  scaleHeight = newPos.y() - mStartTransformRect.y();   break;
        case EPos:   scaleWidth  = newPos.x() - mStartTransformRect.x();   break;
        case NEPos:  scaleWidth  = newPos.x() - mStartTransformRect.x(); scaleHeight = -newPos.y() + mStartTransformRect.bottom(); break;
        case NPos:   scaleHeight = -newPos.y() + mStartTransformRect.bottom(); break;
        case NWPos:  scaleHeight = -newPos.y() + mStartTransformRect.bottom(); scaleWidth =  -newPos.x() + mStartTransformRect.right(); break;
        case WPos:   scaleWidth =  -newPos.x() + mStartTransformRect.right();  break;
        case SWPos:  scaleHeight = newPos.y() - mStartTransformRect.y();  scaleWidth =  -newPos.x() + mStartTransformRect.right(); break;
    default:
            break;
    }

    QTransform trans ;
    qreal scaleY = (qreal)(scaleHeight) / height;
    qreal scaleX = (qreal)(scaleWidth) / width;
    trans.scale(scaleX,scaleY);

    switch (mActiveTransformArrow) { //translate to pos
        case NEPos: item->setPos(item->x(),scenePos.y());       break;
        case NPos:  item->setPos(item->x(),scenePos.y());       break;
        case NWPos: item->setPos(scenePos.x(),scenePos.y());    break;
        case WPos:  item->setPos(scenePos.x(),item->y());       break;
        case SWPos: item->setPos(scenePos.x(),item->y());       break;
    default:
        break;
    }
    item->setTransform(trans);
}

/*!
    Вызывается при перемещении миши, где newPos - новая позиция мыхи.
    mActiveTransformArrow должен указывать на активную стрелу.
 */
void AzGraphicsSelectedItemArrow::mouseMoveRotate(const QPoint& newPos) {
    QGraphicsItem *item = selectedItem();

    const QRect viewRect = mStartTransformRect;
    QPoint basePoint;
    const QPoint centerPoint = viewRect.center();
    switch (mActiveTransformArrow) {
        case SEPos: basePoint = viewRect.bottomRight();     break;
        case NEPos: basePoint = viewRect.topRight();        break;
        case NWPos: basePoint = viewRect.topLeft();         break;
        case SWPos: basePoint = viewRect.bottomLeft();      break;
    default:
        return;
    }
    QLineF lineRadius = QLineF(basePoint,centerPoint);
    QLineF lineMouse = QLineF(centerPoint,newPos);
    qreal radian = qDegreesToRadians(lineMouse.angle()); //angel to X axist
    qreal r = lineRadius.length();
    qreal dx = qCos(radian) * r;
    qreal dy = qSin(radian) * r;

    const QPoint deltaPoint(centerPoint.x() + dx, centerPoint.y() - dy);
    QLineF lineDelta = QLineF(basePoint,deltaPoint);

    //clockwise  or not (multiply vector)
    int clockwise =  (basePoint.x() - centerPoint.x()) * (deltaPoint.y() - centerPoint.y()) - (basePoint.y() - centerPoint.y()) * (deltaPoint.x() - centerPoint.x()) > 0 ? 1 : -1;

    qreal angle = clockwise * 2 * qAsin(lineDelta.length()/2/r);
    if (angle != angle) //NaN
        angle = M_PI;

    QTransform t1,t2,t3;
    const QPointF center =  item->boundingRect().center();
    t1.translate(-center.x(),-center.y());
    t2.rotateRadians(angle);
    t3.translate(center.x(),center.y());

    item->setTransform(t1 * t2 * t3);


#ifdef DEBUG_ROTATE
    QPolygon polygon;
    polygon << lineMouse.toLine().p2() << lineMouse.toLine().p1() << lineRadius.toLine().p1() << lineDelta.toLine().p2();

    QRectF itemRect = item->sceneBoundingRect();
    QRectF circle = itemRect;
    circle.setX(itemRect.x() - (r-itemRect.width()/2));
    circle.setY(itemRect.y() - (r-itemRect.height()/2));
    circle.setWidth(r*2);
    circle.setHeight(r*2);

    mView->scene()->invalidate(mDebugRotatePolygon.boundingRect(),QGraphicsScene::ForegroundLayer);
    mDebugCircle = circle;
    mDebugRotatePolygon = mView->mapToScene(polygon);
    mView->scene()->invalidate(mDebugCircle,QGraphicsScene::ForegroundLayer);
#endif
}


/*!
 * \brief AzGraphicsSelectedItemArrow::lightArrow
 * param mousePos - mouse position
 * Lighting arrow if mouse under
 */
void AzGraphicsSelectedItemArrow::lightArrow(const QPoint& mousePos) {
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
        mArrowType = ArrowTransform;
        mOldRectSelectedItem = itemViewRect(selectedItem());
        calcArrows();
        mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
        mIsShowing = true;
    } else {
        mIsShowing = false;
    }
}

QRectF AzGraphicsSelectedItemArrow::boundingRectToScene() const {
    return mView->mapToScene(mBoundingRect).boundingRect();
}

