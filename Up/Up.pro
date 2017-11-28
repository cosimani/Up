QT       += core gui opengl multimedia sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkit webkitwidgets

TARGET = FotoYa
TEMPLATE = app

win32:LIBS += -lopengl32
win32:LIBS += -lglu32
unix:LIBS += -lGLU

#DEFINES += DEBUG_O1

SOURCES += main.cpp \
    adminDB.cpp \
    publicador.cpp \
    confevento.cpp \
    galeria.cpp \
    impresor.cpp \
    proyectador.cpp \
    scene.cpp \
    search.cpp \
    instagram.cpp \
    miqlineedit.cpp \
    boton.cpp \
    principal.cpp \
    image.cpp \
    comun.cpp \
    webview.cpp \
    app.cpp \
    navegador.cpp \
    manager.cpp

HEADERS  += \
    adminDB.h \
    publicador.h \
    confevento.h \
    galeria.h \
    impresor.h \
    proyectador.h \
    scene.h \
    search.h \
    texture.h \
    video.h \
    instagram.h \
    miqlineedit.h \
    boton.h \
    principal.h \
    image.hpp \
    comun.h \
    webview.h \
    app.h \
    navegador.h \
    manager.h

FORMS += \
    publicador.ui \
    confevento.ui \
    galeria.ui \
    impresor.ui \
    proyectador.ui \
    search.ui \
    instagram.ui \
    boton.ui \
    principal.ui

RESOURCES += \
    recursos.qrc
