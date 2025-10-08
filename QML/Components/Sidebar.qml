import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects
import "Elements"

Rectangle {
    id: root
    property Scene scene: null
    property int updateCallCount: 0

    property bool barvisible: false

    color: Constants.darkGrayA
    width: barvisible ? 430 : 80

    RowLayout {
        anchors.fill: parent
        // Tab navigation buttons (Canva-style vertical)
        Rectangle {
            id: rec
            Layout.preferredWidth: 70
            Layout.fillHeight: true // Height for 6 tabs
            color: "#2c2c2c"
            radius: 6

            Column {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 8

                Repeater {
                    id: tabRepeater
                    model: [
                        {
                            emoji: "📑",
                            name: "Objects",
                            type: "objects"
                        },
                        {
                            emoji: "🔧",
                            name: "Properties",
                            type: "properties"
                        },
                        {
                            emoji: "🎬",
                            name: "Animations",
                            type: "animations"
                        },
                        {
                            emoji: "▶️",
                            name: "Active",
                            type: "active"
                        },
                        {
                            emoji: "🧮",
                            name: "Values",
                            type: "values"
                        },
                        {
                            emoji: "📊",
                            name: "Trackers",
                            type: "trackers"
                        }
                    ]

                    Rectangle {
                        id: tabItem
                        required property var modelData
                        required property int index
                        property bool active: index === stackLayout.currentIndex

                        width: 60
                        height: 60
                        radius: 20
                        color: active && barvisible ? Constants.darkGrayC : "transparent"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.margins: 4
                        anchors.bottomMargin: 10
                        anchors.topMargin: 10
                        Column {
                            anchors.centerIn: parent
                            spacing: 2
                            Text {
                                text: tabItem.modelData.emoji
                                font.pixelSize: 28
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                text: tabItem.modelData.name
                                font.pixelSize: 10
                                color: active ? "#5ce1e6" : "#cccccc"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (tabItem.active) {
                                    barvisible = !barvisible;
                                } else {
                                    barvisible = true;
                                }
                                stackLayout.currentIndex = tabItem.index;
                            }
                        }
                    }
                }
            }
        }

        // Content area
        ScrollView {
            id: scr
            Layout.preferredHeight: root.height
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            visible: barvisible

            StackLayout {
                id: stackLayout
                anchors.fill: parent
                anchors.margins: 20
                currentIndex: 0

                // Objects tab content (from Menubar)
                Rectangle {
                    color: "transparent"

                    GridLayout {
                        id: gridLayout
                        columns: 4
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.margins: 5
                        columnSpacing: 6
                        rowSpacing: 6

                        Repeater {
                            model: MobjectRegistry {}
                            delegate: MButton {
                                basecolor: root.color
                                textval: name
                                path: iconpath
                                size: 60
                                callfunc: () => {
                                    console.log("Adding mobject:", name);
                                    canvas.add_mobject(name);
                                }
                            }
                        }
                    }
                }

                // Properties tab (from ControlPanel)
                MpropertiesEditor {
                    id: editor
                    Layout.preferredWidth: root.width - 80
                    Layout.preferredHeight: scr.height - 20
                }

                // Animations tab (from ControlPanel)
                AnimInput {
                    manager: canvas.animator()
                    scene: canvas
                    Layout.preferredWidth: root.width - 80
                    Layout.preferredHeight: scr.height - 20
                }

                // Active animations tab (from ControlPanel)
                ActiveAnimationsList {
                    manager: canvas.animator()
                    scene: canvas
                    Layout.preferredWidth: root.width - 80
                    Layout.preferredHeight: scr.height - 20
                }

                // Values tab (from ControlPanel)
                ValueManagement {
                    scene: canvas
                    Layout.preferredWidth: root.width - 80
                    Layout.preferredHeight: scr.height - 20
                }

                // Tracker Data tab
                TrackerDataDisplay {
                    scene: canvas
                    Layout.preferredWidth: root.width - 80
                    Layout.preferredHeight: scr.height - 20
                }
            }
        }
    }

    border.width: 2
    border.color: Qt.lighter(root.color, 1.1)

    RectangularShadow {
        anchors.fill: root
        radius: root.radius
        spread: 5
        color: Qt.lighter(root.color)
        z: -10
    }
}
