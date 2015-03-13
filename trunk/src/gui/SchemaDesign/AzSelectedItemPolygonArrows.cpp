#include "AzSelectedItemPolygonArrows.h"

#include <QDebug>
#include <QtMath>

const int ARROWS_COUNT = 8; //in a past life MACROS

const AzOriginalItemPolygonArrows AzSelectedItemPolygonArrows::originalPolygons;
const QSize minimumEffectiveTransformSize(10,10); //very small scale - lost precession (integer) - need more length


AzOriginalItemPolygonArrows::AzOriginalItemPolygonArrows():mZeroPointScalePolygons(ARROWS_COUNT),mZeroPointRotatePolygons(ARROWS_COUNT) {
    calcZeroPolygons();
}

const int tipWidth      = 4; //half size
const int tipHeight     = 6;
const int shaftWidth    = 1;
const int shaftHeight   = 4;

const int shaftRadius = 5;

QPolygon AzOriginalItemPolygonArrows::originalScalePolygon() {
    QPolygon res;
    res << QPoint(0,0)
        << QPoint(tipWidth,tipHeight) << QPoint(shaftWidth,tipHeight)<< QPoint(shaftWidth,shaftHeight+tipHeight)  << QPoint(tipWidth,shaftHeight+tipHeight)
        << QPoint(0,2*tipHeight+shaftHeight);
    for (int i = 4; i > 0; --i) { //mirror half arrow
       res << QPoint(res[i].x() * -1,res[i].y());
    }
    return res;
}


/*!
    Arrow with point (0,0)
 */
QPolygon AzOriginalItemPolygonArrows::originalRotatePolygon() {
    QPolygon res;
    res << QPoint(shaftWidth,tipHeight) << QPoint(tipWidth,tipHeight) << QPoint(0,0)
        << QPoint(-tipWidth,tipHeight)<< QPoint(-shaftWidth,tipHeight);

    res << res.last() + QPoint(0,shaftRadius);
    res << res.last() + QPoint(-shaftRadius/2,shaftRadius/2);
    res << res.last() + QPoint(-shaftRadius,0);

    res << res.last() + QPoint(0,-(tipWidth-shaftWidth)); //check point
    res << res.last() + QPoint(-tipHeight,tipWidth);
    res << res.last() + QPoint(tipHeight,tipWidth);
    res << res.last() + QPoint(0,-(tipWidth-shaftWidth));                ;

    int x = res[3].x();
    int y = res[3].y();
    int lenghtX = x - res[8].x();
    int lenghtY = y - res[8].y();

    QTransform trans;
    trans.translate(-x+lenghtX/2,-y + lenghtY/2); //dirty round
    return trans.map(res);
}

/*!
 * \brief ArrowsPolygon::calcPolygons
 * Вычисляем все стрелочки, а точнее их положение относительно выделеного объекта размером 0х0
 */
void AzOriginalItemPolygonArrows::calcZeroPolygons() {

    const QPolygon scalePolygon = originalScalePolygon();
    const QPolygon rotatePolygon = originalRotatePolygon();

    QTransform trans;
    //calc transform arrow
    mZeroPointScalePolygons[AzTransformArrow::SPos] = scalePolygon; //first pos - original
    for (int i = 1; i < ARROWS_COUNT; ++i) {
        trans.rotate(-45);
        mZeroPointScalePolygons[i] = trans.map(scalePolygon);
    }

    //calc rotate arrow
    trans.reset();
    mZeroPointRotatePolygons[AzTransformArrow::SEPos] = rotatePolygon;
    for (int i = 3; i < ARROWS_COUNT;i+=2) { //90 degree arrow
        trans.rotate(-90);
        mZeroPointRotatePolygons[i] = trans.map(rotatePolygon);
    }
    const int lenghtArrow = scalePolygon.boundingRect().height() / 2;
    const int offsetArrow = -rotatePolygon.boundingRect().x() - 1; //SO SORRY ( - 1) - int graphics
    trans.reset(); //horisontal & vert arrow
    trans.translate(-lenghtArrow,offsetArrow);
    mZeroPointRotatePolygons[AzTransformArrow::SPos] = trans.map(mZeroPointScalePolygons[AzTransformArrow::EPos]);
    trans.reset();
    trans.translate(lenghtArrow,-offsetArrow);
    mZeroPointRotatePolygons[AzTransformArrow::NPos] = trans.map(mZeroPointScalePolygons[AzTransformArrow::WPos]);
    trans.reset();
    trans.translate(lenghtArrow,-offsetArrow);
    mZeroPointRotatePolygons[AzTransformArrow::EPos] = trans.map(mZeroPointScalePolygons[AzTransformArrow::SPos]);
    trans.reset();
    trans.translate(-lenghtArrow,offsetArrow);
    mZeroPointRotatePolygons[AzTransformArrow::WPos] = trans.map(mZeroPointScalePolygons[AzTransformArrow::NPos]);
}


