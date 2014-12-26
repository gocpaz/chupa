#ifndef AZGRAPHICSSELECTEDITEMARROW_H
#define AZGRAPHICSSELECTEDITEMARROW_H
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class AzGraphicsSelectedItemArrow: public QObject
{
Q_OBJECT
public:
    enum SideLight{
        Npos = 0,
        Spos = 1,
        Epos = 2,
        Wpos = 3,
        NEpos = 4,
        NWpos = 5,
        SEpos = 6,
        SWpos = 7,
        NotArrow = 8
    };
   AzGraphicsSelectedItemArrow(QGraphicsScene *);
   ~AzGraphicsSelectedItemArrow(){}
   void show(QPainter* painter);
   void mouseMoveEvent(QGraphicsSceneMouseEvent* mauseEvent);
private slots:
    void itemSelectionChanged();
private:
    SideLight mSideLight;
    QGraphicsScene *mScene;
    AzGraphicsSelectedItemArrow(const AzGraphicsSelectedItemArrow&);
    AzGraphicsSelectedItemArrow& operator = (const AzGraphicsSelectedItemArrow&){}
    QPolygonF arrowPolygon(SideLight);
    QPointF arrowPos(SideLight) const;
    SideLight containsPoint(QGraphicsSceneMouseEvent* mauseEvent);
    void hoverEnterEvent();
    void hoverLeaveEvent();
    inline bool isHasSelectedItem()const { return mScene->selectedItems().size() > 0;}
    QGraphicsItem* selectedItem() const;
    QRectF boundingRect(SideLight)const;
};
#endif // AZGRAPHICSSELECTEDITEMARROW_H
