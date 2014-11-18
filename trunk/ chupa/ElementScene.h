#ifndef ELEMENTSCENE_H
#define ELEMENTSCENE_H

#include <QGraphicsScene>

#include "AzGrpahicsTransformArrowItem.h"

class ElementScene : public QGraphicsScene
{
public:
    explicit ElementScene(QObject *parent = 0);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void focusOutEvent(QFocusEvent * focusEvent);
private:
    void enterMoving();
    void endMoving();
    AzGrpahicsTransformArrowItems mArrows;
    bool mIsMovingItems;
};

#endif // ELEMENTSCENE_H
