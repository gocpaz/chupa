#include "AzTransformArrows.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>

#include <math.h>

#include "AzGraphicsSvgItem.h"
#include "AzGraphicsView.h"
#include "AzSelectedItemPolygonArrows.h"

const Qt::GlobalColor normalColor       = Qt::yellow;
const Qt::GlobalColor selectedColor     = Qt::green;

const QSize minimumEffectiveTransformSize(10,10); //lost precession (maybe releative?, but visible in small size)


/*!
 * \brief AzGraphicsSelectedItemArrow::AzGraphicsSelectedItemArrow
 * \param view
 */
AzTransformArrow::AzTransformArrow(AzGraphicsView *view) {
    mMouseButton = Qt::NoButton;
    mView = view;
    mPreviosSelectedItem = 0;
    mArrows = new AzSelectedItemPolygonArrows;
}

AzTransformArrow::~AzTransformArrow() {
    delete mArrows;
}

QString AzTransformArrow::sideToString(Side side) {
    static QString str[] = {"S","SE","E","NE","N","NW","W","SW","Not arrow"};
    return str[side];
}

QString AzTransformArrow::tranfsofmTypeToString(TransformType type) {
    static QString str[] = {"Scale","Rotate","No transform"};
    return str[type];
}

/*!
 * \brief AzGraphicsSelectedItemArrow::itemRect
 * \param item
 * \return Возвращает координаты и размеры итема в системе координат view
 */

QRect AzTransformArrow::itemViewRect(QGraphicsItem *item) const {
    return mView->mapFromScene(item->sceneBoundingRect()).boundingRect();
}


/*!
 * \brief AzGraphicsSelectedItemArrow::show Показывает стрелочки
 * \param painter
 */
void AzTransformArrow::show(QPainter *painter,const QRectF rect){
    if (!isHasSelectedItem()) {
        return;
    }
#ifdef DEBUG_EDITOR
   // qDebug() << "Draw arrows" << rect;
#endif
    painter->drawRect(selectedItem()->sceneBoundingRect());

#ifdef DEBUG_ROTATE
    painter->drawPolygon(mDebugRotatePolygon);
    painter->drawEllipse(mDebugCircle);
#endif
    //howto  detect change pos - ?
    if (mOldRect != itemViewRect(selectedItem())) { //pos or size change?
        QRect oldBoundingArrowRect = mArrows->boundingRect();
        mArrows->setRect(itemViewRect(selectedItem()));
        QRect unitedRect = oldBoundingArrowRect.united(mArrows->boundingRect());
        mOldRect = itemViewRect(selectedItem());
        QRectF sceneRect = mView->mapToScene(unitedRect).boundingRect();
        mView->scene()->invalidate(rect.united(sceneRect),QGraphicsScene::ForegroundLayer);
        return;
    }

    mArrows->setRect(itemViewRect(selectedItem()));

    QVector<int> arrowsList;
    if (mArrows->activeTransformArrow() == NotArrow) {
       arrowsList = mArrows->containsArrows(mView->mapFromScene(rect).boundingRect());
    } else {
        arrowsList.append(mArrows->activeTransformArrow());
    }
    if (arrowsList.size() == 0)
        return;

    painter->save();
    painter->setWorldMatrixEnabled(false);
    for (int i = 0; i < arrowsList.size(); ++i) {
        if ((arrowsList[i] == mArrows->activeArrow()) || (arrowsList[i] == mArrows->activeTransformArrow()) ) {
            painter->setBrush(selectedColor);
        } else {
            painter->setBrush(normalColor);
        }
        painter->drawPolygon(mArrows->arrow(arrowsList[i]));
    }
    painter->restore();
}

bool AzTransformArrow::isHasSelectedItem()const {
    return mView ? mView->scene()->selectedItems().size() > 0 : false;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::selectedItem
 * \return возвращает выделенный имтем. 0 - если ничего не выделенно
 */
QGraphicsItem* AzTransformArrow::selectedItem()const {
    return isHasSelectedItem() ?  mView->scene()->selectedItems()[0] : 0;
}

void AzTransformArrow::mousePressEvent(QMouseEvent * event) {
#ifdef DEBUG_EDITOR
    qDebug() << "Press mouse btn, side arrow: " << sideToString(mArrows->activeArrow());
#endif
    mMouseButton = event->button(); //save button state for mouse move event
    mStartMousePos = event->pos();    //save pos for moveMouse
    if (mArrows->activeArrow() != NotArrow) {
        event->setAccepted(false); //блокируем, чтобы не пропадал selected
        mArrows->beginTransform(event->pos());
    }
}

/*!
 * \brief AzGraphicsSelectedItemArrow::mouseMoveEvent
 * \param event
 * Вызывается при передвижении мыши.
 * Выполняет подсветку стрелы и трансформацию итема при нужных условиях
 */
void AzTransformArrow::mouseMoveEvent(QMouseEvent *event){
    if (!isHasSelectedItem())
        return;
    QPoint mousePos = event->pos();

    if (mArrows->activeTransformArrow() == AzTransformArrow::NotArrow) {
        lightArrow(mousePos);
        return;
    }
    if (mMouseButton != Qt::LeftButton)
        return;

    const QPoint absPos = mousePos + mArrows->mouseOffset();
    if (mArrows->transformType() == AzTransformArrow::ScaleTransform)
        mouseMoveScale(absPos);
    else
        mouseMoveRotate(absPos);
}

void AzTransformArrow::mouseReleaseEvent(QMouseEvent *event) {
#ifdef DEBUG_EDITOR
    qDebug() << "Release mouse btn";
#endif
    if (mMouseButton == Qt::LeftButton) { //detect click
        if (selectedItem() && mStartMousePos == event->pos() && mPreviosSelectedItem == selectedItem())
            mouseClickOnSelectedItemEvent();
            mPreviosSelectedItem = selectedItem();
    }
    mMouseButton = Qt::NoButton;
    if (mArrows->activeTransformArrow() != NotArrow) {
        mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer); //restore color
        mArrows->setActiveTransformArrow(NotArrow);
        AzGraphicsSvgItem *item = dynamic_cast<AzGraphicsSvgItem*> (selectedItem());
        if (item) {
            item->fixTransofm();
        }
    }
}

