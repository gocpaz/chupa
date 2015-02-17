#include "AzGraphicsView.h"

#include <QEvent>
#include <QDebug>

#include "AzGraphicsScene.h"

AzGraphicsView::AzGraphicsView(QGraphicsScene * scene, QWidget * parent):
    QGraphicsView(parent),mArrows(this),mShowArrow(true) {
    mActiveTransformArrow = AzGraphicsSelectedItemArrow::NotArrow;
    setScene(scene);
}

AzGraphicsView::~AzGraphicsView() {

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

void AzGraphicsView::moveItemEvent(const QPointF& oldPos, const QPointF& newPos) {
    if (mShowArrow)
        mArrows.itemMoved(mapFromScene(oldPos),mapFromScene(newPos));
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
    QGraphicsView::mouseMoveEvent(event);
}

