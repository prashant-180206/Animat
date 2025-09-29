import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0
import "Fields"

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color borderColor: "#444444"
    property string animationType: "Move"
    property Scene scene

    // Expose field values through the child components
    property alias mobjectId: mobjectSelector.currentText
    property alias propertyName: propertyField.text
    property alias startValue: valueFields.startValue
    property alias targetValue: valueFields.targetValue
    property alias startOffset: timingFields.startOffset
    property alias duration: timingFields.duration

    MobjectSelector {
        id: mobjectSelector
        width: parent.width
        animationType: root.animationType
        scene: root.scene
        labelColor: root.labelColor
        inputBackgroundColor: root.inputBackgroundColor
        inputTextColor: root.inputTextColor
        borderColor: root.borderColor
    }

    PropertyField {
        id: propertyField
        width: parent.width
        animationType: root.animationType
        labelColor: root.labelColor
        inputBackgroundColor: root.inputBackgroundColor
        inputTextColor: root.inputTextColor
        borderColor: root.borderColor
    }

    ValueFields {
        id: valueFields
        width: parent.width
        animationType: root.animationType
        labelColor: root.labelColor
        inputBackgroundColor: root.inputBackgroundColor
        inputTextColor: root.inputTextColor
        borderColor: root.borderColor
    }

    // // Info text about auto-capture feature
    // Text {
    //     width: parent.width
    //     text: {
    //         if (root.animationType === "Move" || root.animationType === "CustomScalar" || root.animationType === "CustomPoint") {
    //             return "ℹ️ Start value is automatically captured from object's current state";
    //         }
    //         return "";
    //     }
    //     color: "#888888"
    //     font.pixelSize: 11
    //     wrapMode: Text.WordWrap
    //     visible: text !== ""
    // }

    TimingFields {
        id: timingFields
        width: parent.width
        labelColor: root.labelColor
        inputBackgroundColor: root.inputBackgroundColor
        inputTextColor: root.inputTextColor
        borderColor: root.borderColor
    }

    // Debug connections for scene changes
    Connections {
        target: root.scene
        function onMobjectsChanged() {
            console.log("AnimationFieldsPanel: mobjectsChanged signal received");
            console.log("AnimationFieldsPanel: Updated mobject count:", root.scene ? root.scene.mobjectIds.length : 0);
            if (root.scene && root.scene.mobjectIds.length > 0) {
                console.log("AnimationFieldsPanel: Updated mobject IDs:", JSON.stringify(root.scene.mobjectIds));
            }
        }
        ignoreUnknownSignals: true
    }

    Component.onCompleted: {
        console.log("AnimationFieldsPanel: Component completed");
        console.log("AnimationFieldsPanel: Scene object:", root.scene);
        if (root.scene) {
            console.log("AnimationFieldsPanel: Scene getAllMobjectIds available:", typeof root.scene.getAllMobjectIds === "function");
            console.log("AnimationFieldsPanel: Scene mobjectIds property available:", root.scene.mobjectIds !== undefined);
            if (root.scene.getAllMobjectIds) {
                console.log("AnimationFieldsPanel: Initial mobject IDs (function):", JSON.stringify(root.scene.getAllMobjectIds()));
            }
            if (root.scene.mobjectIds !== undefined) {
                console.log("AnimationFieldsPanel: Initial mobject IDs (property):", JSON.stringify(root.scene.mobjectIds));
            }
        }
    }

    function clearFields() {
        propertyField.clearField();
        valueFields.clearFields();
        timingFields.clearFields();
    }
}
