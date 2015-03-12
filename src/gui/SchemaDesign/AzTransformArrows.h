#ifndef AZTRANSFORMARROWS_H
#define AZTRANSFORMARROWS_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMouseEvent>

class AzGraphicsView;
class AzSelectedItemPolygonArrows;

class AzTransformArrow {
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

    enum TransformType { //need change tranfsofmTypeToString if extended
        ScaleTransform       = 0,
        RotateTransform      = 1,
        NoTransform          = 2
    };

    AzTransformArrow(AzGraphicsView *);
    ~AzTransformArrow();

    void show(QPainter* painter,const QRectF);

    void setTransformType(TransformType);

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);


    void selectionChanged();

    QRect itemViewRect(QGraphicsItem *) const;
    QGraphicsItem* selectedItem() const;

    static QString sideToString(Side side);
    static QString tranfsofmTypeToString(TransformType type);
private:
    AzTransformArrow(const AzTransformArrow&);
    void operator = (const AzTransformArrow&) {}

    void mouseClickOnSelectedItemEvent();

    void mouseMoveScale(const QPoint&);
    void mouseMoveRotate(const QPoint&);

    QRectF boundingRectToScene() const;
    bool isHasSelectedItem() const;

    void lightArrow(const QPoint&);
    void hoverEnterEvent();
    void hoverLeaveEvent();

    AzGraphicsView *mView;
    AzSelectedItemPolygonArrows *mArrows;

    Qt::MouseButton mMouseButton;
    QPoint mStartMousePos; /*! for mouseClick detect */
    QGraphicsItem *mPreviosSelectedItem; //for detect click on (already) selected item

    QRect mOldRect; //for detect change pos or size change.

#ifdef DEBUG_ROTATE
    QPolygonF mDebugRotatePolygon;
    QRectF mDebugCircle;
#endif


};




#endif // AZTRANSFORMARROWS_H
