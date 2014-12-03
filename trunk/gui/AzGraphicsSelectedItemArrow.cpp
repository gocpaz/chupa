#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *item){
    QPointF posi = item->pos();
    QRectF bound = item->boundingRect();
    int x1 = posi.x();
    int y1 = posi.y();
    int x2 = x1 + bound.width();
    int y2 = y1 + bound.height();
    painter->drawRect(x1+bound.width()/2,y2,10,20);
    //QPolygonF
    //painter->drawPolygon(,0);
    //qDebug() << ;
}
