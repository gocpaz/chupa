#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
    class MainWindow;
}

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
};

#endif // MAINWINDOW_H
