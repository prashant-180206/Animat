cmake_minimum_required(VERSION 3.16)

project(Animat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
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
        RESOURCES lib/muparser.pc.in
        RESOURCES lib/muparserConfig.cmake.in
        SOURCES
        SOURCES Math/ValueTracker/valuetacker.h Math/ValueTracker/valuetacker.cpp
        SOURCES Math/Mobjects/line.h Math/Mobjects/line.cpp
        SOURCES Math/Mobjects/Mobject.h Math/Mobjects/Mobject.cpp
        SOURCES Math/Mobjects/Group.h Math/Mobjects/Group.cpp
        SOURCES
        SOURCES Math/Mobjects/Curve.h Math/Mobjects/Curve.cpp
        SOURCES Math/Mobjects/Polygon.h Math/Mobjects/Polygon.cpp
        SOURCES Math/Mobjects/Rectangle.h Math/Mobjects/Rectangle.cpp
        SOURCES Math/Mobjects/Circle.h Math/Mobjects/Circle.cpp
# SOURCES lib/include/muParser.h lib/include/muParserBase.h lib/include/muParserBytecode.h lib/include/muParserCallback.h lib/include/muParserDef.h lib/include/muParserDLL.h lib/include/muParserError.h lib/include/muParserFixes.h lib/include/muParserInt.h lib/include/muParserTemplateMagic.h lib/include/muParserTest.h lib/include/muParserToken.h lib/include/muParserTokenReader.h lib/src/muParser.cpp lib/src/muParserBase.cpp lib/src/muParserBytecode.cpp lib/src/muParserCallback.cpp lib/src/muParserDLL.cpp lib/src/muParserError.cpp lib/src/muParserInt.cpp lib/src/muParserTest.cpp lib/src/muParserTokenReader.cpp
# RESOURCES lib/CMakeLists.txt

)


target_link_libraries(appAnimat
    PRIVATE Qt6::Quick
)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE muparser)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)
target_link_libraries(appAnimat PRIVATE Qt6::Quick)

include(GNUInstallDirs)
install(TARGETS appAnimat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
