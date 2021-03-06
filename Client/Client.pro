
# DO NOT USE QMAKE TO AUTOGEN THESE, IT DOESN'T WORK
QT += core \
    gui \
    xml \
    phonon
OBJECTS_DIR = obj
TEMPLATE = app
TARGET = Client
HEADERS += ../Core/comm/globals.h \
    Animation/Animation.h \
    Animation/Image.h \
    Animation/qtxmlparse.h \
    Animation/QtXmlParse_global.h \
    Comm/Commclient.h \
    Comm/Tcpclient.h \
    engine/BaseWindow.h \
    engine/connectthread.h \
    engine/GameObject.h \
    engine/ipbox.h \
    engine/main.h \
    engine/Map.h \
    engine/MapTile.h \
    engine/Panel.h \
    engine/Roundrectitem.h \
    engine/Splashitem.h \
    Renderer/Renderer.h
SOURCES += Animation/Animation.cpp \
    Animation/Image.cpp \
    Animation/qtxmlparse.cpp \
    Comm/Commclient.cpp \
    Comm/Tcpclient.cpp \
    engine/BaseWindow.cpp \
    engine/connectthread.cpp \
    engine/ipbox.cpp \
    engine/main.cpp \
    engine/Map.cpp \
    engine/MapTile.cpp \
    engine/Panel.cpp \
    engine/Roundrectitem.cpp \
    engine/Splashitem.cpp \
    Renderer/Renderer.cpp
RESOURCES += Resources/textures.qrc \
    menuitems.qrc
FORMS += backside.ui \
    ipbox.ui
contains(QT_CONFIG, opengl):QT += opengl
LIBS += -lCore \
    -L../Core/Debug \
    -lGL \
    -lGLU \
    -lglut
CONFIG += qt \
    debug \
    warn_on \
    resources
OTHER_FILES += ../Core/resources/tiles.xml
