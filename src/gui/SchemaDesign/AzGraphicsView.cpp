#include "AzGraphicsView.h"

#include <QEvent>
#include <QDebug>

#include "AzGraphicsScene.h"

AzGraphicsView::AzGraphicsView(QGraphicsScene * scene, QWidget * parent):
    QGraphicsView(parent),mArrows(this) {
    setScene(scene);
    setShowSelectedArrow(false);
}

AzGraphicsView::~AzGraphicsView() {

}

/*!
 * \brief AzGraphicsView::setShowArrow
 * \param val
 * Показывать стрелочки или нет. Тaкже устанавливает setMousetTraking - ловить передвижение мыши всегда (если true)
 */
void AzGraphicsView::setShowSelectedArrow(bool val) {
    mShowArrow = val;
    setMouseTracking(val);
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

/*!
 * ignore all dbClick -
 *
 */
void AzGraphicsView::mouseDoubleClickEvent(QMouseEvent *) {
//    if (mActiveTransformArrow != AzTransformArrow::NotArrow) //ignore DB click as single click (lost selected)
//        return;
   // QGraphicsView::mouseDoubleClickEvent(event);
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

