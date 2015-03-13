QT       += widgets svg

INCLUDEPATH += ../widgets/ZoomSlider

TEMPLATE = app

DESTDIR = ../../../../bin

INCLUDEPATH += ../../

HEADERS += 	MainWindow.h \
	    AzGraphicsView.h \
	    AzGraphicsScene.h \
	    DebugSchemaDesign.h \
	    ../../AzChupa.h \
    DesignView.h \
    ../widgets/ZoomSlider/AzZoomSlider.h \
    AzTransformArrows.h \
    AzSelectedItemPolygonArrows.h \
    AzGraphicsItem.h

CONFIG+=debug_and_release


SOURCES += main.cpp \
	   MainWindow.cpp \
	    AzGraphicsView.cpp \
	    AzGraphicsScene.cpp \
	    DebugSchemaDesign.cpp \
    DesignView.cpp \
    ../widgets/ZoomSlider/AzZoomSlider.cpp \
    AzTransformArrows.cpp \
    AzSelectedItemPolygonArrows.cpp \
    AzGraphicsItem.cpp


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
