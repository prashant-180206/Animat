import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Position and Size Panel - Object positioning and dimensions
ColumnLayout {
    id: root
    
    property var mprop: null
    spacing: 12

    // Position
    ColumnLayout {
        visible: root.mprop && root.mprop.base
        Layout.fillWidth: true
        spacing: 6

        Text {
            text: "Position"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        PointInput {
            Layout.fillWidth: true
            pt: root.mprop && root.mprop.base ? root.mprop.base.pos : Qt.point(0, 0)
            func: function () {
                if (root.mprop && root.mprop.base)
                    root.mprop.base.pos = pt2;
            }
        }
    }

    // Size (not for Line/Curve)
    ColumnLayout {
        visible: root.mprop && root.mprop.base && root.mprop.type !== "Line" && root.mprop.type !== "Curve"
        Layout.fillWidth: true
        spacing: 6

        Text {
            text: "Size"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        PointInput {
            Layout.fillWidth: true
            pt: root.mprop && root.mprop.base ? root.mprop.base.size : Qt.point(0, 0)
            func: function () {
                if (root.mprop && root.mprop.base)
                    root.mprop.base.size = pt2;
            }
        }
    }
}