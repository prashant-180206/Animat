import QtQuick 2.15
import QtQuick.Controls.Basic

Column {
    id: root
    width: parent.width
    spacing: 4
    visible: animationType.indexOf("Custom") === 0

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"
    property string animationType: "Move"

    property alias text: propField.text

    Text {
        text: "Property Name *"
        color: root.labelColor
        font.pixelSize: 12
    }

    TextField {
        id: propField
        placeholderText: "e.g., opacity, rotation, scale"
        width: parent.width
        color: root.inputTextColor

        background: Rectangle {
            color: root.inputBackgroundColor
            radius: 4
            border.color: root.borderColor
        }

        placeholderTextColor: "#888888"
        font.pixelSize: 14
    }

    function clearField() {
        propField.text = "";
    }
}
