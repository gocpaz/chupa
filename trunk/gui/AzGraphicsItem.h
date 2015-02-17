#ifndef AZGRAPHICSITEM_H
#define AZGRAPHICSITEM_H

#include "QGraphicsItem"

class AzGraphicsItem : public QGraphicsItem
{
public:
    AzGraphicsItem();
    ~AzGraphicsItem();
    QRectF boundingRect() const;
    void	paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private:
    QRect mRect;
};

#endif // AZGRAPHICSITEM_H
