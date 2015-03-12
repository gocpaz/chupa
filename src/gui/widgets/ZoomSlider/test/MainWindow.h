#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>

#include "AzZoomSlider.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void zoomChange(qreal);
private:
    Ui::MainWindow *ui;
    AzZoomSlider *mZoomSlider;
};

#endif // MAINWINDOW_H
