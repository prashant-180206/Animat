import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

Column {
    id: root
    spacing: 8

    property color buttonColor: "#2a2a2a"
    property color buttonTextColor: "#eeeeee"
    property color borderColor: "#444444"
    property color deleteButtonColor: "#d32f2f"
    property int animationCount: 0
    property var manager

    signal addAnimationClicked
    signal addPacketClicked
    signal clearAllClicked

    // Add Animation and Add Packet buttons
    RowLayout {
        width: parent.width
        spacing: 5

        Button {
            text: "Add Animation"
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            background: Rectangle {
                radius: 6
                color: root.buttonColor
                border.color: root.borderColor
            }

            contentItem: Text {
                text: "Add Animation"
                color: root.buttonTextColor
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: root.addAnimationClicked()
        }

        Button {
            text: "Add Packet"
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            background: Rectangle {
                radius: 6
                color: root.buttonColor
                border.color: root.borderColor
            }

            contentItem: Text {
                text: "Add Packet"
                color: root.buttonTextColor
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: root.addPacketClicked()
        }
    }

    // Clear All Button
    Button {
        text: "Clear All Animations"
        width: parent.width
        height: 35
        visible: animationCount > 0

        background: Rectangle {
            radius: 6
            color: root.deleteButtonColor
            border.color: root.borderColor
        }

        contentItem: Text {
            text: "Clear All Animations"
            color: "white"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: root.clearAllClicked()
    }
}
