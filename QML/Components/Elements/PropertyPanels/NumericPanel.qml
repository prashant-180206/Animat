import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Numeric Properties Panel - Opacity, scale, rotation, etc.
ColumnLayout {
    id: root
    
    property var mprop: null
    spacing: 12

    Text {
        text: "Properties"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    NumberInput {
        visible: root.mprop && root.mprop.base
        label: "Opacity:"
        value: root.mprop && root.mprop.base ? root.mprop.base.opacity : 1.0
        func: () => {
            if (root.mprop && root.mprop.base)
                root.mprop.base.opacity = newValue;
        }
    }

    NumberInput {
        visible: root.mprop && root.mprop.base
        label: "Scale:"
        value: root.mprop && root.mprop.base ? root.mprop.base.scale : 1.0
        func: () => {
            if (root.mprop && root.mprop.base)
                root.mprop.base.scale = newValue;
        }
    }

    NumberInput {
        visible: root.mprop && root.mprop.base
        label: "Rotation:"
        value: root.mprop && root.mprop.base ? root.mprop.base.rotation : 0.0
        func: () => {
            if (root.mprop && root.mprop.base)
                root.mprop.base.rotation = newValue;
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
        visible: root.mprop && root.mprop.polygon
        label: "Thickness:"
        value: root.mprop && root.mprop.polygon ? root.mprop.polygon.thickness : 0
        func: () => {
            if (root.mprop && root.mprop.polygon)
                root.mprop.polygon.thickness = newValue;
        }
    }
}