#ifndef AZGRAPHICSSELECTEDITEMARROW_H
#define AZGRAPHICSSELECTEDITEMARROW_H
#include <QPainter>
#include <QGraphicsItem>

class AzGraphicsSelectedItemArrow
{
public:
    AzGraphicsSelectedItemArrow(){}
   ~AzGraphicsSelectedItemArrow(){}
   void show(QPainter* painter, QGraphicsItem* item);
   QPolygon static arrowPolygon();
private:
    AzGraphicsSelectedItemArrow(const AzGraphicsSelectedItemArrow&);
    AzGraphicsSelectedItemArrow& operator = (const AzGraphicsSelectedItemArrow&){}
};
#endif // AZGRAPHICSSELECTEDITEMARROW_H
