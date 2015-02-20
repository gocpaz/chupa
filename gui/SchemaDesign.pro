QT       += widgets svg


TEMPLATE = app

DESTDIR = ../bin

HEADERS += MainWindow.h \
    AzGraphicsSvgItem.h \
    AzGraphicsSelectedItemArrow.h \
    AzGraphicsPoligonItem.h \
    AzGraphicsView.h \
    AzGraphicsScene.h \
    AzGraphicsItem.h \
    DebugSchemaDesign.h

CONFIG+=debug_and_release


SOURCES += main.cpp \
	   MainWindow.cpp \
    AzGraphicsSvgItem.cpp \
    AzGraphicsSelectedItemArrow.cpp \
    AzGraphicsPoligonItem.cpp \
    AzGraphicsView.cpp \
    AzGraphicsScene.cpp \
    AzGraphicsItem.cpp \
    DebugSchemaDesign.cpp


FORMS    += MainWindow.ui



RESOURCES =  res.qrc



CONFIG(debug,debug|release){
    DEFINES =  DEBUG_EDITOR
    unix {
	OBJECTS_DIR = build/.tmp/debug
	MOC_DIR = build/.tmp/debug
    } else {
	OBJECTS_DIR = build/.tmp/debug
	MOC_DIR = build/.tmp/debug
    }
    FORMS    += DebugSchemaDesign.ui
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
