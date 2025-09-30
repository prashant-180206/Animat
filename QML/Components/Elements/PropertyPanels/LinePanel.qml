import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Line Properties Panel - Line endpoints and thickness
ColumnLayout {
    id: root

    property var mprop: null
    spacing: 12

    Text {
        text: "Line Properties"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    // Line endpoints
    GridLayout {
        Layout.fillWidth: true
        columns: 2
        rowSpacing: 12
        columnSpacing: 16

        // Start Point
        Text {
            text: "Start Point"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        PointInput {
            Layout.fillWidth: true
            label: ""
            pt: root.mprop && root.mprop.line ? root.mprop.line.lineStart : Qt.point(0, 0)
            func: function () {
                if (root.mprop && root.mprop.line)
                    root.mprop.line.lineStart = pt2;
            }
        }

        // End Point
        Text {
            text: "End Point"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        PointInput {
            Layout.fillWidth: true
            label: ""
            pt: root.mprop && root.mprop.line ? root.mprop.line.lineEnd : Qt.point(0, 0)
            func: function () {
                if (root.mprop && root.mprop.line)
                    root.mprop.line.lineEnd = pt2;
            }
        }

        // Thickness
        Text {
            text: "Thickness"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.line ? root.mprop.line.thickness : 2
            decimals: 1
            func: () => {
                if (root.mprop && root.mprop.line)
                    root.mprop.line.thickness = newValue;
            }
        }
    }

    // Line Length Display (Read-only)
    RowLayout {
        Layout.fillWidth: true
        spacing: 8

        Text {
            text: "Length:"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        Text {
            text: {
                if (root.mprop && root.mprop.line) {
                    var start = root.mprop.line.lineStart;
                    var end = root.mprop.line.lineEnd;
                    var dx = end.x - start.x;
                    var dy = end.y - start.y;
                    var length = Math.sqrt(dx * dx + dy * dy);
                    return length.toFixed(2);
                }
                return "0.00";
            }
            color: "#ffffff"
            font.pixelSize: 13
            font.family: "Courier New"
        }
    }

    // Quick Line Presets
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 8

        Text {
            text: "Quick Presets"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        Row {
            spacing: 8

            Button {
                id: horizontalBtn
                text: "Horizontal"
                implicitWidth: 80
                implicitHeight: 28

                background: Rectangle {
                    color: horizontalBtn.down ? "#4a9eff" : (horizontalBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: horizontalBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.line) {
                        var center = Qt.point((root.mprop.line.lineStart.x + root.mprop.line.lineEnd.x) / 2, (root.mprop.line.lineStart.y + root.mprop.line.lineEnd.y) / 2);
                        root.mprop.line.lineStart = Qt.point(center.x - 50, center.y);
                        root.mprop.line.lineEnd = Qt.point(center.x + 50, center.y);
                    }
                }
            }

            Button {
                id: verticalBtn
                text: "Vertical"
                implicitWidth: 80
                implicitHeight: 28

                background: Rectangle {
                    color: verticalBtn.down ? "#4a9eff" : (verticalBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: verticalBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.line) {
                        var center = Qt.point((root.mprop.line.lineStart.x + root.mprop.line.lineEnd.x) / 2, (root.mprop.line.lineStart.y + root.mprop.line.lineEnd.y) / 2);
                        root.mprop.line.lineStart = Qt.point(center.x, center.y - 50);
                        root.mprop.line.lineEnd = Qt.point(center.x, center.y + 50);
                    }
                }
            }

            Button {
                id: diagonalBtn
                text: "Diagonal"
                implicitWidth: 80
                implicitHeight: 28

                background: Rectangle {
                    color: diagonalBtn.down ? "#4a9eff" : (diagonalBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: diagonalBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.line) {
                        var center = Qt.point((root.mprop.line.lineStart.x + root.mprop.line.lineEnd.x) / 2, (root.mprop.line.lineStart.y + root.mprop.line.lineEnd.y) / 2);
                        root.mprop.line.lineStart = Qt.point(center.x - 35, center.y - 35);
                        root.mprop.line.lineEnd = Qt.point(center.x + 35, center.y + 35);
                    }
                }
            }
        }
    }
}
