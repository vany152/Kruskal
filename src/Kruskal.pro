QT       += core gui svg svgwidgets webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/htmlviewer.cpp \
    common/settingswindow.cpp \
    demo/GraphDemo.cpp \
    demo/cyclesdemo.cpp \
    demo/demosystem.cpp \
    demo/graph/Edge.cpp \
    demo/graph/Graph.cpp \
    demo/graph/Vertex.cpp \
    demo/kruskaldemo.cpp \
    main.cpp \
    mainwindow.cpp \
    testing/questions/question.cpp \
    testing/questions/questionwidget.cpp \
    testing/stat/StatDatabaseViewOption.cpp \
    testing/stat/sessionstat.cpp \
    testing/stat/sessionstatwidget.cpp \
    testing/stat/statdatabaseviewwidget.cpp \
    testing/testingsystem.cpp \
    testing/testingwidget.cpp \
    testing/welcometotesting.cpp \

HEADERS += \
    common/Error.h \
    common/constants.h \
    common/documents.h \
    common/htmlviewer.h \
    common/settingswindow.h \
    demo/GraphDemo.h \
    demo/cyclesdemo.h \
    demo/demosystem.h \
    demo/graph/Edge.h \
    demo/graph/Graph.h \
    demo/graph/Vertex.h \
    demo/kruskaldemo.h \
    mainwindow.h \
    testing/questions/question.h \
    testing/questions/questionwidget.h \
    testing/stat/StatDatabaseViewOption.h \
    testing/stat/sessionstat.h \
    testing/stat/sessionstatwidget.h \
    testing/stat/statdatabaseviewwidget.h \
    testing/testingsystem.h \
    testing/testingwidget.h \
    testing/welcometotesting.h \

FORMS += \
    common/htmlviewer.ui \
    common/settingswindow.ui \
    demo/cyclesdemo.ui \
    demo/demosystem.ui \
    demo/kruskaldemo.ui \
    mainwindow.ui \
    testing/questions/questionwidget.ui \
    testing/stat/sessionstatwidget.ui \
    testing/stat/statdatabaseviewwidget.ui \
    testing/testingsystem.ui \
    testing/testingwidget.ui \
    testing/welcometotesting.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
