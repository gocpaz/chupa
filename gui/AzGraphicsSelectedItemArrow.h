#ifndef AZGRAPHICSSELECTEDITEMARROW_H
#define AZGRAPHICSSELECTEDITEMARROW_H
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class AzGraphicsSelectedItemArrow
{
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
   void show(QPainter* painter, QGraphicsItem* item);
   QPolygon static arrowPolygon();
   QPoint arrowPos(SideLight) const;
private:
    AzGraphicsSelectedItemArrow(const AzGraphicsSelectedItemArrow&);
    AzGraphicsSelectedItemArrow& operator = (const AzGraphicsSelectedItemArrow&){}
    QGraphicsScene *mScene;
};
#endif // AZGRAPHICSSELECTEDITEMARROW_H