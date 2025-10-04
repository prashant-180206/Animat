import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"
import Animat 1.0

// Plane Properties Panel - Grid, axis and label controls
ColumnLayout {
    id: root

    property MProperties mprop
    spacing: 12

    Text {
        text: "Plane Properties"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    // Dimensions Section
    Text {
        text: "Dimensions"
        color: "#bbbbbb"
        font.pixelSize: 13
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Text {
            text: "Width:"
            color: "#bbbbbb"
            font.pixelSize: 12
        }
        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.width : 10.0
            func: ()=>{
                      mprop.geometric.plane.width = newValue;
                  }
        }

        Text {
            text: "Height:"
            color: "#bbbbbb"
            font.pixelSize: 12
        }
        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.height : 10.0
            func: ()=>{
                      mprop.geometric.plane.height = newValue;
                  }
        }
    }

    // Step Control
    Text {
        text: "Grid Step"
        color: "#bbbbbb"
        font.pixelSize: 13
    }

    NumberInput {
        Layout.fillWidth: true
        value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.step : 1.0
        func: ()=>{
                  mprop.geometric.plane.step = newValue;
              }
    }

    // Grid Settings
    Text {
        text: "Grid Settings"
        color: "#bbbbbb"
        font.pixelSize: 13
    }

    CheckBox {
        text: "Show Grid"
        checked: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.showGrid : true
        onCheckedChanged: {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane) {
                root.mprop.geometric.plane.showGrid = checked;
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Text {
            text: "Grid Color:"
            color: "#bbbbbb"
            font.pixelSize: 12
        }
        ColorPicker {
            Layout.fillWidth: true
            selectedColor: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.lineColor : "#444444"
            func: ()=>{
                      mprop.geometric.plane.lineColor = newColor;
                  }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Text {
            text: "Grid Thickness:"
            color: "#bbbbbb"
            font.pixelSize: 12
        }
        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.gridThickness : 1.0
            decimals: 1
            func: ()=>{
                      mprop.geometric.plane.gridThickness = newValue;
                  }
        }
    }

    // Axis Settings
    Text {
        text: "Axis Settings"
        color: "#bbbbbb"
        font.pixelSize: 13
    }

    CheckBox {

        text: "Show Axis"
        checked: root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.showAxes : true
        onCheckedChanged: {
            if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane) {
                root.mprop.geometric.plane.showAxes = checked;
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Text {
            text: "Axis Color:"
            color: "#bbbbbb"
            font.pixelSize: 12
        }
        ColorPicker {
            Layout.fillWidth: true
            selectedColor:  root.mprop && root.mprop.geometric && root.mprop.geometric.plane ? root.mprop.geometric.plane.axisColor : "#ffffff"
            func: ()=>{
                      mprop.geometric.plane.axisColor = newColor;
                  }
        }
    }

    // Quick Presets
    Text {
        text: "Quick Presets"
        color: "#bbbbbb"
        font.pixelSize: 13
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 5

        StyledButton {
            text: "Standard"
            Layout.fillWidth: true
            onClicked: {
                if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane) {
                    root.mprop.geometric.plane.step = 1.0;
                    root.mprop.geometric.plane.lineColor = "#444444";
                    root.mprop.geometric.plane.axisColor = "#ffffff";
                    root.mprop.geometric.plane.showGrid = true;
                    root.mprop.geometric.plane.showAxes = true;
                }
            }
        }

        StyledButton {
            text: "Fine Grid"
            Layout.fillWidth: true
            onClicked: {
                if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane) {
                    root.mprop.geometric.plane.step = 0.5;
                    root.mprop.geometric.plane.lineColor = "#333333";
                    root.mprop.geometric.plane.gridThickness = 0.5;
                }
            }
        }

        StyledButton {
            text: "Axes Only"
            Layout.fillWidth: true
            onClicked: {
                if (root.mprop && root.mprop.geometric && root.mprop.geometric.plane) {
                    root.mprop.geometric.plane.showGrid = false;
                    root.mprop.geometric.plane.showAxes = true;
                    root.mprop.geometric.plane.axisThickness = 3.0;
                }
            }
        }
    }
}
