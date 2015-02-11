#ifndef AZGRAPHICSSELECTEDITEMARROW_H
#define AZGRAPHICSSELECTEDITEMARROW_H
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class AzGraphicsView;

class AzGraphicsSelectedItemArrow: public QObject
{
public:

    enum CardinalDirection{
        Npos     = 0,
        NEpos    = 1,
        Epos     = 2,
        SEpos    = 3,
        Spos     = 4,
        SWpos    = 5,
        Wpos     = 6,
        NWpos    = 7,
        NotArrow = 8
    };
   AzGraphicsSelectedItemArrow(AzGraphicsView *);
   ~AzGraphicsSelectedItemArrow(){}

   void show(QPainter* painter,const QRectF);
   void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void selectionChanged();
private:
    AzGraphicsSelectedItemArrow(const AzGraphicsSelectedItemArrow&);
    AzGraphicsSelectedItemArrow& operator = (const AzGraphicsSelectedItemArrow&){}

    CardinalDirection containsPoint(QGraphicsSceneMouseEvent* mouseEvent);
    void calcArrows();

    void hoverEnterEvent();
    void hoverLeaveEvent();

    bool isHasSelectedItem()const;

    QGraphicsItem* selectedItem() const;
    QRect boundingRect() const;
    QRectF sceneBoundingRect() const;

    CardinalDirection mSideLight;
    AzGraphicsView *mView;
    QVector <QPolygon> mArrows;
    bool mIsShowing;
};
#endif // AZGRAPHICSSELECTEDITEMARROW_H
