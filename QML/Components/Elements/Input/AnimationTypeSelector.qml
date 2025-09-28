import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"
    property alias currentText: typeBox.currentText
    property alias currentIndex: typeBox.currentIndex

    signal typeChanged(string newType)

    Text {
        text: "Animation Type"
        color: labelColor
        font.pixelSize: 12
    }

    ComboBox {
        id: typeBox
        model: ["Move", "Create", "Destroy", "CustomScalar", "CustomPoint", "Value", "Wait"]
        currentIndex: 0
        width: parent.width

        background: Rectangle {
            color: inputBackgroundColor
            radius: 4
            border.color: borderColor
        }

        contentItem: Text {
            text: typeBox.displayText
            color: inputTextColor
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            leftPadding: 8
        }

        popup: Popup {
            y: typeBox.height
            width: typeBox.width
            height: Math.min(contentItem.implicitHeight, 200)

            background: Rectangle {
                color: inputBackgroundColor
                border.color: borderColor
                radius: 4
            }

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: typeBox.popup.visible ? typeBox.delegateModel : null
                currentIndex: typeBox.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator {}
            }
        }

        delegate: ItemDelegate {
            width: typeBox.width
            height: 32

            background: Rectangle {
                color: parent.hovered ? "#333333" : "transparent"
                radius: 2
            }

            contentItem: Text {
                text: modelData
                color: inputTextColor
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                leftPadding: 8
            }
        }

        onCurrentTextChanged: root.typeChanged(currentText)
    }
}
