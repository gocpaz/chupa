#include "AzGrpahicsTransformArrowItem.h"

#include <QDebug>
#include <QBrush>
#include <QtMath>

AzGrpahicsTransformArrowItems::AzGrpahicsTransformArrowItems(QGraphicsScene *scene,QObject *parent):
    QObject(parent),mScene(0),mItems(8) {
    for (int i = 0; i < mItems.size();++i) {
        mItems[i] = new AzGrpahicsTransformArrowItem(AzGrpahicsTransformArrowItem::ArrowPos(i),this);
    }
    setScene(scene);

}

AzGrpahicsTransformArrowItems::~AzGrpahicsTransformArrowItems() {

}

void AzGrpahicsTransformArrowItems::setScene(QGraphicsScene* scene) {
    if (mScene == scene)
        return;
    if (scene) {
        connect(scene,SIGNAL(selectionChanged()),this,SLOT(show()));
//        for (int i = 0; i < mItems.size();++i) {
//            scene->addItem(mItems[i]);
//        }
    }
    if (mScene) {
        disconnect(mScene,SIGNAL(selectionChanged()),this,SLOT(show()));
//        for (int i = 0; i < mItems.size();++i) {
//            scene->removeItem(mItems[i]);
//        }
    }
    mScene = scene;
}

void AzGrpahicsTransformArrowItems::show() {
    QList<QGraphicsItem *> selectedItems =  mScene->selectedItems();
    hide();
    if (selectedItems.size() > 0) {
        for (int i = 0; i < mItems.size();++i) {
            mItems[i]->show();
        }
    } else {
        hide();
    }
}


void AzGrpahicsTransformArrowItems::hide() {
    for (int i = 0; i <mItems.size();++i) {
        mItems[i]->hide();
    }
}



AzGrpahicsTransformArrowItem::AzGrpahicsTransformArrowItem(ArrowPos arrow,AzGrpahicsTransformArrowItems *collection):
    QGraphicsPolygonItem(0),mIsShow(false),mCollection(collection) {
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    setFlag(QGraphicsItem::ItemIsSelectable,false);
    setArrow(arrow);
    reset();
}

AzGrpahicsTransformArrowItem::~AzGrpahicsTransformArrowItem() {

}

void AzGrpahicsTransformArrowItem::reset() {
    setBrush(Qt::black);
}

/*!
 * \brief AzGrpahicsTransformArrowItem::show Отображает "стрелку"
 * Добавляет элемент в сцену и отображает его
 */
void AzGrpahicsTransformArrowItem::show() {
    if (!mIsShow && mCollection && mCollection->scene() && (mCollection->scene()->selectedItems().size() > 0)) {
        QGraphicsItem *selectedItem = mCollection->scene()->selectedItems()[0];
        QPointF selectedPos = selectedItem->pos();
        QRectF selectedRect = selectedItem->boundingRect();

        QPoint arrowPos;
        switch (mArrow) {
        case NPos:
            arrowPos = QPoint(selectedPos.x() + selectedRect.center().x(),selectedPos.y());
            break;
        case NEPos:
            arrowPos = QPoint(selectedPos.x() + selectedRect.width(),selectedPos.y());
            break;
        case EPos:
            arrowPos = QPoint(selectedPos.x() + selectedRect.width(),selectedPos.y() + selectedRect.center().y());
            break;
        case SEPos:
            arrowPos = QPoint(selectedPos.x() + selectedRect.width(),selectedPos.y() + selectedRect.height());
            break;
        case SPos:
            arrowPos = QPoint(selectedPos.x() + selectedRect.center().x(),selectedPos.y() + selectedRect.height());
            break;
        case SWPos:
            arrowPos = QPoint(selectedPos.x(),selectedPos.y() + selectedRect.height());
            break;
        case WPos:
            arrowPos = QPoint(selectedPos.x(),selectedPos.y() + selectedRect.center().y());
            break;
        case NWPos:
            arrowPos = QPoint(selectedPos.x(),selectedPos.y());
            break;
        default:
            break;
        }

        reset();
        setPos(arrowPos);
        mCollection->scene()->addItem(this);
        QGraphicsPolygonItem::show();
        mIsShow = true;
    }
}

void AzGrpahicsTransformArrowItem::hide() {
    if (!mIsShow)
        return;
    mCollection->scene()->removeItem(this);
    mIsShow = false;
}

QPolygon AzGrpahicsTransformArrowItem::sprite() {
    QPolygon polygon;

    int halfArrowWidth = ArrowWidht / 2;
    int halfStaftWidth = ShaftWidth / 2;

    polygon << QPoint(halfArrowWidth,ArrowLenght)
            << QPoint(halfStaftWidth,ArrowLenght)
            << QPoint(halfStaftWidth,ArrowLenght+ShaftLenght)
            << QPoint(halfArrowWidth,ArrowLenght+ShaftLenght)
            << QPoint(0,ArrowLenght+ShaftLenght+ArrowLenght)
            << QPoint(-halfArrowWidth,ArrowLenght+ShaftLenght)
            << QPoint(-halfStaftWidth,ArrowLenght+ShaftLenght)
            << QPoint(-halfStaftWidth,ArrowLenght)
            << QPoint(-halfArrowWidth,ArrowLenght)
            << QPoint(0,0);
    return polygon;
}

void AzGrpahicsTransformArrowItem::setArrow(ArrowPos arrow) {
    mArrow = arrow;
    QPolygon polygon = AzGrpahicsTransformArrowItem::sprite();

    if (mArrow == NPos) {
        for (int i = 0; i < polygon.size();++i) {
            polygon.setPoint(i,polygon.point(i).x(),-polygon.point(i).y());
        }
    }

    if (arrow == EPos) {
        for (int i = 0; i < polygon.size();++i) {
            QPoint point = polygon.point(i);
            int oldX = point.x();
            point.setX(qAbs(point.y()));
            point.setY(oldX);
            polygon.setPoint(i,point);
        }
    }

    if (arrow == WPos) {
        for (int i = 0; i < polygon.size();++i) {
            QPoint point = polygon.point(i);
            int oldX = point.x();
            point.setX(-1*point.y());
            point.setY(oldX);
            polygon.setPoint(i,point);
        }
    }

    if (arrow == SEPos || arrow == NEPos || arrow == SWPos || arrow == NWPos) {
        qreal angle;
        switch (mArrow) {
        case SEPos:
            angle = 45.0;
            break;
        case NEPos:
            angle = 135.0;
            break;
        case SWPos:
            angle = -45;
            break;
        case NWPos:
            angle = -135;
            break;
        default:
            break;
        }

        qreal angleCos = qCos(qDegreesToRadians(angle));
        qreal angleSin = qSin(qDegreesToRadians(angle));
        for (int i = 0; i < polygon.size();++i) {
            QPoint point = polygon.point(i);
            int x = point.x()*angleCos + point.y()*angleSin;
            int y = -point.x()*angleSin + point.y()*angleCos;
            polygon.setPoint(i,x,y);
        }
    }



    setPolygon(polygon);
}


void AzGrpahicsTransformArrowItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mouseMoveEvent(event);
}

void AzGrpahicsTransformArrowItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
    setBrush(QBrush(Qt::yellow));
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void AzGrpahicsTransformArrowItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
    setBrush(Qt::black);
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}
