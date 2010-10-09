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
SOURCES += main.cpp
SOURCES += glwidget.cpp
SOURCES += mainwindow.cpp
SOURCES += bubble.cpp

HEADERS += glwidget.h
HEADERS += mainwindow.h
HEADERS += bubble.h

RESOURCES += texture.qrc
QT += opengl

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl_es2
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS hellogl_es2.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl_es2
#INSTALLS += target sources
