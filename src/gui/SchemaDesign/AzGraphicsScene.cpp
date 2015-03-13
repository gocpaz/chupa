#include "AzGraphicsScene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "AzTransformArrows.h"
#include "AzGraphicsPoligonItem.h"
#include "AzGraphicsItem.h"


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


/*!
    Create item group  without common parent
    \Warnin  - commented qt code - remove!

*/

 AzGraphicsItemGroup * AzGraphicsScene::createItemGroup(const QList<QGraphicsItem *> & items) {
//    // Build a list of the first item's ancestors
//    QList<QGraphicsItem *> ancestors;
//    int n = 0;
//    if (!items.isEmpty()) {
//        QGraphicsItem *parent = items.at(n++);
//        while ((parent = parent->parentItem()))
//            ancestors.append(parent);
//        }

//         // Find the common ancestor for all items
//         QGraphicsItem *commonAncestor = 0;
//         if (!ancestors.isEmpty()) {
//             while (n < items.size()) {
//                 int commonIndex = -1;
//                 QGraphicsItem *parent = items.at(n++);
//                 do {
//                     int index = ancestors.indexOf(parent, qMax(0, commonIndex));
//                     if (index != -1) {
//                         commonIndex = index;
//                         break;
//                     }
//                 } while ((parent = parent->parentItem()));

//                 if (commonIndex == -1) {
//                     commonAncestor = 0;
//                     break;
//                 }

//                 commonAncestor = ancestors.at(commonIndex);
//             }
//         }

//         // Create a new group at that level
//         AzGraphicsItemGroup *group = new AzGraphicsItemGroup(commonAncestor);
//         if (!commonAncestor)
//             addItem(group);
//         foreach (QGraphicsItem *item, items)
//             group->addToGroup(item);
//         return group;
     AzGraphicsItemGroup *group = new AzGraphicsItemGroup;
     addItem(group);
     foreach (QGraphicsItem *item, items)
     group->addToGroup(item);
     return group;
 }



