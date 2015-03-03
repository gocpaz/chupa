#ifndef AZGRAPHICSSELECTEDITEMARROW_H
#define AZGRAPHICSSELECTEDITEMARROW_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMouseEvent>

//#define DEBUG_USER_TRANSFORM
//#define DEBUG_ROTATE



class AzGraphicsView;

class AzGraphicsSelectedItemArrow
{
public:

    enum Side{
        SPos     = 0,
        SEPos    = 1,
        EPos     = 2,
        NEPos    = 3,
        NPos     = 4,
        NWPos    = 5,
        WPos     = 6,
        SWPos    = 7,
        NotArrow = 8
    };

    enum ArrowType {
        ArrowTransform = 1,
        ArrowRotate    = 2
    };

   AzGraphicsSelectedItemArrow(AzGraphicsView *);
   ~AzGraphicsSelectedItemArrow(){}

   void show(QPainter* painter,const QRectF);

   void setArrowType(ArrowType);

   void mousePressEvent(QMouseEvent * event);
   void mouseReleaseEvent(QMouseEvent * event);
   void mouseMoveEvent(QMouseEvent * event);


   void selectionChanged();
   inline Side activeArrow() const                     { return mSideLight; }

   QRect itemViewRect(QGraphicsItem *) const;
   QGraphicsItem* selectedItem() const;
private:
    AzGraphicsSelectedItemArrow(const AzGraphicsSelectedItemArrow&);
    void operator = (const AzGraphicsSelectedItemArrow&) {}

    void mouseClickOnSelectedItemEvent();
    Side containsPoint(const QPoint&);

    QVector<int> containsArrows(const QRect&);
    void mouseMoveScale(const QPoint&);
    void mouseMoveRotate(const QPoint&);
    void calcArrows();

    QRectF boundingRectToScene() const;
    bool isHasSelectedItem()const;

    void lightArrow(const QPoint&);
    void hoverEnterEvent();
    void hoverLeaveEvent();

    AzGraphicsView *mView;
    Side mSideLight;

    ArrowType mArrowType;

    QRect mStartTransformRect;
    Qt::MouseButton mMouseButton;

    QPoint mStartMousePos; /*! for mouseClick detect and arrow ofset */

    QGraphicsItem *mPreviosSelectedItem; //for detect click on (already) selected item
    QPoint mMouseOffset; //offset from arrow to point on selected item

    Side mActiveTransformArrow;

    QVector <QPolygon> mArrows;
    QRect mBoundingRect;

    QRect mOldRectSelectedItem; //for detect change pos or size change. Вообще проверить что и зачем

    bool mIsShowing; //отображать или нет стрелы  - тех. переменная
#ifdef DEBUG_ROTATE
    QPolygonF mDebugRotatePolygon;
    QRectF mDebugCircle;
#endif

};
#endif // AZGRAPHICSSELECTEDITEMARROW_H
