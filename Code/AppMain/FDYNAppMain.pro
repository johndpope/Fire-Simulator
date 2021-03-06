# Builds the main executable

include(../QMake/common.pri)

TARGET = FRI3D
TEMPLATE = app
QT += widgets core gui network xml
CONFIG += precompile_header
PRECOMPILED_HEADER = PCH_AppMain.h


# Common Configurations #

INCLUDEPATH +=\
        ./ \
        $$NDYNPATH/include \
        $$NDYNPATH/include/Core \
        $$NDYNPATH/include/Geometry \
        $$NDYNPATH/include/Render \
        $$NDYNPATH/include/GLI \
        $$ROOTPATH/Code/FDYNUI

QMAKE_LIBDIR += $$NDYNPATH/lib

LIBS += -lFDYNUI -lNERender -lNEGLI -lNEGeometry -lNECore

if($$RELEASE) {
    DESTDIR = $${OUT_PWD}/../../Build/Release
    LIBS += -L$$ROOTPATH/Build/Release
    LIBS += -L$$NDYNPATH/lib
} else {
    DESTDIR = $${OUT_PWD}/../../Build/Debug
    LIBS += -L$$ROOTPATH/Build/Debug
    LIBS += -L$$NDYNPATH/lib
}


# Platform Specific Configurations #

win32 {
    INCLUDEPATH  += $$NDYNPATH/External/windows/include
    RC_FILE = $$ROOTPATH/Code/FDYNUI/Resources/icons/FDYN.rc
}

unix:!macx {
    INCLUDEPATH  += $$NDYNPATH/External/Linux/include
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -L$$ROOTPATH/External/Linux/lib -lGL
    QMAKE_LFLAGS += '-Wl,-rpath,\'$$ORIGIN/Libs\''
}

macx {
    INCLUDEPATH +=  $$NDYNPATH/External/macosx/include/
    LIBS += -L$$ROOTPATH/External/macosx/lib
    LIBS += -L$$NDYNPATH/External/macosx/lib
    QMAKE_LFLAGS += '-Wl,-rpath,\'$$ORIGIN/Libs\''
    ICON = $$ROOTPATH/Code/FDYNUI/Resources/icons/Simple.icns
}



# Sources #

SOURCES += AppMain.cpp
HEADERS += \
    PCH_AppMain.h


