#-------------------------------------------------
#
# Project created by QtCreator 2021-02-11T11:00:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = teste_IHM
TEMPLATE = app
CONFIG += console c++11
CONFIG += app_bundle
CONFIG += qt


DISTFILES += \
    fft/fftw-3.3.5-dll64/libfftw3-3.def \
    fft/fftw-3.3.5-dll64/libfftw3-3.dll \
    fft/fftw-3.3.5-dll64/libfftw3f-3.def \
    fft/fftw-3.3.5-dll64/libfftw3f-3.dll \
    fft/fftw-3.3.5-dll64/libfftw3l-3.def \
    fft/fftw-3.3.5-dll64/libfftw3l-3.dll \
    fft/fftw/fftw3.dll

DEFINES += QT_DEPRECATED_WARNINGS
win32: LIBS += -lws2_32

CONFIG += c++11

SOURCES += \
        afficheurs.cpp \
        ajoutermusique.cpp \
        boutons.cpp \
        card.cpp \
        main.cpp \
        mainwindow.cpp \
        prestartgame.cpp \
        setting.cpp \
        statistique.cpp \
        windows.cpp

HEADERS += \
        card.h \
        fft/fftw/fftw3.h \
        fft/fftw-3.3.5-dll64/fftw3.h \
        mainwindow.h \
        prestartgame.h \
        setting.h \
        structures.h

FORMS += \
        card.ui \
        mainwindow.ui \
        prestartgame.ui \
        setting.ui

unix|win32: LIBS += -L$$PWD/fft/fftw/ -lfftw3
INCLUDEPATH += $$PWD/fft/fftw
DEPENDPATH += $$PWD/fft/fftw

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
