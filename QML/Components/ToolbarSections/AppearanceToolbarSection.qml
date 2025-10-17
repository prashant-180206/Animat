import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Elements"
import "../Elements/Input"
import Animat 1.0

// Colors grouped
RowLayout {
    id: root
    property MProperties mprop: null
    visible: mprop !== null
    spacing: 10

    ColorPicker {
        selectedColor: root.mprop && root.mprop.base ? root.mprop.base.color : "transparent"
        visible: root.mprop && root.mprop.base
        func: () => {
            if (root.mprop && root.mprop.base)
                root.mprop.base.color = newColor;
        }
    }
    ColorPicker {
        visible: root.mprop && root.mprop.polygon
        selectedColor: root.mprop && root.mprop.polygon ? root.mprop.polygon.borderColor : "transparent"
        func: () => {
            if (root.mprop && root.mprop.polygon)
                root.mprop.polygon.borderColor = newColor;
        }
    }

    // Geometry controls grouped

    PointInput {
        Layout.fillWidth: true
        label: "Pos:"
        pt: root.mprop && root.mprop.base ? root.mprop.base.pos : Qt.point(0, 0)
        func: function () {
            if (root.mprop && root.mprop.base)
                root.mprop.base.pos = pt2;
        }
    }
    PointInput {
        Layout.fillWidth: true
        label: "Size:"
        pt: root.mprop && root.mprop.base ? root.mprop.base.size : Qt.point(0, 0)
        visible: root.mprop && root.mprop.base && root.mprop.base.type !== "Line" &&
                 root.mprop.base.type !== "Curve" && root.mprop.base.type !== "Circle"

        func: function () {
            if (root.mprop && root.mprop.base)
                root.mprop.base.size = pt2;
        }
    }

    // Other property controls grouped

    // Opacity
    RowLayout {
        spacing: 4
        Text {
            text: "Opacity:"
            color: "#cccccc"
            font.pixelSize: 12
            Layout.preferredWidth: 60
        }
        MSlider {
            Layout.preferredWidth: 80
            minValue: 0
            maxValue: 1
            currentValue: root.mprop && root.mprop.base ? root.mprop.base.opacity : 0.0
            onValueUpdated: v => {
                if (root.mprop && root.mprop.base)
                    root.mprop.base.opacity = v;
            }
        }
    }

    NumberInput {
        visible: root.mprop && root.mprop.base
        label: "Z-Index:"
        value: root.mprop && root.mprop.base ? root.mprop.base.zindex : 0.0
        func: () => {
            if (root.mprop && root.mprop.base)
                root.mprop.base.zindex = newValue;
        }
    }

    NumberInput {
        visible: root.mprop && root.mprop.base
        label: "Scale:"
        value: root.mprop && root.mprop.base ? root.mprop.base.scale : 0.0
        func: () => {
                  if (root.mprop && root.mprop.base)
                  root.mprop.base.scale = newValue;
              }
    }

    NumberInput {
        visible: root.mprop && root.mprop.polygon
        label: "Thickness:"
        value: root.mprop && root.mprop.polygon ? root.mprop.polygon.thickness : 0
        func: () => {
            if (root.mprop && root.mprop.polygon)
                root.mprop.polygon.thickness = newValue;
        }
    }

    NumberInput {
        label: "Thick:"
        visible: root.mprop && root.mprop.curve
        Layout.fillWidth: true
        value: root.mprop && root.mprop.curve ? root.mprop.curve.thickness : 2
        decimals: 1
        func: () => {
            if (root.mprop && root.mprop.curve)
                root.mprop.curve.thickness = newValue;
        }
    }

    NumberInput {
        label: "Radius:"
        visible: root.mprop && root.mprop.circle
        Layout.fillWidth: true
        value: root.mprop && root.mprop.circle ? root.mprop.circle.radius : 2
        decimals: 1
        func: () => {
            if (root.mprop && root.mprop.circle)
                root.mprop.circle.radius = newValue;
        }
    }
}
