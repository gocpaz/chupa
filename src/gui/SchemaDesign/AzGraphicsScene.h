#ifndef AZGRAPHICSSCENE_H
#define AZGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "AzGraphicsSelectedItemArrow.h"
#include "AzGraphicsPoligonItem.h"

class AzGraphicsScene : public QGraphicsScene
{
Q_OBJECT
public:
    AzGraphicsScene(QObject *parent = 0);
signals:
    void moveItemEvent(const QPointF&,const QPointF&);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void drawForeground(QPainter *painter, const QRectF &rect);
private:
    void enterMovingItems();
    void endMovingItems();
    void movingItems();
    bool mIsMovingItems;
    QPointF mOldPosSelectedItem;
};

#endif // ELEMENTSCENE_H
