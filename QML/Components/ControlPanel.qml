import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts
import "Elements" as Elements

Rectangle {
    id: root
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 340
    height: 500

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            id: txt
            text: "Control Panel"
            font.pointSize: 14
            color: "#5ce1e6"
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 30
        }

        Rectangle {
            id: tabContainer
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            color: "#2c2c2c"
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.margins: 4
                spacing: 4

                Repeater {
                    id: rp
                    model: [
                        {
                            text: "🔧 Properties",
                            emoji: "🔧",
                            name: "Properties"
                        },
                        {
                            text: "🎬 Animations",
                            emoji: "🎬",
                            name: "Animations"
                        },
                        {
                            text: "▶️ Active",
                            emoji: "▶️",
                            name: "Active"
                        },
                        {
                            text: "🧮 Values",
                            emoji: "🧮",
                            name: "Values"
                        }
                    ]
                    Rectangle {
                        id: tabRect
                        required property var modelData
                        required property int index
                        property bool active: index === stackLayout.currentIndex
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        color: active ? "#444" : "#1f1f1f"
                        border.color: active ? "#5ce1e6" : "transparent"
                        radius: 4

                        Column {
                            anchors.centerIn: parent
                            spacing: 2

                            Text {
                                text: tabRect.modelData.emoji
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pixelSize: 16
                            }

                            Text {
                                text: tabRect.modelData.name
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: tabRect.active ? "#5ce1e6" : "#888"
                                font.bold: tabRect.active
                                font.pixelSize: 8
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                stackLayout.currentIndex = tabRect.index;
                            }
                        }
                    }
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            StackLayout {
                id: stackLayout
                width: parent.width
                height: Math.max(parent.height, implicitHeight)
                currentIndex: 0

                MpropertiesEditor {
                    id: editor
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight
                    Layout.minimumHeight: 400
                }
                AnimInput {
                    manager: canvas.animator()
                    scene: canvas
                    Layout.fillWidth: true
                    Layout.preferredHeight: 600  // Use the AnimInput's designed height
                    Layout.minimumHeight: 600
                }
                Elements.ActiveAnimationsList {
                    manager: canvas.animator()
                    scene: canvas
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight
                    Layout.minimumHeight: 400
                }
                Elements.ValueManagement {
                    scene: canvas
                    Layout.fillWidth: true
                    Layout.preferredHeight: implicitHeight
                    Layout.minimumHeight: 400
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 200
                    color: "#1f1f1f"
                    Text {
                        anchors.centerIn: parent
                        text: "Settings Panel\n(Coming Soon)"
                        color: "#888"
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
