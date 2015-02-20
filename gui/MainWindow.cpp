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
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabClosed(int)));
    //connect(ui->pbLeft,SIGNAL(clicked()),this,SLOT(leftBtnClicked()));
    addElementEditTab();
#ifdef DEBUG_EDITOR
    connect(mView,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveView(QMouseEvent*)));
    connect(mView->scene(),SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));

    selectionChanged();

#endif

}

#ifdef DEBUG_EDITOR
void MainWindow::mouseMoveView(QMouseEvent * event) {
    //QPointF pos = mView->mapToScene(event->pos());
    QPoint pos = event->pos();
    mDebugWin->ui->lbMouseCoord->setText(QString::number(pos.x())+'x'+QString::number(pos.y()));
    showItemCoord();
}

void MainWindow::showItemCoord() {
    QGraphicsItem *item = (mView->scene()->selectedItems().size() > 0) ? mView->scene()->selectedItems()[0] : 0;
    if (item) {
        //QRectF rect = item->sceneBoundingRect();
        QRect rect = mView->mapFromScene(item->sceneBoundingRect()).boundingRect();
        rect.setX(item->pos().x());
        rect.setY(item->pos().y());
        const QString coord(QString::number(rect.x()) +'x' + QString::number(rect.y()));
        const QString rectStr(QString::number(rect.width()) +'x' + QString::number(rect.height()));
        mDebugWin->ui->lbSelectedRect->setText(coord+':' + rectStr);
    } else {
        mDebugWin->ui->lbSelectedRect->setText("");
    }
}

void MainWindow::selectionChanged() {
    showItemCoord();
}
#endif

void MainWindow::addElementEditTab() {

QWidget *w = new QWidget(this);

#ifdef DEBUG_USE_NATIVE
    QGraphicsScene *scene = new QGraphicsScene(this);

    QGraphicsView *view2 = new QGraphicsView(scene,w);
    QGraphicsView *view = new QGraphicsView(scene,w);

    QGraphicsSvgItem *svg = new QGraphicsSvgItem("comp2.svg");
    QGraphicsSvgItem *svg2 = new QGraphicsSvgItem("comp.svg");
#else
   QGraphicsScene *scene = new AzGraphicsScene(this);

   AzGraphicsView *view2 = new AzGraphicsView(scene,w);
   AzGraphicsView *view = new AzGraphicsView(scene,w);

   QGraphicsSvgItem *svg = new AzGraphicsSvgItem("comp2.svg");
   QGraphicsSvgItem *svg2 = new AzGraphicsSvgItem("comp.svg");

#endif
   scene->setSceneRect(-10,-10,5000,5000);
  //view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    AzGraphicsItem *azItem = new AzGraphicsItem;
    azItem->setPos(0,0);
    //scene->addItem(azItem);

    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    rectItem->setRect(0,0,100,100);
    rectItem->setPos(0,150);
    rectItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
    rectItem->setFlag(QGraphicsItem::ItemIsMovable,true);
    scene->addItem(rectItem);

   AzGraphicsPoligonItem *arr = new AzGraphicsPoligonItem;
   arr->setPos(200,250);
   arr->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
   scene->addItem(arr);


   //svg->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
   svg->setFlag(QGraphicsItem::ItemIsSelectable,true);
   svg->setFlag(QGraphicsItem::ItemIsMovable,true);
   svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);


   //svg2->setFlag(QGraphicsItem::ItemSendsGeometryChanges,false);
   svg2->setFlag(QGraphicsItem::ItemIsSelectable,true);
   svg2->setFlag(QGraphicsItem::ItemIsMovable,true);
   svg2->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);
   //svg2->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

    svg2->setPos(0,50);

   QGraphicsItemGroup *gr = new QGraphicsItemGroup;
   gr->addToGroup(svg);
   gr->addToGroup(svg2);

   gr->setFlag(QGraphicsItem::ItemIsSelectable,true);
   gr->setFlag(QGraphicsItem::ItemIsMovable,true);
   gr->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);

   //svg->setSelected(true);
   //svg->setAcceptDrops(true);
   //scene->addItem(svg);
   scene->addItem(svg2);



   //scene->addItem(gr);

   QHBoxLayout *l = new QHBoxLayout(w);

   mView = view;

   l->addWidget(view);
   l->addWidget(view2);

    // view->centerOn(svg);
   //  view2->centerOn(svg);


   connect(ui->zoomSpider,SIGNAL(sliderMoved(int)),this,SLOT(zoomView(int)));

   ui->tabWidget->addTab(w,"editTab");
}


MainWindow::~MainWindow() {
    delete ui;
}

QGraphicsView* MainWindow::currentView() const {
   //QGraphicsView* view = dynamic_cast<QGraphicsView*>(ui->tabWidget->currentWidget());
   // return view;
   return mView;
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
        currentView()->setMatrix(matrix);
    }
}

void MainWindow::leftBtnClicked() {
    if (!currentView())
        return;
    if (!currentView()->scene())
        return;
    if (currentView()->scene()->selectedItems().size() == 0)
        return;
    QGraphicsItem *item = mView->scene()->selectedItems()[0];
    QTransform trans;
    trans.scale(1,-1);
    //qDebug() << item->sceneBoundingRect();
    item->setTransform(trans);
    //qDebug() << item->sceneBoundingRect();
   // qDebug() << item->sceneBoundingRect();

//    QGraphicsItem *item = currentView()->scene()->selectedItems()[0];
//    item->setPos(item->pos().x()+2,item->pos().y());
}
