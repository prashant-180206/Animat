import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"
    property string animationType: "Move"
    property Scene scene

    // Expose field values
    property alias mobjectId: mobjField.currentText
    property alias propertyName: propField.text
    property alias startValue: startValField.text
    property alias targetValue: targetValField.text
    property alias startOffset: startOffsetField.text
    property alias duration: durationField.text

    // Mobject ID field (for all types except Value and Wait)
    Column {
        id: mobjSection
        width: parent.width
        spacing: 4
        visible: animationType !== "Value" && animationType !== "Wait"

        Text {
            text: "Mobject ID *"
            color: root.labelColor
            font.pixelSize: 12
        }

        ComboBox {
            id: mobjField
            width: parent.width
            // Use the function call for now, will be updated by the Connections block
            model: root.scene ? root.scene.getAllMobjectIds() : []
            displayText: currentIndex >= 0 ? currentText : "Select mobject..."

            // Debug the model changes
            onModelChanged: {
                console.log("AnimationFieldsPanel: ComboBox model changed, count:", model ? model.length : 0);
                if (model && model.length > 0) {
                    console.log("AnimationFieldsPanel: Mobject IDs:", JSON.stringify(model));
                } else {
                    console.log("AnimationFieldsPanel: No mobjects found");
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

    // Property field (for Custom animations)
    Column {
        id: propSection
        width: parent.width
        spacing: 4
        visible: animationType.indexOf("Custom") === 0

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
    }

    // Start Position/Value field
    Column {
        id: startValSection
        width: parent.width
        spacing: 4
        visible: animationType === "Move" || animationType === "CustomScalar" || animationType === "CustomPoint" || animationType === "Value"

        Text {
            text: getStartFieldLabel()
            color: root.labelColor
            font.pixelSize: 12
        }

        TextField {
            id: startValField
            placeholderText: getStartFieldPlaceholder()
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
    }

    // Target Position/Value field
    Column {
        id: targetValSection
        width: parent.width
        spacing: 4
        visible: animationType === "Move" || animationType === "CustomScalar" || animationType === "CustomPoint" || animationType === "Value"

        Text {
            text: getTargetFieldLabel()
            color: root.labelColor
            font.pixelSize: 12
        }

        TextField {
            id: targetValField
            placeholderText: getTargetFieldPlaceholder()
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
    }

    // Start Offset
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
            placeholderText: "Start time offset"
            text: "0"
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
    }

    // Duration
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
            placeholderText: animationType === "Wait" ? "Wait/delay duration in seconds" : "Animation duration"
            text: "1"
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
    }

    // Helper functions
    function getStartFieldLabel() {
        switch (animationType) {
        case "Move":
            return "Start Position *";
        case "CustomScalar":
            return "Start Value *";
        case "CustomPoint":
            return "Start Point *";
        case "Value":
            return "Start Value *";
        case "Wait":
            return "";
        default:
            return "Start Value";
        }
    }

    function getStartFieldPlaceholder() {
        switch (animationType) {
        case "Move":
            return "x,y (e.g., 100,200)";
        case "CustomScalar":
            return "Number (e.g., 0.5)";
        case "CustomPoint":
            return "x,y (e.g., 10,20)";
        case "Value":
            return "Number (e.g., 0)";
        case "Wait":
            return "";
        default:
            return "Start value";
        }
    }

    function getTargetFieldLabel() {
        switch (animationType) {
        case "Move":
            return "Target Position *";
        case "CustomScalar":
            return "Target Value *";
        case "CustomPoint":
            return "Target Point *";
        case "Value":
            return "End Value *";
        default:
            return "Target Value";
        }
    }

    function getTargetFieldPlaceholder() {
        switch (animationType) {
        case "Move":
            return "x,y (e.g., 300,400)";
        case "CustomScalar":
            return "Number (e.g., 1.0)";
        case "CustomPoint":
            return "x,y (e.g., 50,80)";
        case "Value":
            return "Number (e.g., 100)";
        case "Wait":
            return "";
        default:
            return "Target value";
        }
    }

    // Explicit connection to mobjectsChanged signal for debugging
    Connections {
        target: root.scene
        function onMobjectsChanged() {
            console.log("AnimationFieldsPanel: mobjectsChanged signal received");
            // Force model update
            mobjField.model = root.scene ? root.scene.getAllMobjectIds() : [];
        }
        ignoreUnknownSignals: true
    }

    Component.onCompleted: {
        console.log("AnimationFieldsPanel: Component completed");
        console.log("AnimationFieldsPanel: Scene object:", root.scene);
        if (root.scene) {
            console.log("AnimationFieldsPanel: Scene getAllMobjectIds available:", typeof root.scene.getAllMobjectIds === "function");
            if (root.scene.getAllMobjectIds) {
                console.log("AnimationFieldsPanel: Initial mobject IDs:", JSON.stringify(root.scene.getAllMobjectIds()));
            }
        }
    }

    function clearFields() {
        propField.text = "";
        startValField.text = "";
        targetValField.text = "";
        startOffsetField.text = "0";
        durationField.text = "1";
    }
}
