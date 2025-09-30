import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0
import "AnimInputComponents/Controls"
import "AnimInputComponents/Display"
import "AnimInputComponents"

Item {
    id: root
    width: 320
    height: 600

    property color backgroundColor: "#121212"
    property AnimManager manager
    property var scene

    Rectangle {
        anchors.fill: parent
        color: root.backgroundColor
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

            onAddAnimationClicked: root.addAnimation()
            onAddPacketClicked: root.addPacket()
            onClearAllClicked: root.clearAll()
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

        // Check values for animations that need them (exclude Wait and animations that auto-capture start values)
        if (type === "Value") {
            // Value animations still need both start and target
            if (!fieldsPanel.startValue.trim()) {
                errors.push("Start value is required");
            }
            if (!fieldsPanel.targetValue.trim()) {
                errors.push("Target value is required");
            }
        } else if (type === "Move" || type === "CustomScalar" || type === "CustomPoint") {
            // These animations only need target value (start value auto-captured)
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
        let duration = parseFloat(fieldsPanel.duration) || 1;
        let easingCurve = fieldsPanel.easingCurve || "InOutQuad";
        let startVal = (type === "Value" && fieldsPanel.startValue.trim() !== "") ? parseVal(fieldsPanel.startValue) : undefined;
        let targetVal = fieldsPanel.targetValue.trim() !== "" ? parseVal(fieldsPanel.targetValue) : undefined;

        logPanel.appendLog(`Type: ${type}`);
        if (mobjId)
            logPanel.appendLog(`Mobject: ${mobjId}`);
        if (prop)
            logPanel.appendLog(`Property: ${prop}`);
        logPanel.appendLog(`Duration: ${duration}s, Easing: ${easingCurve}`);

        // Add to animation list
        let animationData = {
            type: type,
            mobject: type === "Wait" ? "N/A" : mobjId,
            property: prop,
            duration: duration,
            easingCurve: easingCurve,
            values: ""
        };

        if (type === "Wait") {
            animationData.values = "Wait/Delay";
        } else if (type === "Value") {
            // Value animations still show start → target
            animationData.values = startVal.toString() + " → " + targetVal.toString();
        } else if (targetVal !== undefined) {
            // Move, CustomScalar, CustomPoint show: current → target
            animationData.values = "current → " + targetVal.toString();
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

                // Call addAnimation with appropriate parameters based on animation type
                if (type === "Value") {
                    // Value animations still need both start and target values
                    root.manager.packetToAdd.addAnimation(type, mobj, startVal, targetVal, prop, duration, easingCurve);
                } else if (type === "Move" || type === "CustomScalar" || type === "CustomPoint") {
                    // These animations only need target value (start auto-captured)
                    root.manager.packetToAdd.addAnimation(type, mobj, undefined, targetVal, prop, duration, easingCurve);
                } else {
                    // Other animations (Create, Destroy, Wait) don't need values
                    root.manager.packetToAdd.addAnimation(type, mobj, startVal, targetVal, prop, duration, easingCurve);
                }
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
