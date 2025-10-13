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

    // For "val" tracker
    property real minValue: -10.0
    property real maxValue: 10.0

    // Independent properties for point range
    property real minX: -5.0
    property real maxX: 5.0
    property real minY: -5.0
    property real maxY: 5.0

    signal sliderRequested(
        string name, string type, real minVal, real maxVal, real currentVal,
        point currentPt, point minPt, point maxPt
    )

    width: 180
    height: trackerType === "pval" ? 300 : 200
    x: (parent ? (parent.width - width) / 2 : 0)
    y: (parent ? (parent.height - height) / 2 : 0)

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

            NumberInput {
                value: minValue
                label: "Min:"
                func: () => {
                    minValue = newValue
                }
            }

            NumberInput {
                value: maxValue
                label: "Max:"
                func: () => {
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

            // X Range Input
            PointInput {
                label: "X Range:"
                pt: Qt.point(minX, maxX)
                func: () => {
                    minX = pt2.x
                    maxX = pt2.y
                }
            }

            // Y Range Input
            PointInput {
                label: "Y Range:"
                pt: Qt.point(minY, maxY)
                func: () => {
                    minY = pt2.x
                    maxY = pt2.y
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
                        sliderRequested(
                            trackerName, trackerType,
                            minValue, maxValue, currentValue,
                            Qt.point(0, 0), Qt.point(0, 0), Qt.point(0, 0)
                        )
                    } else {
                        sliderRequested(
                            trackerName, trackerType,
                            0, 0, 0,
                            currentPoint,
                            Qt.point(minX, minY),
                            Qt.point(maxX, maxY)
                        )
                    }
                    popup.close()
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
