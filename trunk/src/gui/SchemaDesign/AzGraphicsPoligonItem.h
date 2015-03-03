#ifndef AZGRAPHICSPOLIGONITEM_H
#define AZGRAPHICSPOLIGONITEM_H

#include <QGraphicsPolygonItem>



class AzGraphicsPoligonItem: public QGraphicsPolygonItem {
public:
    AzGraphicsPoligonItem(QGraphicsItem * parent = 0);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:

};



#endif // AZGRAPHICSPOLIGONITEM_H
