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
    property alias duration: timingFields.duration
    property alias easingCurve: timingFields.easingCurve

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
            if (root.scene && root.scene.mobjectIds.length > 0) {
            }
        }
        ignoreUnknownSignals: true
    }

    Component.onCompleted: {
        if (root.scene) {
            if (root.scene.getAllMobjectIds) {  }
            if (root.scene.mobjectIds !== undefined) { }
        }
    }

    function clearFields() {
        propertyField.clearField();
        valueFields.clearFields();
        timingFields.clearFields();
    }
}
