import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Color Properties Panel - Fill and border color pickers
ColumnLayout {
    id: root
    
    property var mprop: null
    spacing: 12

    Text {
        text: "Colors"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    GridLayout {
        Layout.fillWidth: true
        columns: 2
        rowSpacing: 12
        columnSpacing: 16

        Text {
            text: "Fill Color"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        ColorPicker {
            selectedColor: root.mprop && root.mprop.base ? root.mprop.base.color : "transparent"
            func: () => {
                if (root.mprop && root.mprop.base)
                    root.mprop.base.color = newColor;
            }
        }

        Text {
            visible: root.mprop && root.mprop.polygon
            text: "Border Color"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        ColorPicker {
            visible: root.mprop && root.mprop.polygon
            selectedColor: root.mprop && root.mprop.polygon ? root.mprop.polygon.borderColor : "transparent"
            func: () => {
                if (root.mprop && root.mprop.polygon)
                    root.mprop.polygon.borderColor = newColor;
            }
        }
    }
}