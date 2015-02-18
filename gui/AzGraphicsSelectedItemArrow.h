#ifndef AZGRAPHICSSELECTEDITEMARROW_H
#define AZGRAPHICSSELECTEDITEMARROW_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMouseEvent>

//#define VIEW_PAINT_RECT_DEBUG
#define DEBUG_USER_TRANSFORM

class AzGraphicsView;

class AzGraphicsSelectedItemArrow
{
public:

    enum Side{
        NPos     = 0,
        NEPos    = 1,
        EPos     = 2,
        SEPos    = 3,
        SPos     = 4,
        SWPos    = 5,
        WPos     = 6,
        NWPos    = 7,
        NotArrow = 8
    };
   AzGraphicsSelectedItemArrow(AzGraphicsView *);
   ~AzGraphicsSelectedItemArrow(){}

   void show(QPainter* painter,const QRectF);

   void mousePressEvent(QMouseEvent * event);
   void mouseReleaseEvent(QMouseEvent * event);
   void mouseMoveEvent(QMouseEvent * event);


   void selectionChanged();
   void itemMoved(const QPoint&,const QPoint&);
   inline Side activeArrow() const                     { return mSideLight; }
   QPolygon& arrow(Side val)                           { return mArrows[val]; }
   QRect itemRect(QGraphicsItem *) const;
   QRect itemRect() const                              { return isHasSelectedItem() ? itemRect(selectedItem()) : QRect();}
   QGraphicsItem* selectedItem() const;
private:
    AzGraphicsSelectedItemArrow(const AzGraphicsSelectedItemArrow&);
    AzGraphicsSelectedItemArrow& operator = (const AzGraphicsSelectedItemArrow&){}

    Side containsPoint(const QPoint&);
    QVector<int> containsArrows(const QRect&);

    void calcArrows();
    void calcArrows(const QRect&);

    void setCursor(Side);

    QRectF boundingRectToScene() const;
    bool isHasSelectedItem()const;

    void hoverEnterEvent();
    void hoverLeaveEvent();

    Side mSideLight;
    AzGraphicsView *mView;
    QVector <QPolygon> mArrows;

    Qt::MouseButton mMouseButton;
    int mOffsetMousePos;
    QRect mOldItemRect;
    Side mActiveTransformArrow;

    QRect mBoundingRect;
    QRect mOldRectSelectedItem;
    bool mIsShowing; //убрать, mIsShowing = isHasSelectedItem()
#ifdef VIEW_PAINT_RECT_DEBUG
    QVector <QRectF> mDebugRects;
    QRectF mDebugRect;
    void showPaintRectDebug(QPainter*);
    int mPaintCount;
#endif
};
#endif // AZGRAPHICSSELECTEDITEMARROW_H
