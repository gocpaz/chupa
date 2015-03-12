#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   mZoomSlider  = ui->zoomSlider;
   connect(mZoomSlider,SIGNAL(valueChanged(qreal)),this,SLOT(zoomChange(qreal)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::zoomChange(qreal scale) {
    ui->lbSlider->setText(QString::number(scale));
}