AzSelectedItemPolygonArrows::AzSelectedItemPolygonArrows():
    mArrowsPolygons(ARROWS_COUNT) {
    mActiveArrow = AzTransformArrow::NotArrow;
    mTransformArrow = AzTransformArrow::NotArrow;
    mTransformType = AzTransformArrow::ScaleTransform;
}

AzSelectedItemPolygonArrows::~AzSelectedItemPolygonArrows() {
}


void AzSelectedItemPolygonArrows::setRect(const QRect &rect) {
  if (mItemRect == rect)
      return;
  mItemRect = rect;
  calcPolygons();
}

/*!
   return base point arrow releative mItemRect
 */
QPoint AzSelectedItemPolygonArrows::basePoint(AzTransformArrow::Side side) {
    QPoint point;
    switch (side) {
        case AzTransformArrow::SPos: point = QPoint(mItemRect.center().x(),mItemRect.bottom());     break;
        case AzTransformArrow::SEPos:point = mItemRect.bottomRight();                               break;
        case AzTransformArrow::EPos: point = QPoint(mItemRect.right(),mItemRect.center().y());      break;
        case AzTransformArrow::NEPos:point = mItemRect.topRight();                                  break;
        case AzTransformArrow::NPos: point = QPoint(mItemRect.center().x(),mItemRect.top());        break;
        case AzTransformArrow::NWPos:point = mItemRect.topLeft();                                   break;
        case AzTransformArrow::WPos: point = QPoint(mItemRect.left(),mItemRect.center().y());       break;
        case AzTransformArrow::SWPos:point = mItemRect.bottomLeft();                                break;
    default:
        point = QPoint(0,0);
    }
    return point;
}

/*!
 * calculate arrows around mItemRect
 */
void AzSelectedItemPolygonArrows::calcPolygons() {
   QPoint coordArrow;
   for (int i = 0; i < ARROWS_COUNT; ++i) {
       coordArrow = basePoint((AzTransformArrow::Side)i);
       QPolygon res = (mTransformType == AzTransformArrow::ScaleTransform) ? originalPolygons.scalePolygons()[i]: originalPolygons.rotatePolygons()[i];
       res.translate(coordArrow); // translete to base point arrow
       mArrowsPolygons[i] = res;
   }

   //calc bounding rect arrows
   QPoint point = QPoint(mArrowsPolygons[AzTransformArrow::WPos].boundingRect().left(),mArrowsPolygons[AzTransformArrow::NPos].boundingRect().top());
   mBoundingRect.setTopLeft(point);
   point = QPoint(mArrowsPolygons[AzTransformArrow::EPos].boundingRect().right(),mArrowsPolygons[AzTransformArrow::SPos].boundingRect().bottom());
   mBoundingRect.setBottomRight(point);
}

/*!
    Change arrow type and recalc arrow
 */
void AzSelectedItemPolygonArrows::setTransformType(AzTransformArrow::TransformType type) {
    if (mTransformType == type)
        return;
    mTransformType = type;
    calcPolygons();
}

/*!

  return arrow, that containts \a point.
 */
AzTransformArrow::Side AzSelectedItemPolygonArrows::containsPoint(const QPoint& point) {
    if (!boundingRect().contains(point))
        return AzTransformArrow::NotArrow;
    for (int i = 0; i < ARROWS_COUNT; ++i) {
        if (mArrowsPolygons[i].boundingRect().contains(point))
            return (AzTransformArrow::Side)i;
    }
    return AzTransformArrow::NotArrow;
}

