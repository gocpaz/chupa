#ifndef AZGRPAHICSTRANSFORMARROWITEM_H
#define AZGRPAHICSTRANSFORMARROWITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <QVector>

class AzGrpahicsTransformArrowItem;

class AzGrpahicsTransformArrowItems: public QObject {
Q_OBJECT
public:
    AzGrpahicsTransformArrowItems(QGraphicsScene *,QObject* = 0);
    ~AzGrpahicsTransformArrowItems();
    void setScene(QGraphicsScene*);
    inline QGraphicsScene* scene()               { return mScene; }
public slots:
    void show();
    void hide();
private:
    QGraphicsScene *mScene;
    QVector <AzGrpahicsTransformArrowItem*> mItems;
    QList <QGraphicsItem*> mSelectedItems;
};

class AzGrpahicsTransformArrowItem : public QGraphicsPolygonItem
{
public:
    enum Figure {
        ArrowLenght = 4,
        ArrowWidht = 12,
        ShaftLenght = 12,
        ShaftWidth  = 4,
        LinePen = 2
    };

    enum ArrowPos {
        NPos = 0,
        NEPos = 1,
        EPos = 2,
        SEPos = 3,
        SPos = 4,
        SWPos = 5,
        WPos = 6,
        NWPos = 7
    };

    AzGrpahicsTransformArrowItem(ArrowPos arrow,AzGrpahicsTransformArrowItems*);
    ~AzGrpahicsTransformArrowItem();
    void setArrow(ArrowPos);
    void show();
    void hide();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    static QPolygon sprite();
    void reset();
    ArrowPos  mArrow;
    bool mIsShow;
    AzGrpahicsTransformArrowItems *mCollection;
};

#endif // AZGRPAHICSTRANSFORMARROWITEM_H
