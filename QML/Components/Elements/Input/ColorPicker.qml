// ColorPicker.qml - Reusable color picker component
import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Dialogs

Rectangle {
    id: root
    property color selectedColor: "transparent"
    property string label: ""
    property color newColor: selectedColor
    property var func: ()=>{}

    width: 100
    height: 32
    radius: 6
    color: selectedColor
    border.color: "#444444"
    border.width: 1

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: "transparent"
        border.color: "#0078d4"
        border.width: 2
        opacity: parent.hovered ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }

    property bool hovered: false

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.hovered = true
        onExited: parent.hovered = false
        onClicked: colorDialog.open()
    }

    ColorDialog {
        id: colorDialog
        selectedColor: root.selectedColor
        onAccepted: {
            newColor = selectedColor
            func()
        }
    }
}
