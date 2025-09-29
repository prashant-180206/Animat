import QtQuick 2.15
import QtQuick.Controls.Basic

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"

    property alias startOffset: startOffsetField.text
    property alias duration: durationField.text

    // Start Offset field
    Column {
        width: parent.width
        spacing: 4

        Text {
            text: "Start Offset (seconds)"
            color: root.labelColor
            font.pixelSize: 12
        }

        TextField {
            id: startOffsetField
            text: "0"
            placeholderText: "0"
            width: parent.width
            color: root.inputTextColor

            background: Rectangle {
                color: root.inputBackgroundColor
                radius: 4
                border.color: root.borderColor
            }

            placeholderTextColor: "#888888"
            font.pixelSize: 14

            validator: DoubleValidator {
                bottom: 0
                decimals: 3
            }
        }
    }

    // Duration field
    Column {
        width: parent.width
        spacing: 4

        Text {
            text: "Duration (seconds) *"
            color: root.labelColor
            font.pixelSize: 12
        }

        TextField {
            id: durationField
            text: "1"
            placeholderText: "1"
            width: parent.width
            color: root.inputTextColor

            background: Rectangle {
                color: root.inputBackgroundColor
                radius: 4
                border.color: root.borderColor
            }

            placeholderTextColor: "#888888"
            font.pixelSize: 14

            validator: DoubleValidator {
                bottom: 0.001
                decimals: 3
            }
        }
    }

    function clearFields() {
        startOffsetField.text = "0";
        durationField.text = "1";
    }
}
