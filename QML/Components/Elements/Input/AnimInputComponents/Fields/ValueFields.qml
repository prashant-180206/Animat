import QtQuick 2.15
import QtQuick.Controls.Basic

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"
    property string animationType: "Move"

    property alias startValue: startValField.text
    property alias targetValue: targetValField.text

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

    function clearFields() {
        startValField.text = "";
        targetValField.text = "";
    }
}
