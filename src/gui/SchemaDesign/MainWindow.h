#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

#include "DesignView.h"

namespace Ui {
    class MainWindow;
}

#ifdef DEBUG_EDITOR
#include "DebugSchemaDesign.h"
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    inline const AzGraphicsScene* scene() const                 { return mCurrentDesignView->scene(); }

public slots:
    void newView(const QString& name = "NoName");

private slots:
    void tabClosed(int);
    void currentViewChanged(int);
    void selectionChanged();


    void setBtnEnabled();

    void rotateLeft90();
    void rotateRight90();
#ifdef DEBUG_EDITOR
    void mouseMoveEventView(QMouseEvent * event);
#endif
private:
#ifdef DEBUG_EDITOR
    void showItemCoord();
    DebugSchemaDesign *mDebugWin;
#endif
    Ui::MainWindow *ui;
    DesignView *mCurrentDesignView;
};


#endif // MAINWINDOW_H
