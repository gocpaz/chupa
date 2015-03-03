#include "AzGraphicsView.h"

#include <QEvent>
#include <QDebug>

#include "AzGraphicsScene.h"

AzGraphicsView::AzGraphicsView(QGraphicsScene * scene, QWidget * parent):
    QGraphicsView(parent),mArrows(this) {
    mActiveTransformArrow = AzGraphicsSelectedItemArrow::NotArrow;
    setScene(scene);
    setShowArrow(false);



}

AzGraphicsView::~AzGraphicsView() {

}

/*!
 * \brief AzGraphicsView::setShowArrow
 * \param val
 * Показывать стрелочки или нет. Тaкже устанавливает setMousetTraking - ловить передвижение мыши всегда (если true)
 */
void AzGraphicsView::setShowArrow(bool val) {
    mShowArrow = val;
    setMouseTracking(val);

    //selectionChanged();
    //mArrows.scale(AzGraphicsSelectedItemArrow::NPos,QPoint(0,160));
}

/*!
 * \brief AzGraphicsView::setScene
 * \param scene
 * Устанавливаем сцену. Предполагаем, что сцена заведома валидна и задана к конструкторе
 */
void AzGraphicsView::setScene(QGraphicsScene *scene) {
    QGraphicsView::setScene(scene);
    AzGraphicsScene *azScene = dynamic_cast<AzGraphicsScene*>(scene);
    if (azScene) {
        connect(scene,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
        connect(azScene,SIGNAL(moveItemEvent(QPointF,QPointF)),this,SLOT(moveItemEvent(QPointF,QPointF)));
    }

}

void AzGraphicsView::drawForeground(QPainter * painter, const QRectF & rect) {
    QGraphicsView::drawForeground(painter,rect);
    if (mShowArrow)
        mArrows.show(painter,rect);
}

void AzGraphicsView::selectionChanged() {
    if (mShowArrow)
        mArrows.selectionChanged();
}

void AzGraphicsView::moveItemEvent(const QPointF&, const QPointF&) {
//    if (mShowArrow)
//        mArrows.itemMoved(mapFromScene(oldPos),mapFromScene(newPos));
}

void AzGraphicsView::mousePressEvent(QMouseEvent * event) {
    if (mShowArrow)
        mArrows.mousePressEvent(event);
    if (event->isAccepted())
        QGraphicsView::mousePressEvent(event);
}

void AzGraphicsView::mouseReleaseEvent(QMouseEvent * event) {
    if (mShowArrow)
        mArrows.mouseReleaseEvent(event);
    QGraphicsView::mouseReleaseEvent(event);
}

void AzGraphicsView::mouseDoubleClickEvent(QMouseEvent * event) {
    if (mActiveTransformArrow != AzGraphicsSelectedItemArrow::NotArrow) //ignore DB click as single click (lost selected)
        return;
    QGraphicsView::mouseDoubleClickEvent(event);
}

void AzGraphicsView::mouseMoveEvent(QMouseEvent * event) {
    if (mShowArrow) {
        mArrows.mouseMoveEvent(event);
    }
#ifdef DEBUG_EDITOR
    emit mouseMove(event);
#endif
    QGraphicsView::mouseMoveEvent(event);

}

