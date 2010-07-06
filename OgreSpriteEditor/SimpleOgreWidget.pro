# -------------------------------------------------
# Project created by QtCreator 2010-05-03T12:44:54
# -------------------------------------------------
TARGET = SimpleOgreWidget
TEMPLATE = app

# # You may need to change this for Ogre installation default directory
# INCLUDEPATH += /usr/include/OGRE
# CONFIG += link_pkgconfig
# PKGCONFIG += OGRE
# CONFIG += thread warn_off
# /usr/lib/libboost_thread.so \
# /usr/lib/libboost_date_time.so
QT += core \
    gui \
    xml
CONFIG += thread \
    warn_off \
    link_pkgconfig
PKGCONFIG += OGRE
INCLUDEPATH += /usr/local/include/OGRE \
    /usr/include/boost \
    /usr/include/freetype2 \
    code/FastDelegate \
    code/Canvas
LIBS += -Wl,-rpath, \
    /usr/local/lib/libOgreMain.so
SOURCES += code/DomModel.cpp \
    code/DomItem.cpp \
    code/Canvas/Atlas.cpp \
    code/Canvas/Canvas.cpp \
    code/Canvas/GUIBox.cpp \
    code/Canvas/GUICanvas.cpp \
    code/Canvas/GUILabel.cpp \
    code/Canvas/GUITextList.cpp \
    code/Parser2DS.cpp \
    code/MainWindow.cpp \
    code/OgreTestWidget.cpp \
    code/OgreWidget.cpp \
    code/Application.cpp \
    code/main.cpp
HEADERS += code/DomItem.h \
    code/DomModel.h \
    code/Canvas/Atlas.h \
    code/Canvas/Canvas.h \
    code/Canvas/GUIBox.h \
    code/Canvas/GUICanvas.h \
    code/Canvas/GUILabel.h \
    code/Canvas/GUITextList.h \
    code/FastDelegate/FastDelegate.h \
    code/FastDelegate/FastDelegateBind.h \
    code/Animacion2d.h \
    code/Elementos2d.h \
    code/Parser2DS.h \
    code/MainWindow.h \
    code/OgreTestWidget.h \
    code/OgreWidget.h \
    code/Application.h
FORMS = qt/MainWindow.ui
