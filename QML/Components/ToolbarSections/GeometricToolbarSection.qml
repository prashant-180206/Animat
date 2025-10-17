import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Elements"
import "../Elements/Input"
import Animat 1.0

RowLayout {
    id: root
    property MProperties mprop: null
    spacing: 10
    visible: mprop && mprop.geometric && mprop.geometric.angle && mprop.base && mprop.base.type === "Angle"
    height: 32

    // Angle input
    NumberInput {
        label: "Angle:"
        value: root.mprop && root.mprop.geometric && root.mprop.geometric.angle ? root.mprop.geometric.angle.angle : 45.0
        decimals: 1
        func: function() {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.angle)
                root.mprop.geometric.angle.angle = newValue;
        }
    }

    Rectangle {
        width: 1
        height: 20
        color: "#444444"
    }

    // Start Vector Length
    NumberInput {
        label: "Start Length:"
        value: root.mprop && root.mprop.geometric && root.mprop.geometric.angle ? root.mprop.geometric.angle.startVectorLength : 2.0
        decimals: 2
        func: function() {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.angle)
                root.mprop.geometric.angle.startVectorLength = newValue;
        }
    }

    // End Vector Length
    NumberInput {
        label: "End Length:"
        value: root.mprop && root.mprop.geometric && root.mprop.geometric.angle ? root.mprop.geometric.angle.endVectorLength : 2.0
        decimals: 2
        func: function() {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.angle)
                root.mprop.geometric.angle.endVectorLength = newValue;
        }
    }

    Rectangle {
        width: 1
        height: 20
        color: "#444444"
    }

    // Arc Radius
    NumberInput {
        label: "Arc Radius:"
        value: root.mprop && root.mprop.geometric && root.mprop.geometric.angle ? root.mprop.geometric.angle.arcRadius : 0.5
        decimals: 2
        func: function() {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.angle)
                root.mprop.geometric.angle.arcRadius = newValue;
        }
    }

    // Arc Thickness
    NumberInput {
        label: "Arc Thickness:"
        value: root.mprop && root.mprop.geometric && root.mprop.geometric.angle ? root.mprop.geometric.angle.arcThickness : 3.0
        decimals: 1
        func: function() {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.angle)
                root.mprop.geometric.angle.arcThickness = newValue;
        }
    }

    Rectangle {
        width: 1
        height: 20
        color: "#444444"
    }

    // Arc Color Picker
    ColorPicker {
        selectedColor: root.mprop && root.mprop.geometric && root.mprop.geometric.angle ? root.mprop.geometric.angle.arcColor : "green"
        func: () => {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.angle)
                root.mprop.geometric.angle.arcColor = newColor;
        }
    }
}