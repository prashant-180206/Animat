import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0

Item {
    id: root
    width: 320
    height: 600

    property color backgroundColor: "#121212"
    property AnimManager manager
    property var scene

    Rectangle {
        anchors.fill: parent
        color: backgroundColor
        radius: 6
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        AnimationTypeSelector {
            id: typeSelector
            Layout.fillWidth: true
            onTypeChanged: function (newType) {
                fieldsPanel.animationType = newType;
            }
        }

        AnimationFieldsPanel {
            id: fieldsPanel
            Layout.fillWidth: true
            scene: root.scene
            animationType: typeSelector.currentText
        }

        AnimationButtons {
            id: buttonsPanel
            Layout.fillWidth: true
            animationCount: listView.count
            manager: root.manager

            onAddAnimationClicked: addAnimation()
            onAddPacketClicked: addPacket()
            onClearAllClicked: clearAll()
        }

        AnimationListView {
            id: listView
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            onItemRemoved: function (index) {
                logPanel.appendLog("Removed animation at index " + index);
            }
        }

        AnimationLog {
            id: logPanel
            Layout.fillWidth: true
            Layout.preferredHeight: 80
        }
    }

    // Helper functions
    function validateRequiredFields() {
        let type = typeSelector.currentText;
        let errors = [];

        // Check duration
        if (!fieldsPanel.duration || parseFloat(fieldsPanel.duration) <= 0) {
            errors.push("Duration must be a positive number");
        }

        // Check mobject for animations that need them (exclude Value and Wait)
        if (type !== "Value" && type !== "Wait" && !fieldsPanel.mobjectId.trim()) {
            errors.push("Mobject ID is required");
        }

        // Check property for Custom animations
        if (type.indexOf("Custom") === 0 && !fieldsPanel.propertyName.trim()) {
            errors.push("Property name is required for custom animations");
        }

        // Check values for animations that need them (exclude Wait)
        if ((type === "Move" || type === "CustomScalar" || type === "CustomPoint" || type === "Value")) {
            if (!fieldsPanel.startValue.trim()) {
                errors.push("Start value is required");
            }
            if (!fieldsPanel.targetValue.trim()) {
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
        logPanel.appendLog("=== Adding Animation ===");

        // Validate required fields
        let errors = validateRequiredFields();
        if (errors.length > 0) {
            logPanel.appendLog("Validation errors:");
            errors.forEach(error => logPanel.appendLog("- " + error));
            return;
        }

        let type = typeSelector.currentText;
        let prop = fieldsPanel.propertyName.trim();
        let mobjId = fieldsPanel.mobjectId.trim();
        let startOffset = parseFloat(fieldsPanel.startOffset) || 0;
        let duration = parseFloat(fieldsPanel.duration) || 1;
        let startVal = fieldsPanel.startValue.trim() !== "" ? parseVal(fieldsPanel.startValue) : undefined;
        let targetVal = fieldsPanel.targetValue.trim() !== "" ? parseVal(fieldsPanel.targetValue) : undefined;

        logPanel.appendLog(`Type: ${type}`);
        if (mobjId)
            logPanel.appendLog(`Mobject: ${mobjId}`);
        if (prop)
            logPanel.appendLog(`Property: ${prop}`);
        logPanel.appendLog(`Start Offset: ${startOffset}s, Duration: ${duration}s`);

        // Add to animation list
        let animationData = {
            type: type,
            mobject: type === "Wait" ? "N/A" : mobjId,
            property: prop,
            startOffset: startOffset,
            duration: duration,
            values: ""
        };

        if (type === "Wait") {
            animationData.values = "Wait/Delay";
        } else if (startVal !== undefined && targetVal !== undefined) {
            animationData.values = startVal.toString() + " → " + targetVal.toString();
        }

        listView.addAnimation(animationData);

        // Try to add to packet if available
        if (root.manager) {
            try {
                let mobj = null;
                if (type !== "Value" && type !== "Wait") {
                    if (root.scene && root.scene.getMobject) {
                        mobj = root.scene.getMobject(mobjId);
                        if (!mobj) {
                            logPanel.appendLog(`Warning: Mobject "${mobjId}" not found`);
                            return;
                        }
                    } else {
                        logPanel.appendLog("Warning: scene or getMobject() not available");
                        return;
                    }
                }

                root.manager.packetToAdd.addAnimation(type, mobj, startVal, targetVal, prop, startOffset, duration);
                logPanel.appendLog("✓ Animation added to packet");
                logPanel.appendLog("✓ Packet processed");
            } catch (error) {
                logPanel.appendLog("Error adding animation: " + error);
            }
        } else {
            logPanel.appendLog("Warning: No packet manager available");
        }

        // Clear form fields after successful addition
        fieldsPanel.clearFields();
        logPanel.appendLog("=================");
    }

    function addPacket() {
        root.manager.add();
        listView.clearAll();
    }

    function clearAll() {
        listView.clearAll();
        logPanel.appendLog("Cleared all animations");
    }

    Component.onCompleted: {
        logPanel.appendLog("Animation UI ready");
        console.log("AnimInput: Scene object:", scene);
        console.log("AnimInput: Scene getAllMobjectIds available:", scene && typeof scene.getAllMobjectIds === "function");
        if (scene && scene.getAllMobjectIds) {
            console.log("AnimInput: Current mobject IDs (function):", JSON.stringify(scene.getAllMobjectIds()));
        }
        if (scene && scene.mobjectIds) {
            console.log("AnimInput: Current mobject IDs (property):", JSON.stringify(scene.mobjectIds));
        }
    }
}
