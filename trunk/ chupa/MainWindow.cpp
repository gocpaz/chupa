#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QGraphicsSvgItem>
#include <QtMath>

#include "ElementScene.h"
#include "AzGraphicsSvgItem.h"
#include "AzGrpahicsTransformArrowItem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabClosed(int)));
    addElementEditTab();
}

void MainWindow::addElementEditTab() {
   QGraphicsScene *scene = new ElementScene(this);

   scene->setSceneRect(-10,-10,5000,5000);
   QGraphicsView *view = new QGraphicsView(scene,this);

   QGraphicsSvgItem *svg = new AzGraphicsSvgItem("comp.svg");
   QGraphicsSvgItem *svg2 = new AzGraphicsSvgItem("comp.svg");
   svg2->setPos(100,100);


   svg->setFlag(QGraphicsItem::ItemIsSelectable,true);
   svg->setFlag(QGraphicsItem::ItemIsMovable,true);
   svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);

   svg2->setFlag(QGraphicsItem::ItemIsSelectable,true);
   svg2->setFlag(QGraphicsItem::ItemIsMovable,true);
   svg2->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);

   QGraphicsItemGroup *gr = new QGraphicsItemGroup;
   gr->addToGroup(svg);
   gr->addToGroup(svg2);

   gr->setFlag(QGraphicsItem::ItemIsSelectable,true);
   gr->setFlag(QGraphicsItem::ItemIsMovable,true);
   gr->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);

   //svg->setSelected(true);
   //svg->setAcceptDrops(true);
   scene->addItem(svg);
   scene->addItem(svg2);



   //scene->addItem(gr);


   view->centerOn(svg);

   //view->scale(1,1);


   connect(ui->zoomSpider,SIGNAL(sliderMoved(int)),this,SLOT(zoomView(int)));

   ui->tabWidget->addTab(view,"editTab");
}


MainWindow::~MainWindow() {
    delete ui;
}

QGraphicsView* MainWindow::currentView() const {
   QGraphicsView* view = dynamic_cast<QGraphicsView*>(ui->tabWidget->currentWidget());
    return view;
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
