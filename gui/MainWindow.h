#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
    class MainWindow;
}

//#define DEBUG_USE_NATIVE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void addElementEditTab();
    void zoomView(int);
private slots:
    void tabClosed(int);
    void leftBtnClicked();
private:
    QGraphicsView *currentView() const;
    Ui::MainWindow *ui;
    QGraphicsView *mView;
};

#endif // MAINWINDOW_H
