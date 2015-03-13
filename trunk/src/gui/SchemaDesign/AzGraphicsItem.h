#ifndef AZGRAPHICSITEM_H
#define AZGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QDebug>

class AzGraphicsItem {
public:
    AzGraphicsItem() {}
    virtual void fixTransform() = 0;
    virtual ~AzGraphicsItem() {}
};

class AzGraphicsItemGroup : public QGraphicsItemGroup, public AzGraphicsItem {
public:
    AzGraphicsItemGroup() {}
    AzGraphicsItemGroup(QGraphicsItem *);
    virtual void fixTransform() { qDebug() << "Group fix"; }
};


class AzGraphicsSvgItem : public QGraphicsSvgItem, public AzGraphicsItem {
public:
    AzGraphicsSvgItem(QGraphicsItem * = 0);
    AzGraphicsSvgItem(const QString &, QGraphicsItem * = 0);
    ~AzGraphicsSvgItem();

    QByteArray mapTo();
    AzGraphicsSvgItem *clone(QGraphicsItem* = 0);
    virtual void fixTransform();

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private:
};

#endif // AZGRAPHICSITEM_H
