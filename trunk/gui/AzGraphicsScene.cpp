#include "AzGraphicsScene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "AzGraphicsSelectedItemArrow.h"
#include "AzGraphicsPoligonItem.h"


/*!
 * \brief ElementScene::ElementScene Конструктор
 * \param parent
 */
AzGraphicsScene::AzGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),mIsMovingItems(false) {
}

void AzGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    if (mIsMovingItems)
        movingItems();
    if ((event->buttons() & Qt::LeftButton) && !mIsMovingItems ) { //check that moving items
        QList<QGraphicsItem*> selectedItems = AzGraphicsScene::selectedItems();
        for (int i = 0; i < selectedItems.size();++i) {
            if (selectedItems[i]->flags() & QGraphicsItem::ItemIsMovable) {
                mIsMovingItems = true;
                enterMovingItems();
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
void AzGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    if (event->button() == Qt::LeftButton && mIsMovingItems) {
        mIsMovingItems = false;
        endMovingItems();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void AzGraphicsScene::enterMovingItems() {
    mOldPosSelectedItem = selectedItems().first()->pos();
}


void AzGraphicsScene::movingItems() {
    QPointF newPosSelectedItem = selectedItems().first()->pos();
    emit moveItemEvent(mOldPosSelectedItem,newPosSelectedItem);
    mOldPosSelectedItem = newPosSelectedItem;
}

void AzGraphicsScene::endMovingItems() {

}

void AzGraphicsScene::drawForeground(QPainter *painter, const QRectF &rect) {
    QGraphicsScene::drawForeground(painter, rect);
}



