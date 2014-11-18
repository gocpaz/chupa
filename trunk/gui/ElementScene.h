#ifndef ELEMENTSCENE_H
#define ELEMENTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>

class ElementScene : public QGraphicsScene
{
public:
    explicit ElementScene(QObject *parent = 0);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
private:
    void enterMoving();
    void endMoving();
    bool mIsMovingItems;
};

#endif // ELEMENTSCENE_H
