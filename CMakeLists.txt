cmake_minimum_required(VERSION 3.16)

project(Animat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)


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
        QML/Components/TitleBar.qml
        QML/Components/Taskbar.qml
        QML/Components/Menubar.qml
        QML/Components/ControlPanel.qml
        QML/Components/SceneManager.qml
        QML/Components/Elements/MButton.qml
        QML/Components/Elements/TextButton.qml
        QML/Components/Elements/ActiveAnimationsList.qml
        QML/Components/Elements/ValueManagement.qml
        QML/Components/Elements/MpropertiesEditor.qml
        QML/Components/Elements/Input/PointInput.qml
        QML/Components/Elements/Input/AnimInput.qml
        QML/Components/Elements/Input/AnimInputComponents/AnimationFieldsPanel.qml
        QML/Components/Elements/Input/AnimInputComponents/Fields/MobjectSelector.qml
        QML/Components/Elements/Input/AnimInputComponents/Fields/PropertyField.qml
        QML/Components/Elements/Input/AnimInputComponents/Fields/ValueFields.qml
        QML/Components/Elements/Input/AnimInputComponents/Fields/TimingFields.qml
        QML/Components/Elements/Input/AnimInputComponents/Controls/AnimationTypeSelector.qml
        QML/Components/Elements/Input/AnimInputComponents/Controls/AnimationButtons.qml
        QML/Components/Elements/Input/AnimInputComponents/Controls/AnimationNavigationControls.qml
        QML/Components/Elements/Input/AnimInputComponents/Display/AnimationListView.qml
        QML/Components/Elements/Input/AnimInputComponents/Display/AnimationLog.qml
        QML/Components/Elements/Input/PlaybackInput.qml
        QML/Components/Elements/Input/StyledTextField.qml
        QML/Components/Elements/Input/StyledTextArea.qml
        QML/Components/Elements/Input/StyledButton.qml
        QML/Components/Elements/Input/ColorPicker.qml
        QML/Components/Elements/Input/NumberInput.qml
    SOURCES
        Utils/Singleton.h
        Utils/Constants.h
        Utils/mobjectmap.h Utils/mobjectmap.cpp
        Utils/mobjectregistry.h Utils/mobjectregistry.cpp
        Math/ValueTracker/valuetracker.h Math/ValueTracker/valuetracker.cpp
        Math/ValueTracker/ptvaluetracker.h Math/ValueTracker/ptvaluetracker.cpp
        Math/ValueTracker/playbackslider.h Math/ValueTracker/playbackslider.cpp
        Math/Mobjects/line.h Math/Mobjects/line.cpp
        Math/Mobjects/Mobject.h Math/Mobjects/Mobject.cpp
        Math/Mobjects/Group.h Math/Mobjects/Group.cpp
        Math/Mobjects/Curve.h Math/Mobjects/Curve.cpp
        Math/Mobjects/Polygon.h Math/Mobjects/Polygon.cpp
        Math/Mobjects/MRectangle.h Math/Mobjects/MRectangle.cpp
        Math/Mobjects/Circle.h Math/Mobjects/Circle.cpp
        Math/Mobjects/Text.h Math/Mobjects/Text.cpp
        Math/Mobjects/SimpleLine.h Math/Mobjects/SimpleLine.cpp
        Math/Helper/ClickableMobject.h Math/Helper/ClickableMobject.cpp
        Math/Helper/mproperties.h Math/Helper/mproperties.cpp
        Math/Helper/Properties/baseproperties.h Math/Helper/Properties/baseproperties.cpp
        Math/Helper/Properties/lineproperties.h Math/Helper/Properties/lineproperties.cpp
        Math/Helper/Properties/curveproperties.h Math/Helper/Properties/curveproperties.cpp
        Math/Helper/Properties/circleproperties.h Math/Helper/Properties/circleproperties.cpp
        Math/Helper/Properties/polygonproperties.h Math/Helper/Properties/polygonproperties.cpp
        Math/Helper/Properties/textproperties.h Math/Helper/Properties/textproperties.cpp
        Math/Parser/trackermanager.h Math/Parser/trackermanager.cpp
        Math/Parser/parser.h Math/Parser/parser.cpp
        Math/Parser/trackercommand.h Math/Parser/trackercommand.cpp
        Math/Parser/animationscriptparser.h Math/Parser/animationscriptparser.cpp
        Math/Parser/dependencygraph.h Math/Parser/dependencygraph.cpp
        Math/Animations/animation.h Math/Animations/animation.cpp
        Math/Animations/animationmanager.h Math/Animations/animationmanager.cpp
        Math/Animations/animTypes/MoveAnimation.h Math/Animations/animTypes/MoveAnimation.cpp
        Math/Animations/animTypes/CreateAnimation.h Math/Animations/animTypes/CreateAnimation.cpp
        Math/Animations/animTypes/DestroyAnimation.h Math/Animations/animTypes/DestroyAnimation.cpp
        Math/Animations/animTypes/CustomScalarAnimation.h Math/Animations/animTypes/CustomScalarAnimation.cpp
        Math/Animations/animTypes/CustomPointAnimation.h Math/Animations/animTypes/CustomPointAnimation.cpp
        Math/Animations/animTypes/ValueAnimation.h Math/Animations/animTypes/ValueAnimation.cpp
        Math/Animations/animTypes/WaitAnimation.h Math/Animations/animTypes/WaitAnimation.cpp
        Math/Animations/AllAnimations.h
        Math/Animations/animpacket.h Math/Animations/animpacket.cpp
        Math/Managers/funcmap.h Math/Managers/funcmap.cpp
    RESOURCES
        lib/muparser.pc.in
        lib/muparserConfig.cmake.in
        Resources.qrc
)

target_link_libraries(appAnimat
    PRIVATE Qt6::Core Qt6::Quick muparser
)

target_include_directories(appAnimat
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/include
    PRIVATE ${CMAKE_SOURCE_DIR}/Math
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Helper
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Helper/Properties
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Mobjects
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/ValueTracker
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Managers
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Animations
    PRIVATE ${CMAKE_SOURCE_DIR}/Math/Parser
    PRIVATE ${CMAKE_SOURCE_DIR}/Utils
)

include(GNUInstallDirs)
install(TARGETS appAnimat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
