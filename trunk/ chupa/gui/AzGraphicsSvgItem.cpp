#include "AzGraphicsSvgItem.h"

#include <QDebug>

AzGraphicsSvgItem::AzGraphicsSvgItem(const QString& fileName): QGraphicsSvgItem(fileName) {
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    i = 0;
}

void AzGraphicsSvgItem::mousePressEvent(QGraphicsSceneMouseEvent * event) {
   QGraphicsSvgItem::mousePressEvent(event);
}

QVariant AzGraphicsSvgItem::itemChange(GraphicsItemChange change, const QVariant & value) {
   if (change == QGraphicsItem::ItemPositionChange) {
       //qDebug() << "Pos change";
   }
   if (change == QGraphicsItem::ItemPositionHasChanged) {
       ++i;
       //qDebug() << "Pos changed" << i;
   }
   return QGraphicsSvgItem::itemChange(change,  value);
}
