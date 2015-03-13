#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QDebug>
#include <QGraphicsSvgItem>
#include <QtMath>
#include <QPushButton>
#include <math.h>

#include "AzGraphicsScene.h"
#include "AzGraphicsPoligonItem.h"
#include "AzGraphicsView.h"
#include "AzGraphicsItem.h"
#include "DesignView.h"


#ifdef DEBUG_EDITOR
    static DebugSchemaDesign *debugOutput;

    void debugMessageOutput(QtMsgType, const QMessageLogContext &, const QString & msg) {
        if (!debugOutput)
            return;
        debugOutput->addMessage(msg);
    }
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mCurrentDesignView = 0;
#ifdef DEBUG_EDITOR
    mDebugWin = new DebugSchemaDesign(ui->centralwidget);
    debugOutput = mDebugWin;
    ui->verticalLayout->addWidget(mDebugWin);
    //qInstallMessageHandler(debugMessageOutput);
    setMouseTracking(true);
    qDebug() << "Start debug version";
    connect(ui->actionRotateRight,SIGNAL(triggered()),this,SLOT(rotateRight90()));
#endif

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabClosed(int))); //close tab event
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentViewChanged(int))); //change view
    newView();
}

MainWindow::~MainWindow() {
#ifdef DEBUG_EDITOR
    debugOutput = 0;
    qInstallMessageHandler(0);
#endif
    delete ui;
}

/*!
 * \brief MainWindow::tabClosed
 * \param tabIndex
 * Закрываем таб и удаляем ресурсы
 */
void MainWindow::tabClosed(int tabIndex) {
  QWidget *widget = ui->tabWidget->widget(tabIndex);
  delete widget;
}


/*!
 * \brief MainWindow::newView
 * \param name
 * Add new empty view
 */
void MainWindow::newView(const QString& name) {
    DesignView *designView = new DesignView;
    ui->tabWidget->addTab(designView,name);


    AzGraphicsSvgItem *svg = new AzGraphicsSvgItem("comp.svg");
    svg->setPos(50,50);
    designView->addItem(svg);

    AzGraphicsSvgItem *svg2 = new AzGraphicsSvgItem("comp.svg");
    designView->addItem(svg2);

}

#ifdef DEBUG_EDITOR
/*!
 * \brief MainWindow::mouseMoveView
 * Получаем с view mouseMoveEvent
 */
//void MainWindow::mouseMoveView(QMouseEvent *) {
//    //QPointF pos = mView->mapToScene(event->pos());
//    QPoint pos = event->pos();
//    mDebugWin->ui->lbViewPortCoord->setText(QString::number(pos.x())+'x'+QString::number(pos.y()));
//    QPointF scenePos = mCurrentView->mapToScene(pos);
//    //scenePos = setX();
//    mDebugWin->ui->lbSceneCoord->setText(QString::number(scenePos.x())+'x'+QString::number(scenePos.y()));
//    showItemCoord();
//}

void MainWindow::showItemCoord() {
//    QGraphicsItem *item = (mCurrentDesignView->scene()->selectedItems().size() > 0) ? mCurrentDesignView->scene()->selectedItems()[0] : 0;
//    if (item) {
//        QRect rect = mCurrentDesignView->view()->mapFromScene(item->sceneBoundingRect()).boundingRect();
//        QRectF sceneRect = item->sceneBoundingRect();
//        mDebugWin->ui->sceneRect->setText(QString::number(sceneRect.x())+','+QString::number(sceneRect.y())+' '+QString::number(sceneRect.width())+'x'+QString::number(sceneRect.height()));

//        mDebugWin->ui->viewRect->setText(QString::number(rect.x())+'x'+QString::number(rect.y()));
//        //mDebugWin->ui->lbViewSize->setText(QString::number(rect.width())+'x'+QString::number(rect.height()));
//    } else {
//        mDebugWin->ui->sceneRect->setText("");
//        mDebugWin->ui->viewRect->setText("");
//    }
}

void MainWindow::mouseMoveEventView(QMouseEvent *) {
   showItemCoord();
}

#endif

void MainWindow::selectionChanged() {
#ifdef DEBUG_EDITOR
    showItemCoord();
#endif
    setBtnEnabled();
}



void MainWindow::setBtnEnabled() {
//    bool selected = mCurrentView->scene()->selectedItems().size() > 0;
//    ui->actionRotateLeft->setEnabled(selected);
//    ui->actionRotateRight->setEnabled(selected);
}

void MainWindow::rotateLeft90() {
//    AzGraphicsSvgItem *item = dynamic_cast<AzGraphicsSvgItem *>(selectedItem());
//   if (item) {
//       QTransform trans;
//       trans.rotate(-90);
//       item->setTransform(trans);
//       item->fixTransofm();

//   }
}

void MainWindow::rotateRight90() {
//    QGraphicsItem *qitem = 0;
//    if (mCurrentDesignView->scene()->selectedItems().size() > 0)
//        qitem = mCurrentDesignView->scene()->selectedItems()[0];
//    AzGraphicsSvgItem *item = dynamic_cast<AzGraphicsSvgItem *>(qitem);
//    if (item) {
//        QTransform trans;
//        trans.shear(-2,0);
//        item->setTransform(trans);
//    }
    //QGraphicsItemGroup *group = mCurrentDesignView->scene()->createItemGroup(mCurrentDesignView->scene()->selectedItems());

//   AzGraphicsItemGroup *group =  mCurrentDesignView->createItemGroup(scene()->selectedItems());
//   group->setFlag(QGraphicsItem::ItemIsSelectable,true);
//   group->setFlag(QGraphicsItem::ItemIsMovable,true);

}

/*!
 * \brief MainWindow::viewChanged
 * Вызывается при смене активного вью, где tab - активный таб. Если -1 - нет выделеных табов
 */
void MainWindow::currentViewChanged(int tab) {
    if (mCurrentDesignView) {
        disconnect(mCurrentDesignView,0,this,0);
        disconnect(ui->zoomSlider,0,mCurrentDesignView,0);
    }
    //ui->tabWidget->widget(tab) - return 0 if tab out of range
    mCurrentDesignView = dynamic_cast<DesignView*>(ui->tabWidget->widget(tab));
    if (mCurrentDesignView) {
        connect(mCurrentDesignView,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
        connect(ui->zoomSlider,SIGNAL(scaleChanged(qreal)),mCurrentDesignView,SLOT(setScale(qreal)));
#ifdef DEBUG_EDITOR
        connect(mCurrentDesignView,SIGNAL(mouseMoveView(QMouseEvent*)),this,SLOT(mouseMoveEventView(QMouseEvent*)));
#endif
    }
}
