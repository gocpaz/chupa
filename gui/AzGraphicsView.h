#ifndef AZGRAPHICSVIEW_H
#define AZGRAPHICSVIEW_H

#include <QMouseEvent>
#include <QGraphicsView>

#include "AzGraphicsSelectedItemArrow.h"


class AzGraphicsView : public QGraphicsView
{
Q_OBJECT
public:
    AzGraphicsView(QGraphicsScene *, QWidget * = 0);
    ~AzGraphicsView();
    void setShowArrow(bool val)                 { mShowArrow = val;}
    bool showArrow() const              { return mShowArrow; }
#ifdef DEBUG_EDITOR
signals:
    void mouseMove(QMouseEvent * event);
#endif
protected slots:
    void selectionChanged();
    void moveItemEvent(const QPointF&, const QPointF&);
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void drawForeground(QPainter * painter, const QRectF & rect);
private:
    void setScene(QGraphicsScene *);
    AzGraphicsSelectedItemArrow mArrows;
    bool mShowArrow;

    AzGraphicsSelectedItemArrow::Side mActiveTransformArrow;

};

#endif // AZGRAPHICSVIEW_H
