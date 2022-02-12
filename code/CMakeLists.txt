cmake_minimum_required(VERSION 3.20)
project(Kruskal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        Sql
        REQUIRED)

add_executable(asd
        main.cpp

        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        common/documents.cpp
        common/documents.h
        common/constants.h
        common/Error.h

        testing/testingsystem.cpp
        testing/testingsystem.h
        testing/testingsystem.ui

        testing/questions/Question.cpp
        testing/questions/Question.h

        testing/testingsettingswindow.cpp
        testing/testingsettingswindow.h
        testing/testingsettingswindow.ui

        testing/welcometotesting.cpp
        testing/welcometotesting.h
        testing/welcometotesting.ui

        testing/stat/statwidget.cpp
        testing/stat/statwidget.h
        testing/stat/statwidget.ui

        testing/testing.cpp
        testing/testing.h
        testing/testing.ui

        testing/questions/questionwidget.cpp
        testing/questions/questionwidget.h
        testing/questions/questionwidget.ui

        testing/stat/Stat.cpp
        testing/stat/Stat.h

#        theory/theorysystem.cpp
#        theory/theorysystem.h
#        theory/theorysystem.ui
#
#        theory/theorysettingswindow.cpp
#        theory/theorysettingswindow.h
#        theory/theorysettingswindow.ui

        demo/demosystem.cpp
        demo/demosystem.h
        demo/demosystem.ui

        demo/graph/Graph.cpp
        demo/graph/Graph.h

        demo/graph/Edge.cpp
        demo/graph/Edge.h

        demo/graph/Vertex.cpp
        demo/graph/Vertex.h

        demo/kruskaldemo.cpp
        demo/kruskaldemo.h
        demo/kruskaldemo.ui

        demo/cyclesdemo.cpp
        demo/cyclesdemo.h
        demo/cyclesdemo.ui

        testing/stat/statviewwidget.cpp testing/stat/statviewwidget.h testing/stat/statviewwidget.ui)

target_link_libraries(asd
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg
        Qt::Sql
        )

if (WIN32)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    if (EXISTS "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
    endif ()

    foreach (QT_LIB Core Gui Widgets Svg Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
