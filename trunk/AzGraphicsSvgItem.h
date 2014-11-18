#ifndef AZGRAPHICSSVGITEM_H
#define AZGRAPHICSSVGITEM_H

#include <QGraphicsSvgItem>


class AzGraphicsSvgItem : public QGraphicsSvgItem
{
public:
    AzGraphicsSvgItem(const QString&);
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
private:
    int i;
};

#endif // AZGRAPHICSSVGITEM_H
