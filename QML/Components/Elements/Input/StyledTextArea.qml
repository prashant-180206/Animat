// StyledTextArea.qml - Reusable text area component
import QtQuick 2.15
import QtQuick.Controls.Basic

TextArea {
    id: control
    property color backgroundColor: "#1a1a1a"
    property color textColor: "#ffffff"
    property color borderColor: "#333333"
    property color placeholderColor: "#666666"

    color: textColor
    selectByMouse: true
    wrapMode: TextArea.Wrap
    placeholderTextColor: placeholderColor

    background: Rectangle {
        color: backgroundColor
        radius: 6
        border.color: control.activeFocus ? "#0078d4" : borderColor
        border.width: 1

        Behavior on border.color {
            ColorAnimation {
                duration: 150
            }
        }
    }
}
