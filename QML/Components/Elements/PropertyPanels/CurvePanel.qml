import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Curve Properties Panel - Mathematical curve functions and parameters
ColumnLayout {
    id: root

    property var mprop: null
    spacing: 12

    Text {
        text: "Curve Properties"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    // Curve Functions
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 8

        Text {
            text: "Mathematical Functions"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        // X Function
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Text {
                text: "X(t) ="
                color: "#aaaaaa"
                font.pixelSize: 12
                font.family: "Courier New"
            }

            StyledTextField {
                Layout.fillWidth: true
                placeholderText: "Enter X function (e.g., 3*cos(t))"
                text: root.mprop && root.mprop.curve ? root.mprop.curve.curveXFunc : ""
                onEditingFinished: {
                    if (root.mprop && root.mprop.curve)
                        root.mprop.curve.curveXFunc = text;
                }
            }
        }

        // Y Function
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Text {
                text: "Y(t) ="
                color: "#aaaaaa"
                font.pixelSize: 12
                font.family: "Courier New"
            }

            StyledTextField {
                Layout.fillWidth: true
                placeholderText: "Enter Y function (e.g., 3*sin(t))"
                text: root.mprop && root.mprop.curve ? root.mprop.curve.curveYFunc : ""
                onEditingFinished: {
                    if (root.mprop && root.mprop.curve)
                        root.mprop.curve.curveYFunc = text;
                }
            }
        }
    }

    // Parameter Settings
    GridLayout {
        Layout.fillWidth: true
        columns: 2
        rowSpacing: 12
        columnSpacing: 16

        // Parameter Range
        Text {
            text: "Parameter Range"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        PointInput {
            Layout.fillWidth: true
            label: "t range:"
            pt: root.mprop && root.mprop.curve ? root.mprop.curve.tRange : Qt.point(0, 1)
            func: function () {
                if (root.mprop && root.mprop.curve)
                    root.mprop.curve.tRange = pt2;
            }
        }

        // Segments
        Text {
            text: "Segments"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.curve ? root.mprop.curve.segments : 30
            integersOnly: true
            func: () => {
                if (root.mprop && root.mprop.curve)
                    root.mprop.curve.segments = newValue;
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
            value: root.mprop && root.mprop.curve ? root.mprop.curve.thickness : 2
            decimals: 1
            func: () => {
                if (root.mprop && root.mprop.curve)
                    root.mprop.curve.thickness = newValue;
            }
        }
    }

    // Function Presets
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 8

        Text {
            text: "Function Presets"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        Flow {
            Layout.fillWidth: true
            spacing: 8

            Button {
                id: circleBtn
                text: "Circle"
                implicitWidth: 65
                implicitHeight: 28

                background: Rectangle {
                    color: circleBtn.down ? "#4a9eff" : (circleBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: circleBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.curve) {
                        root.mprop.curve.curveXFunc = "50*cos(t)";
                        root.mprop.curve.curveYFunc = "50*sin(t)";
                        root.mprop.curve.tRange = Qt.point(0, 6.28318);
                    }
                }
            }

            Button {
                id: ellipseBtn
                text: "Ellipse"
                implicitWidth: 65
                implicitHeight: 28

                background: Rectangle {
                    color: ellipseBtn.down ? "#4a9eff" : (ellipseBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: ellipseBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.curve) {
                        root.mprop.curve.curveXFunc = "80*cos(t)";
                        root.mprop.curve.curveYFunc = "40*sin(t)";
                        root.mprop.curve.tRange = Qt.point(0, 6.28318);
                    }
                }
            }

            Button {
                id: sineBtn
                text: "Sine Wave"
                implicitWidth: 75
                implicitHeight: 28

                background: Rectangle {
                    color: sineBtn.down ? "#4a9eff" : (sineBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: sineBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.curve) {
                        root.mprop.curve.curveXFunc = "t*20";
                        root.mprop.curve.curveYFunc = "30*sin(t)";
                        root.mprop.curve.tRange = Qt.point(-3.14159, 3.14159);
                    }
                }
            }

            Button {
                id: spiralBtn
                text: "Spiral"
                implicitWidth: 65
                implicitHeight: 28

                background: Rectangle {
                    color: spiralBtn.down ? "#4a9eff" : (spiralBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: spiralBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.curve) {
                        root.mprop.curve.curveXFunc = "t*cos(t)*5";
                        root.mprop.curve.curveYFunc = "t*sin(t)*5";
                        root.mprop.curve.tRange = Qt.point(0, 12.56636);
                    }
                }
            }

            Button {
                id: lissajousBtn
                text: "Lissajous"
                implicitWidth: 75
                implicitHeight: 28

                background: Rectangle {
                    color: lissajousBtn.down ? "#4a9eff" : (lissajousBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: lissajousBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.curve) {
                        root.mprop.curve.curveXFunc = "50*sin(3*t)";
                        root.mprop.curve.curveYFunc = "50*sin(2*t)";
                        root.mprop.curve.tRange = Qt.point(0, 6.28318);
                    }
                }
            }

            Button {
                id: heartBtn
                text: "Heart"
                implicitWidth: 65
                implicitHeight: 28

                background: Rectangle {
                    color: heartBtn.down ? "#4a9eff" : (heartBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: heartBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.curve) {
                        root.mprop.curve.curveXFunc = "16*sin(t)^3";
                        root.mprop.curve.curveYFunc = "13*cos(t)-5*cos(2*t)-2*cos(3*t)-cos(4*t)";
                        root.mprop.curve.tRange = Qt.point(0, 6.28318);
                    }
                }
            }
        }
    }

    // Parameter Info
    Rectangle {
        Layout.fillWidth: true
        implicitHeight: 60
        color: "#1a1a1a"
        border.color: "#333333"
        border.width: 1
        radius: 4

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 4

            Text {
                text: "💡 Function Tips"
                color: "#888888"
                font.pixelSize: 12
                font.bold: true
            }

            Text {
                text: "• Use 't' as the parameter variable\n• Available functions: sin, cos, tan, exp, log, sqrt, abs\n• Use parentheses for grouping: sin(2*t), cos(t+1)\n• Constants: PI = 3.14159, E = 2.71828"
                color: "#888888"
                font.pixelSize: 10
                lineHeight: 1.2
                wrapMode: Text.WordWrap
            }
        }
    }
}
