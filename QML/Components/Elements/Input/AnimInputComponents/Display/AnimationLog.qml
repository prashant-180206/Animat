import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputTextColor: "#ffffff"
    property color inputBackgroundColor: "#1e1e1e"
    property color borderColor: "#444444"
    property alias text: logArea.text

    Text {
        text: "Log"
        color: root.labelColor
        font.pixelSize: 12
        font.bold: true
    }

    ScrollView {
        width: parent.width
        height: 80
        clip: true

        TextArea {
            id: logArea
            readOnly: true
            wrapMode: TextArea.Wrap
            font.family: "monospace"
            font.pixelSize: 10
            color: root.inputTextColor

            background: Rectangle {
                color: root.inputBackgroundColor
                radius: 6
                border.color: root.borderColor
            }

            selectByMouse: true
            placeholderTextColor: "#666666"
        }
    }

    function appendLog(message) {
        logArea.text += message + "\n";
    }

    function clear() {
        logArea.text = "";
    }
}
