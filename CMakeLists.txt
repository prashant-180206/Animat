cmake_minimum_required(VERSION 3.16)

project(Animat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)


qt_standard_project_setup(REQUIRES 6.8)

# 🔹 Override muparser options BEFORE including it
# Disable wide char support before adding muparser
set(ENABLE_WIDE_CHAR OFF CACHE BOOL "Disable wide char support")
# set(ENABLE_WIDE_CHAR OFF CACHE BOOL "Disable wide char support" FORCE)
set(ENABLE_SAMPLES OFF CACHE BOOL "Disable samples" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable tests" FORCE)
add_subdirectory(lib)

qt_add_executable(appAnimat
    main.cpp
    Math/Scene.h Math/Scene.cpp
)

qt_add_qml_module(appAnimat
    URI Animat
    VERSION 1.0
    QML_FILES
    QML/Main.qml
        SOURCES
        QML_FILES
        QML_FILES
        SOURCES
        QML_FILES
        QML_FILES
        SOURCES Utils/Singleton.h
        SOURCES Utils/Constants.h
        QML_FILES QML/Components/TitleBar.qml
        QML_FILES QML/Components/Taskbar.qml
        QML_FILES QML/Components/Menubar.qml
        QML_FILES QML/Components/ControlPanel.qml
        QML_FILES QML/Components/SceneManager.qml
        RESOURCES lib/muparser.pc.in
        RESOURCES lib/muparserConfig.cmake.in
        SOURCES
        SOURCES Math/ValueTracker/valuetracker.h Math/ValueTracker/valuetracker.cpp
        SOURCES Math/Mobjects/line.h Math/Mobjects/line.cpp
        SOURCES Math/Mobjects/Mobject.h Math/Mobjects/Mobject.cpp
        SOURCES Math/Mobjects/Group.h Math/Mobjects/Group.cpp
        SOURCES
        SOURCES Math/Mobjects/Curve.h Math/Mobjects/Curve.cpp
        SOURCES Math/Mobjects/Polygon.h Math/Mobjects/Polygon.cpp
        SOURCES Math/Mobjects/MRectangle.h Math/Mobjects/MRectangle.cpp
        SOURCES Math/Mobjects/Circle.h Math/Mobjects/Circle.cpp
        SOURCES Math/Helper/Text.h Math/Helper/Text.cpp
        SOURCES Math/Helper/ClickableMobject.h
        SOURCES Math/Mobjects/SimpleLine.h Math/Mobjects/SimpleLine.cpp
        SOURCES
        SOURCES Utils/mobjectmap.h Utils/mobjectmap.cpp
        SOURCES Utils/mobjectregistry.h Utils/mobjectregistry.cpp
        RESOURCES Resources.qrc
        SOURCES Math/Helper/ClickableMobject.cpp
        SOURCES
        QML_FILES QML/Components/Elements/MButton.qml
        QML_FILES QML/Components/Elements/InputMap.qml
        QML_FILES QML/Components/Elements/TextButton.qml
        SOURCES
        SOURCES
        SOURCES Math/Helper/mproperties.h Math/Helper/mproperties.cpp
        SOURCES
        QML_FILES QML/Components/Elements/MpropertiesEditor.qml
        QML_FILES QML/Components/Elements/Input/PointInput.qml
)

target_link_libraries(appAnimat
    PRIVATE Qt6::Core Qt6::Quick muparser
)

target_include_directories(appAnimat
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/include
    PRIVATE ${CMAKE_SOURCE_DIR}/Math
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Helper
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Mobjects
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/ValueTracker
    PRIVATE ${CMAKE_SOURCE_DIR}/Utils
)


target_link_libraries(appAnimat
    PRIVATE Qt6::Quick
)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Core Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Core Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)


include(GNUInstallDirs)
install(TARGETS appAnimat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
