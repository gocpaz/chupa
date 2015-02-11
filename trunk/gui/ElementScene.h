#ifndef ELEMENTSCENE_H
#define ELEMENTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "AzGraphicsSelectedItemArrow.h"
#include "AzGraphicsPoligonItem.h"

class ElementScene : public QGraphicsScene
{
public:
    explicit ElementScene(QObject *parent = 0);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void drawForeground(QPainter *painter, const QRectF &rect);
private:
    void enterMoving();
    void endMoving();
    bool mIsMovingItems;
};

#endif // ELEMENTSCENE_H
