cmake_minimum_required(VERSION 3.16)

project(Animat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appAnimat
    main.cpp
    Math/Scene.h Math/Scene.cpp
    Math/Mobjects/Mobject.h Math/Mobjects/Mobject.cpp

)

qt_add_qml_module(appAnimat
    URI Animat
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        QML_FILES
        QML_FILES
        SOURCES
        QML_FILES
        QML_FILES
        SOURCES Singleton.h
        SOURCES Constants.h
        QML_FILES Components/TitleBar.qml
        QML_FILES Components/Taskbar.qml
        QML_FILES Components/Menubar.qml
        QML_FILES Components/ControlPanel.qml
        QML_FILES Components/SceneManager.qml
)


target_link_libraries(appAnimat
    PRIVATE Qt6::Quick
)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)

include(GNUInstallDirs)
install(TARGETS appAnimat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
