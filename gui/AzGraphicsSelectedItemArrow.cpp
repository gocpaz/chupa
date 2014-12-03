#include "AzGraphicsSelectedItemArrow.h"
#include <QDebug>

QPolygon AzGraphicsSelectedItemArrow::arrowPolygon(){
    QPolygon poligon;
    poligon << QPoint(5,10)
            << QPoint(-10,30)
            << QPoint(0,30)
            << QPoint(0,50)
            << QPoint(10,50)
            << QPoint(10,30)
            << QPoint(20,30);
    return poligon;
}

void AzGraphicsSelectedItemArrow::show(QPainter *painter, QGraphicsItem *item){
    QPointF posi = item->pos();
    QRectF bound = item->boundingRect();
    int x1 = posi.x();
    int y1 = posi.y();
    int x2 = x1 + bound.width();
    int y2 = y1 + bound.height();
    //painter->drawRect(x1+bound.width()/2,y2,10,20);

    painter->drawPolygon(arrowPolygon(),Qt::WindingFill);
    //qDebug() << ;
}