void AzTransformArrow::setTransformType(TransformType type) {
    if (mArrows->transformType() == type)
        return;
#ifdef DEBUG_EDITOR
    qDebug() << "Change arrow type:" << tranfsofmTypeToString(mArrows->transformType());
#endif
    mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
    mArrows->setTransformType(type);
    mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);

}

void AzTransformArrow::mouseClickOnSelectedItemEvent() {
#ifdef DEBUG_EDITOR
     qDebug() << "Click mouse";
#endif
    if (mArrows->transformType() == ScaleTransform) {
        setTransformType(RotateTransform);
    } else {
        setTransformType(ScaleTransform);
    }
}

/*!
 * \brief AzGraphicsSelectedItemArrow::scale
 * Масштабирует по стороне итема /a side на новую позицию /a newPos в координатах view.
 * Функция предназначена для итеративного маштабирования
 * \Note
 */
void AzTransformArrow::mouseMoveScale(const QPoint& newPos) {
    if (!isHasSelectedItem())
        return;

    QTransform trans;
    QSizeF scale =  mArrows->newScale(newPos);
    trans.scale(scale.width(),scale.height());
    const QPointF scenePos = mView->mapToScene(newPos);
    QGraphicsItem *item = selectedItem();
    item->setTransform(trans);

    switch (mArrows->activeTransformArrow()) { //translate to pos
        case NEPos: item->setPos(item->x(),scenePos.y());       break;
        case NPos:  item->setPos(item->x(),scenePos.y());       break;
        case NWPos: item->setPos(scenePos.x(),scenePos.y());    break;
        case WPos:  item->setPos(scenePos.x(),item->y());       break;
        case SWPos: item->setPos(scenePos.x(),item->y());       break;
    default:
        break;
    }
}

/*!
    Вызывается при перемещении миши, где newPos - новая позиция мыхи.
    mActiveTransformArrow должен указывать на активную стрелу.
 */
void AzTransformArrow::mouseMoveRotate(const QPoint& newPos) {

    qreal angle = mArrows->newRotate(newPos);

    QGraphicsItem *item = selectedItem();
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
void AzTransformArrow::lightArrow(const QPoint& mousePos) {
    Side res = mArrows->containsPoint(mousePos);
    if(mArrows->activeArrow() == NotArrow){
        if (res != NotArrow) {
            mArrows->setActiveArrow(res);
            hoverEnterEvent();
            qDebug() << "Change side light" << sideToString(mArrows->activeArrow());
        }
    } else {
        if(res == NotArrow){
           hoverLeaveEvent();
           mArrows->setActiveArrow(res);
           qDebug() << "Change side light" << sideToString(mArrows->activeArrow());
        }
    }
}

/*!
 * \brief Вызывается при заходе на стрелочку
 * Пересовывает стрелку другим цветом на "активную"
 *
 */
void AzTransformArrow::hoverEnterEvent(){
    QRectF rect = mView->mapToScene(mArrows->arrow(mArrows->activeArrow()).boundingRect()).boundingRect();
    mView->scene()->invalidate(rect,QGraphicsScene::ForegroundLayer);
}

/*!
 * \brief Вызывается при выходе со стрелочки
 *  \a mSideLight указывает на стрелочку, с которой выходим
 */
void AzTransformArrow::hoverLeaveEvent(){
    QRectF rect = mView->mapToScene(mArrows->arrow(mArrows->activeArrow()).boundingRect()).boundingRect();
    mView->scene()->invalidate(rect,QGraphicsScene::ForegroundLayer);
}

void AzTransformArrow::selectionChanged() {
    mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer); //repaint old selected rect or empty rect
    if (isHasSelectedItem()) {
#ifdef DEBUG_EDITOR
        qDebug() << "Selection change, has selected item";
#endif
        mOldRect = itemViewRect(selectedItem());
        mArrows->setRect(mOldRect);
        mArrows->setTransformType(ScaleTransform);
        mView->scene()->invalidate(boundingRectToScene(),QGraphicsScene::ForegroundLayer);
        return;
    }
#ifdef DEBUG_EDITOR
    else {
        qDebug() << "Selection change, no selected item";
    }
#endif
}

QRectF AzTransformArrow::boundingRectToScene() const {
    return mView->mapToScene(mArrows->boundingRect()).boundingRect();
}

