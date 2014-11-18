#ifndef ELEMENTVIEW_H
#define ELEMENTVIEW_H

#include <QGraphicsView>

#include "AzGrpahicsTransformArrowItem.h"

class ElementView : public QGraphicsView
{
public:
    ElementView(QGraphicsScene * scene, QWidget * parent = 0);
private:
};

#endif // ELEMENTVIEW_H
