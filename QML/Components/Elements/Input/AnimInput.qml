import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0

Item {
    id: root
    width: 320
    height: 600

    property color backgroundColor: "#121212"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color labelColor: "#bbbbbb"
    property color borderColor: "#444444"
    property color buttonColor: "#2a2a2a"
    property color buttonTextColor: "#eeeeee"
    property color listItemColor: "#333333"
    property color deleteButtonColor: "#d32f2f"
    property AnimManager manager
    property var scene: null  // Scene reference to get mobject IDs

    // Update ComboBox model when mobjects change
    Connections {
        target: scene
        function onMobjectsChanged() {
            if (mobjField) {
                mobjField.model = scene ? scene.getAllMobjectIds() : [];
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: backgroundColor
        radius: 6
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        // Animation Type Selection
        Text {
            text: "Animation Type"
            color: labelColor
            font.pixelSize: 12
        }

        ComboBox {
            id: typeBox
            model: ["Move", "Create", "Destroy", "CustomScalar", "CustomPoint", "Value"]
            currentIndex: 0
            Layout.fillWidth: true
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
            onCurrentTextChanged: updateFieldVisibility()
        }

        // Dynamic fields based on animation type
        Column {
            Layout.fillWidth: true
            spacing: 8

            // Mobject ID field (for all types except Value)
            Column {
                id: mobjSection
                width: parent.width
                spacing: 4
                visible: typeBox.currentText !== "Value"

                Text {
                    text: "Mobject ID *"
                    color: labelColor
                    font.pixelSize: 12
                }

                ComboBox {
                    id: mobjField
                    width: parent.width
                    model: scene ? scene.getAllMobjectIds() : []
                    displayText: currentIndex >= 0 ? currentText : "Select mobject..."
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    font.pixelSize: 14

                    // Custom text property to maintain compatibility
                    property string text: currentText
                }
            }

            // Property field (for Custom animations)
            Column {
                id: propSection
                width: parent.width
                spacing: 4
                visible: typeBox.currentText.indexOf("Custom") === 0

                Text {
                    text: "Property Name *"
                    color: labelColor
                    font.pixelSize: 12
                }

                TextField {
                    id: propField
                    placeholderText: "e.g., opacity, rotation, scale"
                    width: parent.width
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    font.pixelSize: 14
                }
            }

            // Start Position/Value field (for Move, CustomScalar, CustomPoint, Value)
            Column {
                id: startValSection
                width: parent.width
                spacing: 4
                visible: typeBox.currentText === "Move" || typeBox.currentText === "CustomScalar" || typeBox.currentText === "CustomPoint" || typeBox.currentText === "Value"

                Text {
                    text: getStartFieldLabel()
                    color: labelColor
                    font.pixelSize: 12
                }

                TextField {
                    id: startValField
                    placeholderText: getStartFieldPlaceholder()
                    width: parent.width
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    font.pixelSize: 14
                }
            }

            // Target Position/Value field (for Move, CustomScalar, CustomPoint, Value)
            Column {
                id: targetValSection
                width: parent.width
                spacing: 4
                visible: typeBox.currentText === "Move" || typeBox.currentText === "CustomScalar" || typeBox.currentText === "CustomPoint" || typeBox.currentText === "Value"

                Text {
                    text: getTargetFieldLabel()
                    color: labelColor
                    font.pixelSize: 12
                }

                TextField {
                    id: targetValField
                    placeholderText: getTargetFieldPlaceholder()
                    width: parent.width
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    font.pixelSize: 14
                }
            }

            // Start Offset
            Column {
                width: parent.width
                spacing: 4

                Text {
                    text: "Start Offset (seconds)"
                    color: labelColor
                    font.pixelSize: 12
                }

                TextField {
                    id: startOffsetField
                    placeholderText: "Start time offset"
                    text: "0"
                    width: parent.width
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    font.pixelSize: 14
                }
            }

            // Duration
            Column {
                width: parent.width
                spacing: 4

                Text {
                    text: "Duration (seconds) *"
                    color: labelColor
                    font.pixelSize: 12
                }

                TextField {
                    id: durationField
                    placeholderText: "Animation duration"
                    text: "1"
                    width: parent.width
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    font.pixelSize: 14
                }
            }
        }

        // Add Animation Button
        Button {
            text: "Add Animation"
            Layout.fillWidth: true
            background: Rectangle {
                radius: 6
                color: buttonColor
                border.color: borderColor
            }
            contentItem: Text {
                text: "Add Animation"
                color: buttonTextColor
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: addAnimation()
        }

        Button {
            text: "Add Packet"
            Layout.fillWidth: true
            background: Rectangle {
                radius: 6
                color: buttonColor
                border.color: borderColor
            }
            contentItem: Text {
                text: "Add Packet"
                color: buttonTextColor
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                root.manager.add();
                animationListModel.clear();
            }
        }

        // Animation List Section
        Text {
            text: "Animation List (" + animationListModel.count + ")"
            color: labelColor
            font.pixelSize: 12
            font.bold: true
        }

        // Animation List
        ScrollView {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            clip: true

            ListView {
                id: animationList
                model: ListModel {
                    id: animationListModel
                }

                delegate: Rectangle {
                    width: animationList.width
                    height: 60
                    color: listItemColor
                    radius: 4
                    border.color: borderColor

                    Row {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        Column {
                            width: parent.width - deleteBtn.width - 16
                            anchors.verticalCenter: parent.verticalCenter

                            Text {
                                text: model.type + (model.mobject ? " - " + model.mobject : "") + (model.property ? " (" + model.property + ")" : "")
                                color: inputTextColor
                                font.pixelSize: 12
                                font.bold: true
                                elide: Text.ElideRight
                                width: parent.width
                            }

                            Text {
                                text: "Offset: " + model.startOffset + "s, Duration: " + model.duration + "s"
                                color: labelColor
                                font.pixelSize: 10
                            }

                            Text {
                                text: model.values ? "Values: " + model.values : ""
                                color: labelColor
                                font.pixelSize: 10
                                visible: model.values !== ""
                                elide: Text.ElideRight
                                width: parent.width
                            }
                        }

                        Button {
                            id: deleteBtn
                            text: "×"
                            width: 30
                            height: 30
                            anchors.verticalCenter: parent.verticalCenter
                            background: Rectangle {
                                color: deleteButtonColor
                                radius: 15
                            }
                            contentItem: Text {
                                text: "×"
                                color: "white"
                                font.pixelSize: 16
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                animationListModel.remove(index);
                                logArea.text += "Removed animation at index " + index + "\n";
                            }
                        }
                    }
                }
            }
        }

        // Clear All Button
        Button {
            text: "Clear All Animations"
            Layout.fillWidth: true
            visible: animationListModel.count > 0
            background: Rectangle {
                radius: 6
                color: deleteButtonColor
                border.color: borderColor
            }
            contentItem: Text {
                text: "Clear All Animations"
                color: "white"
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                animationListModel.clear();
                logArea.text += "Cleared all animations\n";
            }
        }

        // Log area
        Text {
            text: "Log"
            color: labelColor
            font.pixelSize: 12
            font.bold: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            clip: true

            TextArea {
                id: logArea
                readOnly: true
                wrapMode: TextArea.Wrap
                font.family: "monospace"
                font.pixelSize: 10
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 6
                    border.color: borderColor
                }
                selectByMouse: true
            }
        }
    }

    // Helper functions
    function updateFieldVisibility() {
        // Force re-evaluation of visible properties
        mobjSection.visible = Qt.binding(() => typeBox.currentText !== "Value");
        propSection.visible = Qt.binding(() => typeBox.currentText.indexOf("Custom") === 0);
        startValSection.visible = Qt.binding(() => {
            return typeBox.currentText === "Move" || typeBox.currentText === "CustomScalar" || typeBox.currentText === "CustomPoint" || typeBox.currentText === "Value";
        });
        targetValSection.visible = Qt.binding(() => {
            return typeBox.currentText === "Move" || typeBox.currentText === "CustomScalar" || typeBox.currentText === "CustomPoint" || typeBox.currentText === "Value";
        });
    }

    function getStartFieldLabel() {
        switch (typeBox.currentText) {
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
        switch (typeBox.currentText) {
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
        switch (typeBox.currentText) {
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
        switch (typeBox.currentText) {
        case "Move":
            return "x,y (e.g., 300,400)";
        case "CustomScalar":
            return "Number (e.g., 1.0)";
        case "CustomPoint":
            return "x,y (e.g., 50,80)";
        case "Value":
            return "Number (e.g., 100)";
        default:
            return "Target value";
        }
    }

    function validateRequiredFields() {
        let type = typeBox.currentText;
        let errors = [];

        // Check duration
        if (!durationField.text || parseFloat(durationField.text) <= 0) {
            errors.push("Duration must be a positive number");
        }

        // Check mobject for non-Value animations
        if (type !== "Value" && !mobjField.text.trim()) {
            errors.push("Mobject ID is required");
        }

        // Check property for Custom animations
        if (type.indexOf("Custom") === 0 && !propField.text.trim()) {
            errors.push("Property name is required for custom animations");
        }

        // Check values for animations that need them
        if ((type === "Move" || type === "CustomScalar" || type === "CustomPoint" || type === "Value")) {
            if (!startValField.text.trim()) {
                errors.push("Start value is required");
            }
            if (!targetValField.text.trim()) {
                errors.push("Target value is required");
            }
        }

        return errors;
    }

    function parseVal(txt) {
        if (txt.indexOf(",") !== -1) {
            let parts = txt.split(",");
            return Qt.point(parseFloat(parts[0]), parseFloat(parts[1]));
        }
        return parseFloat(txt);
    }

    function addAnimation() {
        logArea.text += "=== Adding Animation ===\n";

        // Validate required fields
        let errors = validateRequiredFields();
        if (errors.length > 0) {
            logArea.text += "Validation errors:\n";
            errors.forEach(error => logArea.text += "- " + error + "\n");
            return;
        }

        let type = typeBox.currentText;
        let prop = propField.text.trim();
        let mobjId = mobjField.text.trim();
        let startOffset = parseFloat(startOffsetField.text) || 0;
        let duration = parseFloat(durationField.text) || 1;
        let startVal = startValField.text.trim() !== "" ? parseVal(startValField.text) : undefined;
        let targetVal = targetValField.text.trim() !== "" ? parseVal(targetValField.text) : undefined;

        logArea.text += `Type: ${type}\n`;
        if (mobjId)
            logArea.text += `Mobject: ${mobjId}\n`;
        if (prop)
            logArea.text += `Property: ${prop}\n`;
        logArea.text += `Start Offset: ${startOffset}s, Duration: ${duration}s\n`;

        // Add to animation list
        let animationData = {
            type: type,
            mobject: mobjId,
            property: prop,
            startOffset: startOffset,
            duration: duration,
            values: ""
        };

        if (startVal !== undefined && targetVal !== undefined) {
            animationData.values = startVal.toString() + " → " + targetVal.toString();
        }

        animationListModel.append(animationData);

        // Try to add to packet if available
        if (root.manager) {
            try {
                let mobj = null;
                if (type !== "Value") {
                    if (typeof canvas !== "undefined" && canvas.getMobject) {
                        mobj = canvas.getMobject(mobjId);
                        if (!mobj) {
                            logArea.text += `Warning: Mobject "${mobjId}" not found\n`;
                            return;
                        }
                    } else {
                        logArea.text += "Warning: canvas or getMobject() not available\n";
                        return;
                    }
                }

                root.manager.packetToAdd.addAnimation(type, mobj, startVal, targetVal, prop, startOffset, duration);
                logArea.text += "✓ Animation added to packet\n";
                logArea.text += "✓ Packet processed\n";
            } catch (error) {
                logArea.text += "Error adding animation: " + error + "\n";
            }
        } else {
            logArea.text += "Warning: No packet manager available\n";
        }

        // Clear form fields after successful addition
        clearFormFields();
        logArea.text += "=================\n";
    }

    function clearFormFields() {
        propField.text = "";
        mobjField.text = "";
        startValField.text = "";
        targetValField.text = "";
        startOffsetField.text = "0";
        durationField.text = "1";
    }

    Component.onCompleted: {
        updateFieldVisibility();
        logArea.text = "Animation UI ready\n";
    }
}
