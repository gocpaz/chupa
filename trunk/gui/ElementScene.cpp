#include "ElementScene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "AzGraphicsSelectedItemArrow.h"


/*!
 * \brief ElementScene::ElementScene Конструктор
 * \param parent
 */

ElementScene::ElementScene(QObject *parent) :
    QGraphicsScene(parent),mIsMovingItems(false) {
}

void ElementScene::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    if ((event->buttons() & Qt::LeftButton) && !mIsMovingItems ) { //check that moving items
        QList<QGraphicsItem*> selectedItems = ElementScene::selectedItems();
        for (int i = 0; i < selectedItems.size();++i) {
            if (selectedItems[i]->flags() & QGraphicsItem::ItemIsMovable) {
                mIsMovingItems = true;
                enterMoving();
                break;
            }
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

/*!
 * \brief ElementScene::mouseReleaseEvent- переопределяет стандартное событие мыши
 * \param event - событие мыши
 * Переопределяет событие миши для определения выхода мыши из объекта сцены. Вызывает метод endMoving()
 */
void ElementScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    if (event->button() == Qt::LeftButton && mIsMovingItems) {
        mIsMovingItems = false;
        endMoving();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ElementScene::enterMoving() {
}

void ElementScene::endMoving() {
}

void ElementScene::drawForeground(QPainter *painter, const QRectF &rect){

    QGraphicsScene::drawForeground(painter, rect);

    QList<QGraphicsItem*> selected = selectedItems();
    if(selected.size()>0){
      mSelectedArrow.show(painter,selected[0]);
    }

}

