#ifndef AZGRAPHICSVIEW_H
#define AZGRAPHICSVIEW_H

#include <QMouseEvent>
#include <QGraphicsView>

#include "AzTransformArrows.h"


class AzGraphicsView : public QGraphicsView
{
Q_OBJECT
public:
    AzGraphicsView(QGraphicsScene *, QWidget * = 0);
    ~AzGraphicsView();
    void setShowSelectedArrow(bool val);
#ifdef DEBUG_EDITOR
signals:
    void mouseMove(QMouseEvent * event);
#endif
protected slots:
    void selectionChanged();
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void drawForeground(QPainter * painter, const QRectF & rect);
private:
    void setScene(QGraphicsScene *);
    AzTransformArrow mArrows;
    bool mShowArrow;  // !!!убрать, нет смысла, заменить на динамику, проверять на валидность объекта
};

#endif // AZGRAPHICSVIEW_H
