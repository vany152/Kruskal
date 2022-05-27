QT       += core gui svg webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/documents.cpp \
    demo/cyclesdemo.cpp \
    demo/demosystem.cpp \
    demo/graph/Edge.cpp \
    demo/graph/Graph.cpp \
    demo/graph/Vertex.cpp \
    demo/kruskaldemo.cpp \
    main.cpp \
    mainwindow.cpp \
    testing/questions/Question.cpp \
    testing/questions/questionwidget.cpp \
    testing/stat/Stat.cpp \
    testing/stat/statviewwidget.cpp \
    testing/stat/statwidget.cpp \
    testing/testing.cpp \
    testing/testingsettingswindow.cpp \
    testing/testingsystem.cpp \
    testing/welcometotesting.cpp \
    theory/theorysettingswindow.cpp \
    theory/theorysystem.cpp

HEADERS += \
    common/FileError.h \
    common/constants.h \
    common/documents.h \
    demo/cyclesdemo.h \
    demo/demosystem.h \
    demo/graph/Edge.h \
    demo/graph/Graph.h \
    demo/graph/Vertex.h \
    demo/kruskaldemo.h \
    mainwindow.h \
    testing/questions/Question.h \
    testing/questions/questionwidget.h \
    testing/stat/Stat.h \
    testing/stat/statviewwidget.h \
    testing/stat/statwidget.h \
    testing/testing.h \
    testing/testingsettingswindow.h \
    testing/testingsystem.h \
    testing/welcometotesting.h \
    theory/theorysettingswindow.h \
    theory/theorysystem.h

FORMS += \
    demo/cyclesdemo.ui \
    demo/demosystem.ui \
    demo/kruskaldemo.ui \
    mainwindow.ui \
    testing/questions/questionwidget.ui \
    testing/stat/statviewwidget.ui \
    testing/stat/statwidget.ui \
    testing/testing.ui \
    testing/testingsettingswindow.ui \
    testing/testingsystem.ui \
    testing/welcometotesting.ui \
    theory/theorysettingswindow.ui \
    theory/theorysystem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
