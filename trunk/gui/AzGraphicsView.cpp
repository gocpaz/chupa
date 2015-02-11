#include "AzGraphicsView.h"

#include <QEvent>
#include <QDebug>

AzGraphicsView::AzGraphicsView(QGraphicsScene * scene, QWidget * parent): QGraphicsView(parent),mArrows(this) {
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
    if (scene)
        connect(scene,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));

}


void AzGraphicsView::drawForeground(QPainter * painter, const QRectF & rect) {
    QGraphicsView::drawForeground(painter,rect);
    mArrows.show(painter,rect);
}

void AzGraphicsView::selectionChanged() {
    mArrows.selectionChanged();
}
