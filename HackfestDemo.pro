#-------------------------------------------------
#
# Project created by QtCreator 2010-10-09T12:32:52
#
#-------------------------------------------------
TARGET = HackfestDemo
TEMPLATE = app

DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
    Entity.cpp \
    Utils.cpp \
    critter.cpp \
    tail.cpp \
    synth.c \
    particle.cpp \
    SynthLoaderThread.cpp
SOURCES += mainwindow.cpp
SOURCES += DemoGLWidget.cpp

HEADERS += Entity.h \
    DemoGLWidget.h \
    config.h \
    Utils.h \
    tail.h \
    critter.h \
    synth.h \
    song.h \
    particle.h \
    SynthLoaderThread.h
HEADERS += mainwindow.h

QT += opengl
LIBS += -lSDL_mixer

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl_es2
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS hellogl_es2.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl_es2
#INSTALLS += target sources
