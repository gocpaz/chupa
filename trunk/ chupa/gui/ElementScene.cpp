#include "ElementScene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

ElementScene::ElementScene(QObject *parent) :
    QGraphicsScene(parent),mArrows(this),mIsMovingItems(false) {
    //connect (this,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
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

void ElementScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    if (event->button() == Qt::LeftButton && mIsMovingItems) {
        mIsMovingItems = false;
        endMoving();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ElementScene::focusOutEvent(QFocusEvent * focusEvent) {
    QGraphicsScene::focusOutEvent(focusEvent);
}

void ElementScene::enterMoving() {
    mArrows.hide();
}

void ElementScene::endMoving() {
    mArrows.show();
}
