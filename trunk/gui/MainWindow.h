#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
    class MainWindow;
}

#ifdef DEBUG_EDITOR
#include "DebugSchemaDesign.h"
#endif


class AzGraphicsView;

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
#ifdef DEBUG_EDITOR
    void mouseMoveView(QMouseEvent * event);
    void selectionChanged();
#endif
private:
    QGraphicsView *currentView() const;
    void showItemCoord();
    Ui::MainWindow *ui;
    AzGraphicsView *mView;
#ifdef DEBUG_EDITOR
    DebugSchemaDesign *mDebugWin;
#endif
};

#endif // MAINWINDOW_H
