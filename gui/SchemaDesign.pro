QT       += widgets svg


TEMPLATE = app

#DESTDIR = ../bin

HEADERS += MainWindow.h \
    AzGraphicsSvgItem.h \
    ElementScene.h \
    AzGraphicsSelectedItemArrow.h \
    AzGraphicsPoligonItem.h

CONFIG+=debug_and_release


SOURCES += main.cpp \
	   MainWindow.cpp \
    ElementScene.cpp \
    AzGraphicsSvgItem.cpp \
    AzGraphicsSelectedItemArrow.cpp \
    AzGraphicsPoligonItem.cpp


FORMS    += MainWindow.ui

#RESOURCES =  res.qrc



CONFIG(debug,debug|release){
    DEFINES =  AZ_DEBUG
    unix {
	OBJECTS_DIR = build/.tmp/debug
	MOC_DIR = build/.tmp/debug
    } else {
	OBJECTS_DIR = build/.tmp/debug
	MOC_DIR = build/.tmp/debug
    }
    TARGET = SchemaDesignd
}
else {
    unix {
	OBJECTS_DIR = build/.tmp/release
	MOC_DIR = build/.tmp/release
    } else {
	OBJECTS_DIR = build/.tmp/release
	MOC_DIR = build/.tmp/release
    }
    TARGET = SchemaDesign
}


UI_DIR = build/.tmp/
RCC_DIR = build/.tmp/
