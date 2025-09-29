// StyledButton.qml - Reusable button component
import QtQuick 2.15
import QtQuick.Controls.Basic

Button {
    id: control
    property color backgroundColor: "#4a9eff"
    property color textColor: "#ffffff"
    property color hoverColor: "#357abd"
    property color pressedColor: "#1a5490"
    property bool isDestructive: false

    background: Rectangle {
        color: {
            if (!control.enabled)
                return "#333333";
            if (control.pressed)
                return control.isDestructive ? "#b71c1c" : control.pressedColor;
            if (control.hovered)
                return control.isDestructive ? "#d32f2f" : control.hoverColor;
            return control.isDestructive ? "#c62828" : control.backgroundColor;
        }
        radius: 6
        border.color: control.activeFocus ? "#0078d4" : "transparent"
        border.width: control.activeFocus ? 2 : 0

        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
    }

    contentItem: Text {
        text: control.text
        color: control.enabled ? control.textColor : "#666666"
        font: control.font
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
