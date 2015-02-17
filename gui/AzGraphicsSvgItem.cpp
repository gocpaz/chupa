#include "AzGraphicsSvgItem.h"

#include <QDebug>

AzGraphicsSvgItem::AzGraphicsSvgItem(const QString& fileName): QGraphicsSvgItem(fileName) {
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
}

QVariant AzGraphicsSvgItem::itemChange(GraphicsItemChange change, const QVariant & value) {
    return QGraphicsSvgItem::itemChange(change,value);
}
