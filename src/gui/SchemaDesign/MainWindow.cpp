#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QDebug>
#include <QGraphicsSvgItem>
#include <QtMath>
#include <QPushButton>

#include "AzGraphicsScene.h"
#include "AzGraphicsSvgItem.h"
#include "AzGraphicsPoligonItem.h"
#include "AzGraphicsView.h"
#include "AzGraphicsItem.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
#ifdef DEBUG_EDITOR
     mDebugWin = new DebugSchemaDesign(ui->centralwidget);
     ui->verticalLayout->addWidget(mDebugWin);
#endif
     mDebugRotate = 0;
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabClosed(int)));
    addElementEditTab();
#ifdef DEBUG_EDITOR
    connect(mCurrentView,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveView(QMouseEvent*)));
#endif


}

#ifdef DEBUG_EDITOR
/*!
 * \brief MainWindow::mouseMoveView
 * Получаем с view mouseMoveEvent
 */
void MainWindow::mouseMoveView(QMouseEvent * event) {
    //QPointF pos = mView->mapToScene(event->pos());
    QPoint pos = event->pos();
    mDebugWin->ui->lbViewPortCoord->setText(QString::number(pos.x())+'x'+QString::number(pos.y()));
    QPointF scenePos = mCurrentView->mapToScene(pos);
    //scenePos = setX();
    mDebugWin->ui->lbSceneCoord->setText(QString::number(scenePos.x())+'x'+QString::number(scenePos.y()));
    showItemCoord();
}

void MainWindow::showItemCoord() {
    QGraphicsItem *item = (mCurrentView->scene()->selectedItems().size() > 0) ? mCurrentView->scene()->selectedItems()[0] : 0;
    if (item) {
        QRect rect = mCurrentView->mapFromScene(item->sceneBoundingRect()).boundingRect();
        QPointF scenePos = item->sceneBoundingRect().center();
        //QPointF scenePos = item->pos();

        mDebugWin->ui->lbScenePos->setText(QString::number(scenePos.x())+'x'+QString::number(scenePos.y()));
        mDebugWin->ui->lbViewPos->setText(QString::number(rect.x())+'x'+QString::number(rect.y()));
        mDebugWin->ui->lbViewSize->setText(QString::number(rect.width())+'x'+QString::number(rect.height()));
    } else {
        mDebugWin->ui->lbScenePos->setText("");
    }
}

#endif

void MainWindow::selectionChanged() {
#ifdef DEBUG_EDITOR
    showItemCoord();
#endif
    setBtnEnabled();
}


void MainWindow::addElementEditTab() {

QWidget *w = new QWidget(this);

#ifdef DEBUG_USE_NATIVE
    QGraphicsScene *scene = new QGraphicsScene(this);

    QGraphicsView *view2 = new QGraphicsView(scene,w);
    QGraphicsView *view = new QGraphicsView(scene,w);

    QGraphicsSvgItem *svg = new QGraphicsSvgItem("comp.svg");
#else
   QGraphicsScene *scene = new AzGraphicsScene(this);

   AzGraphicsView *view2 = new AzGraphicsView(scene,w);
   AzGraphicsView *view = new AzGraphicsView(scene,w);

   AzGraphicsSvgItem *svg = new AzGraphicsSvgItem("comp.svg");

#endif

   //view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

   mCurrentView = view;

   QTransform trans;
   trans.rotate(45);
   //svg->setTransform(trans);

   scene->setSceneRect(-0,-0,5000,5000);

    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    rectItem->setRect(0,0,100,100);
    rectItem->setPos(50,150);
    rectItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
    rectItem->setFlag(QGraphicsItem::ItemIsMovable,true);
    scene->addItem(rectItem);

   svg->setFlag(QGraphicsItem::ItemIsSelectable,true);
   svg->setFlag(QGraphicsItem::ItemIsMovable,true);
   svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);
   scene->addItem(svg);

   AzGraphicsSvgItem *svgClone = svg->clone();
   svgClone->setPos(50,150);
   scene->addItem(svgClone);



   QHBoxLayout *l = new QHBoxLayout(w);



   l->addWidget(view);
   l->addWidget(view2);

   connect(ui->zoomSpider,SIGNAL(sliderMoved(int)),this,SLOT(zoomView(int)));   
   connect(mCurrentView->scene(),SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
   connect(ui->actionRotateLeft,SIGNAL(triggered()),this,SLOT(rotateLeft90()));
   connect(ui->actionRotateRight,SIGNAL(triggered()),this,SLOT(rotateRight90()));

   selectionChanged();


   ui->tabWidget->addTab(w,"editTab");

    svgClone->setSelected(true);
    view->setShowArrow(true);
    //view2->setShowArrow(true);


}


MainWindow::~MainWindow() {
    delete ui;
}


/*!
 * \brief MainWindow::tabClosed
 * \param tabIndex
 * Закрываем таб и удаляем ресурсы - сцену и вьюху.
 */
void MainWindow::tabClosed(int tabIndex) {
    QGraphicsView *view = dynamic_cast<QGraphicsView*>(ui->tabWidget->widget(tabIndex));
    if (view) {
        delete view->scene();
        delete view;
    }
    ui->tabWidget->removeTab(tabIndex);
}

void MainWindow::zoomView(int) {
    QAbstractSlider *slider = dynamic_cast<QAbstractSlider*>(sender());
    if (slider) {
        qreal scale = qPow(2.0,(slider->value()-slider->maximum()/2)/100.0);
        QMatrix matrix;
        matrix.scale(scale,scale);
        mCurrentView->setMatrix(matrix);
    }
}

void MainWindow::setBtnEnabled() {
    bool selected = mCurrentView->scene()->selectedItems().size() > 0;
    ui->actionRotateLeft->setEnabled(selected);
    ui->actionRotateRight->setEnabled(selected);
}

void MainWindow::rotateLeft90() {
    AzGraphicsSvgItem *item = dynamic_cast<AzGraphicsSvgItem *>(selectedItem());
   if (item) {
       QTransform trans;
       trans.rotate(-90);
       item->setTransform(trans);
       item->fixTransofm();

   }
}

void MainWindow::rotateRight90() {
    AzGraphicsSvgItem *item = dynamic_cast<AzGraphicsSvgItem *>(selectedItem());
    if (item) {
        const QRectF sceneRect = item->boundingRect();
        const QPointF center = sceneRect.center();
//        item->setTransformOriginPoint(item->boundingRect().center());
//        item->setRotation(item->rotation() + 1);

        mDebugRotate+=10;
        QTransform t1,t2,t3;
        t1.translate(-center.x(),-center.y());
        t2.rotate(mDebugRotate);
        t3.translate(center.x(),center.y());

//        item->setTransformOriginPoint(item->boundingRect().center());
//        trans.rotate(90);
        item->setTransform(t1*t2*t3);

    }
}
