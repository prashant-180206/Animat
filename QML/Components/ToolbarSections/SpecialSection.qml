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
    property Scene scene: null
    visible: mprop !== null
    spacing: 10

    // Curve function inputs grouped
    RowLayout {
        visible: root.mprop && root.mprop.curve
        spacing: 2

        RowLayout {
            Layout.fillHeight: true
            spacing: 2

            Text {
                text: "X(t) ="
                color: "#aaaaaa"
                font.pixelSize: 12
                font.family: "Courier New"
            }

            StyledTextField {
                Layout.fillHeight: true
                placeholderText: "Enter X function (e.g., 3*cos(t))"
                text: root.mprop && root.mprop.curve ? root.mprop.curve.curveXFunc : ""
                onEditingFinished: {
                    if (root.mprop && root.mprop.curve)
                        root.mprop.curve.curveXFunc = text;
                }
            }
        }

        RowLayout {
            Layout.fillHeight: true
            spacing: 4

            Text {
                text: "Y(t) ="
                color: "#aaaaaa"
                font.pixelSize: 12
                font.family: "Courier New"
            }

            StyledTextField {
                Layout.fillHeight: true
                placeholderText: "Enter Y function (e.g., 3*sin(t))"
                text: root.mprop && root.mprop.curve ? root.mprop.curve.curveYFunc : ""
                onEditingFinished: {
                    if (root.mprop && root.mprop.curve)
                        root.mprop.curve.curveYFunc = text;
                }
            }
        }
        PointInput {
            Layout.fillWidth: true
            label: "T range:"
            pt: root.mprop && root.mprop.curve ? root.mprop.curve.tRange : Qt.point(0, 1)
            func: function () {
                if (root.mprop && root.mprop.curve)
                    root.mprop.curve.tRange = pt2;
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
            visible: root.mprop && root.mprop.base && root.mprop.base.type !== "Line" && root.mprop.base.type !== "Curve" && root.mprop.base.type !== "Circle"
            func: function () {
                if (root.mprop && root.mprop.base)
                    root.mprop.base.size = pt2;
            }
        }
    }

    // Line Input Parameters
    RowLayout {
        visible: root.mprop && root.mprop.line && root.mprop.base.type === "Line"
        spacing: 2

        PointInput {
            Layout.fillWidth: true
            label: "Start:"
            pt: root.mprop && root.mprop.line ? root.mprop.line.lineStart : Qt.point(0, 0)
            func: function () {
                if (root.mprop && root.mprop.line)
                    root.mprop.line.lineStart = pt2;
            }
        }
        PointInput {
            Layout.fillWidth: true
            label: "End:"
            pt: root.mprop && root.mprop.line ? root.mprop.line.lineEnd : Qt.point(0, 0)
            func: function () {
                if (root.mprop && root.mprop.line)
                    root.mprop.line.lineEnd = pt2;
            }
        }
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
    }
    // Plane Input Parameters
    RowLayout {
        spacing: 10
        visible: root.mprop && root.mprop.geometric && root.mprop.base.type === "Plane"
        NumberInput {
            Layout.fillHeight: true
            label: "Width:"
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.width : 10.0
            func: () => {
                      root.mprop.geometric.plane.width = newValue;
                  }
        }
        NumberInput {
            Layout.fillHeight: true
            label: "Height:"
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.height : 10.0
            func: () => {
                      root.mprop.geometric.plane.height = newValue;
                  }
        }
        NumberInput {
            Layout.fillHeight: true
            label: "Step:"
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.step : 1.0
            func: () => {
                      root.mprop.geometric.plane.step = newValue;
                  }
        }
        NumberInput {
            label:"Grid Thickness"
            Layout.fillWidth: true
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.gridThickness : 1.0
            decimals: 1
            func: () => {
                      root.mprop.geometric.plane.gridThickness = newValue;
                  }
        }
        DarkCheckBox {
            Layout.fillHeight: true
            label: "Show Axes"
            checked: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.showAxes : true
            onChanged:  (v) => {
                            if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane)
                            root.mprop.geometric.plane.showAxes = v
                        }
        }

        DarkCheckBox {
            Layout.fillHeight: true
            label: "Show Grid"
            checked: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.showGrid : true
            onChanged:  (v) => {
                            if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane)
                            root.mprop.geometric.plane.showGrid = v
                        }
        }

        ColorPicker {

            selectedColor: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.lineColor : "#444444"
            func: () => {
                      root.mprop.geometric.plane.lineColor = newColor;
                  }
        }
        ColorPicker {

            selectedColor: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.axisColor : "#ffffff"
            func: () => {
                      root.mprop.geometric.plane.axisColor = newColor;
                  }
        }
    }
    PolygonToolBar {
        visible: mprop && mprop.polygon && !mprop.circle && ! mprop.base.name==="Rectangle"
        Layout.fillWidth: true
        canvas:scene
        mprop: root.mprop
    }
}
