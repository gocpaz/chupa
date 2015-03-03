#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

#include "AzGraphicsItem.h"
#include "AzGraphicsView.h"

namespace Ui {
    class MainWindow;
}

#ifdef DEBUG_EDITOR
#include "DebugSchemaDesign.h"
#endif

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
    void setBtnEnabled();
    void selectionChanged();
    void rotateLeft90();
    void rotateRight90();
#ifdef DEBUG_EDITOR
    void mouseMoveView(QMouseEvent * event);

#endif
private:
    inline bool hasSelected() const                { return mCurrentView->scene()->selectedItems().size() > 0; }
    inline QGraphicsItem* selectedItem() const     { return hasSelected() ?  mCurrentView->scene()->selectedItems()[0] : 0;}

    Ui::MainWindow *ui;
    AzGraphicsView *mCurrentView;
    int mDebugRotate;
#ifdef DEBUG_EDITOR
    void showItemCoord();
    DebugSchemaDesign *mDebugWin;
#endif
};

#endif // MAINWINDOW_H
