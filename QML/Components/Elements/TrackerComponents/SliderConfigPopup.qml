import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import "../Input"

Popup {
    id: popup

    property string trackerName: ""
    property string trackerType: "val" // "val" or "pval"
    property real currentValue: 0.0
    property point currentPoint: Qt.point(0, 0)

    property real minValue: -10.0
    property real maxValue: 10.0
    property point minPoint: Qt.point(-10, -10)
    property point maxPoint: Qt.point(10, 10)

    signal sliderRequested(string name, string type, real minVal, real maxVal, real currentVal, point currentPt, point minPt, point maxPt)

    width: 180
    height: trackerType === "pval" ? 300 : 200
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#2c2c2c"
        border.color: "#5ce1e6"
        border.width: 2
        radius: 8
    }

    contentItem: ColumnLayout {
        spacing: 12

        // Header
        Text {
            text: `Create Slider for "${trackerName}"`
            color: "#5ce1e6"
            font.pixelSize: 16
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: trackerType === "pval" ? "Point Tracker (x, y)" : "Value Tracker"
            color: "#ccc"
            font.pixelSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#444"
        }

        // Configuration for Value Tracker
        ColumnLayout {
            visible: trackerType === "val"
            spacing: 8

            Text {
                text: `Current Value: ${currentValue.toFixed(3)}`
                color: "#44ff44"
                font.pixelSize: 12
            }

            NumberInput{
                value:minValue
                label:"Min:"
                func : ()=>{
                          minValue = newValue
                      }
            }

            NumberInput{
                value:maxValue
                label:"Max:"
                func : ()=>{
                          maxValue = newValue
                      }
            }
        }

        // Configuration for Point Tracker
        ColumnLayout {
            visible: trackerType === "pval"
            spacing: 8

            Text {
                text: `Current Point: (${currentPoint.x.toFixed(2)}, ${currentPoint.y.toFixed(2)})`
                color: "#44ff44"
                font.pixelSize: 12
            }

            PointInput{
                label:"X Range:"
                pt:minPoint
                func : (newPoint)=>{
                          minPoint = pt2
                      }
            }

            PointInput{
                label:"Y Range:"
                pt:maxPoint
                func : (newPoint)=>{
                          maxPoint = pt2
                      }
            }

        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#444"
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "Cancel"
                Layout.fillWidth: true
                onClicked: popup.close()

                background: Rectangle {
                    color: "#666"
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: "#fff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "Create Slider"
                Layout.fillWidth: true
                onClicked: {
                    if (trackerType === "val") {
                        sliderRequested(trackerName, trackerType, minValue, maxValue, currentValue, Qt.point(0, 0), Qt.point(0, 0), Qt.point(0, 0));
                    } else {
                        sliderRequested(trackerName, trackerType, 0, 0, 0, currentPoint, minPoint, maxPoint);
                    }
                    popup.close();
                }

                background: Rectangle {
                    color: "#4a9eff"
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: "#fff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
