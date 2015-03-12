QT          += widgets designer

CONFIG      += plugin
TEMPLATE    = lib

TARGET = $$qtLibraryTarget($$TARGET)

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

CONFIG += release



HEADERS     = AzZoomSlider.h \
              AzZoomSliderPlugin.h
              
SOURCES     = AzZoomSlider.cpp \
              AzZoomSliderPlugin.cpp
              
unix {
        OBJECTS_DIR = build/.tmp
        MOC_DIR = build/.tmp
    } else {
        OBJECTS_DIR = build/.tmp
        MOC_DIR = build/.tmp
}

              
OTHER_FILES += AzZoomSlider.json
