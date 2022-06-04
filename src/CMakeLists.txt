cmake_minimum_required(VERSION 3.20)
project(Kruskal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    set(CMAKE_PREFIX_PATH "D:/Programs/Qt/6.3.0/mingw_64")
else ()
    set(CMAKE_PREFIX_PATH "/home/vany/Qt/6.3.0/gcc_64")
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        SvgWidgets
        Sql
        REQUIRED)

add_executable(Kruskal
        main.cpp

        mainwindow.cpp mainwindow.h mainwindow.ui

        common/documents.h
        common/constants.h
        common/Error.h
        common/settingswindow.cpp common/settingswindow.h common/settingswindow.ui

        testing/testingsystem.cpp testing/testingsystem.h testing/testingsystem.ui
        testing/questions/question.cpp testing/questions/question.h
        testing/welcometotesting.cpp testing/welcometotesting.h testing/welcometotesting.ui
        testing/stat/sessionstatwidget.cpp testing/stat/sessionstatwidget.h testing/stat/sessionstatwidget.ui
        testing/testingwidget.cpp testing/testingwidget.h testing/testingwidget.ui
        testing/questions/questionwidget.cpp testing/questions/questionwidget.h testing/questions/questionwidget.ui
        testing/stat/sessionstat.cpp testing/stat/sessionstat.h
        testing/stat/statdatabaseviewwidget.cpp testing/stat/statdatabaseviewwidget.h testing/stat/statdatabaseviewwidget.ui
        testing/stat/StatDatabaseViewOption.cpp testing/stat/StatDatabaseViewOption.h

        demo/GraphDemo.cpp demo/GraphDemo.h
        demo/demosystem.cpp demo/demosystem.h demo/demosystem.ui
        demo/graph/Graph.cpp demo/graph/Graph.h
        demo/graph/Edge.cpp demo/graph/Edge.h
        demo/graph/Vertex.cpp demo/graph/Vertex.h
        demo/kruskaldemo.cpp demo/kruskaldemo.h demo/kruskaldemo.ui
        demo/cyclesdemo.cpp demo/cyclesdemo.h demo/cyclesdemo.ui
        )

set(NoWebEngine true) # если переменная установлена, не компилируем исходники с компонентом QWebEngine
if (${NoWebEngine})
    add_compile_definitions("NOWEBENGINE")
    message("No Web Engine")
else()
    target_sources(Kruskal PUBLIC common/htmlviewer.cpp common/htmlviewer.h common/htmlviewer.ui)
endif()

target_link_libraries(Kruskal
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg
        Qt::SvgWidgets
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

    foreach (QT_LIB Core Gui Widgets Svg SvgWidgets Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

