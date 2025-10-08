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

    signal sliderRequested(string name, string type, real minVal, real maxVal, real currentVal, point currentPt, point minPt, point maxPt)

    width: 350
    height: trackerType === "pval" ? 300 : 200
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    modal: true
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

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Min:"
                    color: "#ccc"
                    Layout.preferredWidth: 40
                }

                SpinBox {
                    id: minValueSpinBox
                    from: -20
                    to: 20
                    value: Math.min(-10, currentValue - 10)
                    stepSize: 1
                    editable: true
                    Layout.fillWidth: true

                    property real realValue: value / 100

                    validator: DoubleValidator {
                        bottom: -1000
                        top: 1000
                        decimals: 2
                    }

                    textFromValue: function (value, locale) {
                        return (value / 100).toFixed(2);
                    }

                    valueFromText: function (text, locale) {
                        return parseFloat(text) * 100;
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Max:"
                    color: "#ccc"
                    Layout.preferredWidth: 40
                }

                // NumberInput{
                //     id: maxValueSpinBox
                //     value: -5
                //     property real realValue: value
                //     func: ()=>{
                //               realValue = newValue
                //           }
                // }

                SpinBox {
                    id: maxValueSpinBox
                    from: -10000
                    to: 10000
                    value: Math.max(1000, currentValue + 10) * 100
                    stepSize: 1
                    editable: true
                    Layout.fillWidth: true

                    property real realValue: value / 100

                    validator: DoubleValidator {
                        bottom: -1000
                        top: 1000
                        decimals: 2
                    }

                    textFromValue: function (value, locale) {
                        return (value / 100).toFixed(2);
                    }

                    valueFromText: function (text, locale) {
                        return parseFloat(text) * 100;
                    }
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

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "X Range:"
                    color: "#ccc"
                    Layout.preferredWidth: 60
                }

                SpinBox {
                    id: minXSpinBox
                    from: -10000
                    to: 10000
                    value: Math.min(-500, (currentPoint.x - 5) * 100)
                    stepSize: 10
                    editable: true
                    Layout.fillWidth: true

                    textFromValue: function (value, locale) {
                        return (value / 100).toFixed(2);
                    }

                    valueFromText: function (text, locale) {
                        return parseFloat(text) * 100;
                    }
                }

                Text {
                    text: "to"
                    color: "#ccc"
                }

                SpinBox {
                    id: maxXSpinBox
                    from: -10000
                    to: 10000
                    value: Math.max(500, (currentPoint.x + 5) * 100)
                    stepSize: 10
                    editable: true
                    Layout.fillWidth: true

                    textFromValue: function (value, locale) {
                        return (value / 100).toFixed(2);
                    }

                    valueFromText: function (text, locale) {
                        return parseFloat(text) * 100;
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Y Range:"
                    color: "#ccc"
                    Layout.preferredWidth: 60
                }

                SpinBox {
                    id: minYSpinBox
                    from: -10000
                    to: 10000
                    value: Math.min(-500, (currentPoint.y - 5) * 100)
                    stepSize: 10
                    editable: true
                    Layout.fillWidth: true

                    textFromValue: function (value, locale) {
                        return (value / 100).toFixed(2);
                    }

                    valueFromText: function (text, locale) {
                        return parseFloat(text) * 100;
                    }
                }

                Text {
                    text: "to"
                    color: "#ccc"
                }

                SpinBox {
                    id: maxYSpinBox
                    from: -10000
                    to: 10000
                    value: Math.max(500, (currentPoint.y + 5) * 100)
                    stepSize: 10
                    editable: true
                    Layout.fillWidth: true

                    textFromValue: function (value, locale) {
                        return (value / 100).toFixed(2);
                    }

                    valueFromText: function (text, locale) {
                        return parseFloat(text) * 100;
                    }
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
                        sliderRequested(trackerName, trackerType, minValueSpinBox.realValue, maxValueSpinBox.realValue, currentValue, Qt.point(0, 0), Qt.point(0, 0), Qt.point(0, 0));
                    } else {
                        // Convert spinbox values back to real numbers and create proper point ranges
                        let minX = minXSpinBox.value / 100;
                        let maxX = maxXSpinBox.value / 100;
                        let minY = minYSpinBox.value / 100;
                        let maxY = maxYSpinBox.value / 100;
                        sliderRequested(trackerName, trackerType, 0, 0, 0, currentPoint, Qt.point(minX, minY), Qt.point(maxX, maxY));
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
