FORMS += \
    HistoryWindow.ui \
    mainwindow.ui \
    plotwindow.ui \
    historywindow.ui

HEADERS += \
    evaporation.h \
    mainwindow.h \
    plotwindow.h \
    qcustomplot.h \
    historywindow.h

SOURCES += \
    evaporation.cpp \
    main.cpp \
    mainwindow.cpp \
    plotwindow.cpp \
    qcustomplot.cpp \
    historywindow.cpp

QT += widgets
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