/*!
 * \brief AzGraphicsSelectedItemArrow::containsArrows
 * \param rec область
 * \return Возвращает массив "стрелок", которые попадают в \a rec
 */
QVector<int> AzSelectedItemPolygonArrows::containsArrows(const QRect& rec) {
    QVector<int> res;
    for (int i = 0; i < ARROWS_COUNT; ++i) {
        if (rec.intersects(mArrowsPolygons[i].boundingRect()))
            res.append(i);
    }
    return res;
}

/*!
 * start begin scale - fixing starting position, where pos - mouse pos
 * \sa endScale \sa newScalePos
 */
void AzSelectedItemPolygonArrows::beginTransform(const QPoint& pos) {
    if (mActiveArrow != AzTransformArrow::NotArrow) {
        mTransformArrow = mActiveArrow;  //set transform state arrow
        mRotateBasePointArrow = mActiveArrow;   //for rotate only
        mStartTransformRect = mItemRect;
        //calc mouse ofset arrow
        switch (mTransformArrow) {
            case AzTransformArrow::SPos:  mMouseOffset = QPoint(0,mItemRect.bottom() - pos.y());break;
            case AzTransformArrow::SEPos: mMouseOffset = QPoint(mItemRect.right() - pos.x(),mItemRect.bottom() - pos.y()); break;
            case AzTransformArrow::EPos:  mMouseOffset = QPoint(mItemRect.right() - pos.x(),0); break;
            case AzTransformArrow::NEPos: mMouseOffset = QPoint(mItemRect.right() - pos.x(),mItemRect.top() - pos.y()); break;
            case AzTransformArrow::NPos:  mMouseOffset = QPoint(0,mItemRect.top() - pos.y()); break;
            case AzTransformArrow::NWPos: mMouseOffset = QPoint(mItemRect.left() - pos.x(),mItemRect.top() - pos.y()); break;
            case AzTransformArrow::WPos:  mMouseOffset = QPoint(mItemRect.left() - pos.x(),0); break;
            case AzTransformArrow::SWPos: mMouseOffset = QPoint(mItemRect.left() - pos.x(),mItemRect.bottom() - pos.y()); break;
        default:
            mMouseOffset = QPoint(0,0);
        }

        //lost precession
        if (mStartTransformRect.height() < minimumEffectiveTransformSize.height())
            mMouseOffset.setY(0);
        if (mStartTransformRect.width() < minimumEffectiveTransformSize.width())
            mMouseOffset.setX(0);
    }
}

/*!
    reset value
 */
void AzSelectedItemPolygonArrows::endTransform() {
    mMouseOffset = QPoint(0,0);
    mTransformArrow = AzTransformArrow::NotArrow;
}

/*!
 * calculate new scaled rectangle with newPos
 * return QRect with scaled height,width and pos offset original rect
 */
QRect AzSelectedItemPolygonArrows::newScale(const QPoint& newPos) {
    QSize startSize = mStartTransformRect.size(); //orininal size
    QSize scaleSize = startSize;//keep original pos - do not need calculate in some case
    QPoint scalePos;

    switch (mTransformArrow) {
        case AzTransformArrow::SPos:   scaleSize.setHeight(newPos.y() - mStartTransformRect.y());    break;
        case AzTransformArrow::SEPos:  scaleSize.setWidth(newPos.x() - mStartTransformRect.x()); scaleSize.setHeight(newPos.y() - mStartTransformRect.y());   break;
        case AzTransformArrow::EPos:   scaleSize.setWidth(newPos.x() - mStartTransformRect.x());   break;
        case AzTransformArrow::NEPos:
            scaleSize.setWidth(newPos.x() - mStartTransformRect.x()); scaleSize.setHeight(-newPos.y() + mStartTransformRect.bottom());
            scalePos.setY(startSize.height() - scaleSize.height());
            break;
        case AzTransformArrow::NPos:
            scaleSize.setHeight(-newPos.y() + mStartTransformRect.bottom());
            scalePos.setY(startSize.height() - scaleSize.height());
            break;
        case AzTransformArrow::NWPos:
            scaleSize.setHeight(-newPos.y() + mStartTransformRect.bottom());scaleSize.setWidth(-newPos.x() + mStartTransformRect.right());
            scalePos.setX(startSize.width() - scaleSize.width());
            scalePos.setY(startSize.height() - scaleSize.height());
            break;
        case AzTransformArrow::WPos:
            scaleSize.setWidth(-newPos.x() + mStartTransformRect.right());
            scalePos.setX(startSize.width() - scaleSize.width());
            break;
        case AzTransformArrow::SWPos:
            scaleSize.setHeight(newPos.y() - mStartTransformRect.y());scaleSize.setWidth(-newPos.x() + mStartTransformRect.right());
            scalePos.setX(startSize.width() - scaleSize.width());
            break;
    default:
            break;
    }
    return QRect(scalePos,scaleSize);
}

