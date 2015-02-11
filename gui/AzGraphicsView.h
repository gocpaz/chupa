#ifndef AZGRAPHICSVIEW_H
#define AZGRAPHICSVIEW_H

#include <QGraphicsView>

#include "AzGraphicsSelectedItemArrow.h"


class AzGraphicsView : public QGraphicsView
{
Q_OBJECT
public:
    AzGraphicsView(QGraphicsScene *, QWidget * = 0);
    ~AzGraphicsView();
protected slots:
    void selectionChanged();
protected:
    void drawForeground(QPainter * painter, const QRectF & rect);
private:
    void setScene(QGraphicsScene *);
    AzGraphicsSelectedItemArrow mArrows;
};

#endif // AZGRAPHICSVIEW_H
