#ifndef DEBUGSCHEMADESIGN_H
#define DEBUGSCHEMADESIGN_H
#include "ui_DebugSchemaDesign.h"

#include <QWidget>

namespace Ui {
    class DebugSchemaDesign;
}


class DebugSchemaDesign : public QWidget {
public:
    explicit DebugSchemaDesign(QWidget *parent = 0);
    ~DebugSchemaDesign();
    Ui::DebugSchemaDesign *ui;
};

#endif // DEBUGSCHEMADESIGN_H
