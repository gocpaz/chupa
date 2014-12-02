#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *item){
    QPointF posi = item->pos();
    QRectF bound = item->boundingRect();
}
