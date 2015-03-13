#include "DebugSchemaDesign.h"


DebugSchemaDesign::DebugSchemaDesign(QWidget *parent) : QWidget(parent),ui(new Ui::DebugSchemaDesign) {
      ui->setupUi(this);
}

DebugSchemaDesign::~DebugSchemaDesign() {
delete ui;
}

void DebugSchemaDesign::addMessage(const QString& message) {
    ui->plainTextEdit->appendPlainText(message);
}
