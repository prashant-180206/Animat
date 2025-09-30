import QtQuick 2.15
import QtQuick.Controls.Basic

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"

    property alias duration: durationField.text
    property alias easingCurve: easingComboBox.currentText

    // Easing Curve selection
    Column {
        width: parent.width
        spacing: 4

        Text {
            text: "Easing Curve"
            color: root.labelColor
            font.pixelSize: 12
        }

        ComboBox {
            id: easingComboBox
            width: parent.width

            model: ["Linear", "InOutQuad", "InQuad", "OutQuad", "InBounce", "OutBounce"]
            currentIndex: 1 // Default to InOutQuad

            background: Rectangle {
                color: root.inputBackgroundColor
                radius: 4
                border.color: root.borderColor
            }

            contentItem: Text {
                leftPadding: 10
                text: easingComboBox.displayText
                font.pixelSize: 14
                color: root.inputTextColor
                verticalAlignment: Text.AlignVCenter
            }

            popup: Popup {
                y: easingComboBox.height - 1
                width: easingComboBox.width
                implicitHeight: contentItem.implicitHeight
                padding: 1

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: easingComboBox.popup.visible ? easingComboBox.delegateModel : null
                    currentIndex: easingComboBox.highlightedIndex
                }

                background: Rectangle {
                    color: root.inputBackgroundColor
                    border.color: root.borderColor
                    radius: 4
                }
            }

            delegate: ItemDelegate {
                width: easingComboBox.width
                height: 35

                contentItem: Text {
                    text: modelData
                    color: root.inputTextColor
                    font.pixelSize: 14
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }

                background: Rectangle {
                    color: parent.hovered ? "#333333" : "transparent"
                    radius: 4
                }
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
        durationField.text = "1";
        easingComboBox.currentIndex = 1; // Reset to InOutQuad
    }
}