/*!
 *
 * calc angle with new pos mouse and return angle
 */
qreal AzSelectedItemPolygonArrows::newRotate(const QPoint& newPos) {

    QPoint basePoint = this->basePoint(mRotateBasePointArrow); //base point rectangle
    const QPoint centerPoint = mStartTransformRect.center();    //center point rectangle
    QLineF lineRadius = QLineF(basePoint,centerPoint);
    QLineF lineMouse = QLineF(centerPoint,newPos);
    qreal radian = qDegreesToRadians(lineMouse.angle()); //degree to X axist
    qreal r = lineRadius.length();
    qreal dx = qCos(radian) * r;
    qreal dy = qSin(radian) * r;

    const QPoint deltaPoint(centerPoint.x() + dx, centerPoint.y() - dy); //delta point rectangle
    QLineF lineDelta = QLineF(basePoint,deltaPoint);

    //clockwise  or not (multiply vector)
    int clockWise =  (basePoint.x() - centerPoint.x()) * (deltaPoint.y() - centerPoint.y()) - (basePoint.y() - centerPoint.y()) * (deltaPoint.x() - centerPoint.x()) > 0 ? 1 : -1;

    qreal angle = clockWise * 2 * qAsin(lineDelta.length()/2/r);
    if (angle != angle) //NaN (impossible?)
        angle = M_PI;


    int arrowOffsetFromOriginial =  mTransformArrow - mRotateBasePointArrow;
    qreal offsetAngle = arrowOffsetFromOriginial *M_PI_4; // * 45 degree


    if (angle > M_PI_2) {//if more that 90 - change base point (180 degree trouble) - for next iteration
         mRotateBasePointArrow-= clockWise*2;        //set base point +- 90 degree (1 arrow - 45 degree)
    }
    return angle + offsetAngle;
}


/*!
 *  return true is arrow located on the centre item rectangle
*/
bool AzSelectedItemPolygonArrows::isCenterArrow(AzTransformArrow::Side side) const {
    if (side == AzTransformArrow::NotArrow)
        return false;
    switch (side) {
        case AzTransformArrow::SPos:
        case AzTransformArrow::EPos:
        case AzTransformArrow::NPos:
        case AzTransformArrow::WPos:
        return true;
    default:
        return false;
    }
}

AzTransformArrow::Side& operator +=(AzTransformArrow::Side& a, int b) {
    int sum = (int)a + b % ARROWS_COUNT;
    if (sum >= ARROWS_COUNT)
        sum = sum - ARROWS_COUNT;
    a = (AzTransformArrow::Side)sum;
    return a;
}

AzTransformArrow::Side& operator -=(AzTransformArrow::Side& a, int b) {
    int sum = (int)a - b % ARROWS_COUNT;
    if (sum < 0)
        sum = ARROWS_COUNT + sum;
    a = (AzTransformArrow::Side)sum;
    return a;
}

AzTransformArrow::Side operator +(const AzTransformArrow::Side& a, int b) {
    int sum = (int)a + b % ARROWS_COUNT;
    if (sum >= ARROWS_COUNT)
        sum = sum - ARROWS_COUNT;
    return (AzTransformArrow::Side)sum;
}

AzTransformArrow::Side operator -(const AzTransformArrow::Side& a, int b) {
    int sum = (int)a - b % ARROWS_COUNT;
    if (sum < 0)
        sum = ARROWS_COUNT + sum;
    return (AzTransformArrow::Side)sum;
}
