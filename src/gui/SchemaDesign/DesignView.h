#ifndef DESIGNVIEW_H
#define DESIGNVIEW_H

#include <QWidget>

#include "AzGraphicsView.h"
#include "AzGraphicsScene.h"

class DesignView : public QWidget
{
    Q_OBJECT
public:

    explicit DesignView(QWidget *parent = 0);
    ~DesignView();

    void addItem(QGraphicsItem * item)                  { mScene->addItem(item); }

    inline const AzGraphicsView* view() const           { return mView; }
    inline const AzGraphicsScene *scene() const         { return mScene; }
    inline qreal scale() const                          { return mView->transform().m11(); }

signals:
    void selectionChanged();
    void scaleChanged(qreal);
#ifdef DEBUG_EDITOR
    void mouseMoveView(QMouseEvent *);
#endif
public slots:
    void setScale(qreal scale);
private:
    AzGraphicsView      *mView;
    AzGraphicsScene     *mScene;
};

#endif // DESIGNVIEW_H
