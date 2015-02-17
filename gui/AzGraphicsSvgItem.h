#ifndef AZGRAPHICSSVGITEM_H
#define AZGRAPHICSSVGITEM_H

#include <QGraphicsSvgItem>


class AzGraphicsSvgItem : public QGraphicsSvgItem
{
public:
    AzGraphicsSvgItem(const QString&);
protected:
    virtual QVariant itemChange(GraphicsItemChange, const QVariant &);
private:
};

#endif // AZGRAPHICSSVGITEM_H
