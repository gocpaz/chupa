#include "AzGraphicsSvgItem.h"

#include <QDebug>

AzGraphicsSvgItem::AzGraphicsSvgItem(const QString& fileName): QGraphicsSvgItem(fileName) {
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
}
