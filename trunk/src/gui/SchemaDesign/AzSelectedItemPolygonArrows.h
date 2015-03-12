#ifndef AZSELECTEDITEMPOLYGONSARROWS_H
#define AZSELECTEDITEMPOLYGONSARROWS_H

#include "AzTransformArrows.h"

typedef QVector<QPolygon> PolygonVector;

class AzOriginalItemPolygonArrows {
public:
    AzOriginalItemPolygonArrows();
    inline const PolygonVector& scalePolygons() const           { return mZeroPointScalePolygons; }
    inline const PolygonVector& rotatePolygons() const          { return mZeroPointRotatePolygons; }
private:
    static QPolygon originalScalePolygon();
    static QPolygon originalRotatePolygon();
    void calcZeroPolygons();
    PolygonVector mZeroPointScalePolygons;
    PolygonVector mZeroPointRotatePolygons;
};


class AzSelectedItemPolygonArrows
{
public:

    AzSelectedItemPolygonArrows();
    ~AzSelectedItemPolygonArrows();

    static const AzOriginalItemPolygonArrows originalPolygons;

    inline void setActiveArrow(AzTransformArrow::Side side)                    { mActiveArrow = side; }
    inline AzTransformArrow::Side activeArrow() const                          { return mActiveArrow; }

    inline void setActiveTransformArrow(AzTransformArrow::Side side)           { mTransformArrow = side; }
    inline AzTransformArrow::Side activeTransformArrow() const                 { return mTransformArrow; }

    void setRect(const QRect& rect);
    void setTransformType(AzTransformArrow::TransformType);
    inline AzTransformArrow::TransformType transformType() const    { return mTransformType; }

    void beginTransform(const QPoint&);
    QSizeF newScale(const QPoint&);
    qreal newRotate(const QPoint&);
    //qreal newShear(const QPoint&);
    void endTransform();

    QPoint mouseOffset() const                                          { return mMouseOffset; }

    AzTransformArrow::Side containsPoint(const QPoint&);
    QVector<int> containsArrows(const QRect&);

    inline const QPolygon &arrow(int side) const               { return mArrowsPolygons[side]; } //out of range (NotArrow)
    inline const QRect& boundingRect() const                   { return mBoundingRect;         }
private:
    AzTransformArrow::Side mActiveArrow;                        /*! Active selected arrow */
    AzTransformArrow::Side mTransformArrow;                     /*! Active arrow if start transform */

    AzTransformArrow::Side mRotateBasePointArrow;               /*! flowing arrow for soft migrate 180 angle in graphics  */


    PolygonVector mArrowsPolygons;
    QRect mItemRect;        /*! rect selected  item   */
    QRect mBoundingRect;    /*! bounding rect all arrows */
    QRect mStartTransformRect; /*! start rect before transform */
    QPoint mMouseOffset; /*! offset from point arrow to point on selected item */
    AzTransformArrow::TransformType mTransformType;

    QPoint basePoint(AzTransformArrow::Side);
    bool isCenterArrow(AzTransformArrow::Side) const;

    void calcPolygons();
};

AzTransformArrow::Side& operator +=(AzTransformArrow::Side& a, int b);
AzTransformArrow::Side& operator -=(AzTransformArrow::Side& a, int b);

AzTransformArrow::Side operator +(const AzTransformArrow::Side& a,int b);
AzTransformArrow::Side operator -(const AzTransformArrow::Side& a,int b);


#endif // AZSELECTEDITEMPOLYGONSARROWS_H
