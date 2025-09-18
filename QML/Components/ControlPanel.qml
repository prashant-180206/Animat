import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts
import "Elements"

Rectangle {
    id: root
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 300
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

        TabBar {
            id: bar
            width: parent.width
            background: Rectangle {
                color: "#2c2c2c"
                radius: 4
                anchors.fill: parent
            }

            // Use ButtonGroup to ensure single checked TabButton
            ButtonGroup {
                id: buttonGroup
                exclusive: true
            }

            Repeater {
                id: rp
                model: ["Properties", "Animations"]
                TabButton {
                    required property string modelData
                    checkable: true

                    // Assign to exclusive ButtonGroup
                    ButtonGroup.group: buttonGroup
                    background: Rectangle {
                        radius: 4
                        anchors.fill: parent
                        color: active ? "#444" : "#1f1f1f"
                        border.color: active ? "#5ce1e6" : "transparent"
                    }

                    contentItem: Text {
                        text: modelData
                        anchors.centerIn: parent
                        color: active ? "#5ce1e6" : "#888"
                        font.bold: active
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    width: (root.width - (rp.count - 1) * 10) / rp.count
                    // 10 pixels spacing between tabs, divided evenly

                    // Spacing between tabs: margin right except last item
                    anchors.rightMargin: 10
                }
            }
        }

        StackLayout {
            id: stackLayout
            width: parent.width
            anchors.topMargin: 10
            height: parent.height - bar.height - txt.height - 40
            currentIndex: bar.currentIndex

            MpropertiesEditor {
                id: editor
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            AnimInput {
                packet: canvas.animator()
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
