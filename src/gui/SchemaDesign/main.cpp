#include <QApplication>
#include <QTextCodec>
#include "MainWindow.h"


int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   Q_INIT_RESOURCE(res);
   QTextCodec *codec = QTextCodec::codecForName("UTF-8");
   QTextCodec::setCodecForLocale(codec);

   MainWindow w;
   //w.setWindowState(Qt::WindowMaximized);
   w.show();

   return a.exec();


   return 0;
}
