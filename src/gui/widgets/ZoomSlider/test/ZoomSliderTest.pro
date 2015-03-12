QT       += widgets

INCLUDEPATH += ../

TEMPLATE = app

CONFIG+=debug_and_release

HEADERS += \  
    MainWindow.h \
    ../AzZoomSlider.h




SOURCES += main.cpp \
    MainWindow.cpp \
    ../AzZoomSlider.cpp

FORMS    += \
    MainWindow.ui




CONFIG(debug,debug|release){
    unix {
	OBJECTS_DIR = build/.tmp/debug
	MOC_DIR = build/.tmp/debug
    } else {
	OBJECTS_DIR = build/.tmp/debug
	MOC_DIR = build/.tmp/debug
    }
    TARGET = ZoomSliderTestd
}
else {
    unix {
	OBJECTS_DIR = build/.tmp/release
	MOC_DIR = build/.tmp/release
    } else {
	OBJECTS_DIR = build/.tmp/release
	MOC_DIR = build/.tmp/release
    }
    TARGET = ZoomSliderTest
}


UI_DIR = build/.tmp/
RCC_DIR = build/.tmp/
