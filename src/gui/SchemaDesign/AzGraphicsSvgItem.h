#ifndef AZGRAPHICSSVGITEM_H
#define AZGRAPHICSSVGITEM_H

#include <QString>
#include <QGraphicsSvgItem>

class AzGraphicsSvgItem : public QGraphicsSvgItem {
public:
    AzGraphicsSvgItem(QGraphicsItem * = 0);
    AzGraphicsSvgItem(const QString &, QGraphicsItem * = 0);
    ~AzGraphicsSvgItem();

    QByteArray mapTo();
    AzGraphicsSvgItem *clone(QGraphicsItem* = 0);
    void fixTransofm();

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
private:
};

#endif // AZGRAPHICSSVGITEM_H
