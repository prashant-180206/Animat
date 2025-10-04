import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0

Column {
    id: root
    width: parent.width
    spacing: 4
    visible: animationType !== "Value" && animationType !== "Wait"

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"
    property string animationType: "Move"
    property Scene scene

    property alias currentText: mobjField.currentText

    Text {
        text: "Mobject ID *"
        color: root.labelColor
        font.pixelSize: 12
    }

    ComboBox {
        id: mobjField
        width: parent.width
        // Use a direct property binding that updates when scene or mobjectsChanged
        model: root.scene ? root.scene.mobjectIds : []
        displayText: currentIndex >= 0 ? currentText : "Select mobject..."

        // Debug the model changes
        onModelChanged: {

            if (model && model.length > 0) {
            } else {
            }
        }

        background: Rectangle {
            color: root.inputBackgroundColor
            radius: 4
            border.color: root.borderColor
        }

        contentItem: Text {
            text: mobjField.displayText
            color: root.inputTextColor
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            leftPadding: 8
        }

        popup: Popup {
            y: mobjField.height
            width: mobjField.width
            height: Math.min(contentItem.implicitHeight, 150)

            background: Rectangle {
                color: root.inputBackgroundColor
                border.color: root.borderColor
                radius: 4
            }

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: mobjField.popup.visible ? mobjField.delegateModel : null
                currentIndex: mobjField.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator {}
            }
        }

        delegate: ItemDelegate {
            width: mobjField.width
            height: 32

            background: Rectangle {
                color: parent.hovered ? "#333333" : "transparent"
                radius: 2
            }

            contentItem: Text {
                text: modelData
                color: root.inputTextColor
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                leftPadding: 8
            }
        }
    }
}
