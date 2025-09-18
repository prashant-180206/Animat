// StyledTextField.qml - Reusable text field component
import QtQuick 2.15
import QtQuick.Controls.Basic

TextField {
    id: control
    property color backgroundColor: "#1e1e1e"
    property color textColor: "#ffffff"
    property color borderColor: "#444444"

    color: textColor
    selectByMouse: true

    background: Rectangle {
        color: backgroundColor
        radius: 6
        border.color: control.activeFocus ? "#0078d4" : borderColor
        border.width: 1

        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
    }
}
