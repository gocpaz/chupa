#include "DesignView.h"

#include <QDebug>
#include <QLayout>



DesignView::DesignView(QWidget *parent) : QWidget(parent) {
    mScene = new AzGraphicsScene;
    mScene->setSceneRect(-20,-20,5000,2000);
    mView = new AzGraphicsView(mScene);
    mView->centerOn(0,0);
    mView->setShowSelectedArrow(true);
    QHBoxLayout *l = new QHBoxLayout(this);
   //
    l->addWidget(mView);
    connect(mScene,SIGNAL(selectionChanged()),this,SIGNAL(selectionChanged()));
#ifdef DEBUG_EDITOR
    connect(mView,SIGNAL(mouseMove(QMouseEvent*)),this,SIGNAL(mouseMoveView(QMouseEvent *)));
#endif
}

DesignView::~DesignView() {
    delete mView;
    delete mScene;
}

/*!
 * \brief DesignView::setScale
 * \param scale
 * Масштабирует view
 */
void DesignView::setScale(qreal scale) {
    mView->setTransform(QTransform::fromScale(scale,scale));
    emit scaleChanged(scale);
}

