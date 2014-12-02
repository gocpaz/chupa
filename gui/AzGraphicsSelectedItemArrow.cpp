#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *item){
    QPointF posi = item->pos();
    QPointF bound = item->boundingRect();
}
