import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import Animat 1.0

Rectangle {
    id: root

    property string trackerName: ""
    property string trackerType: "val" // "val" or "pval"
    property real minValue: 0.0
    property real maxValue: 5
    property real currentValue: 0.0
    property point currentPoint: Qt.point(0, 0)
    property point minPoint: Qt.point(0, 0)  // minY in x, maxY in y for pval
    property point maxPoint: Qt.point(5, 5)

    property Parser parser: null

    // Remove explicit width to avoid conflicts
    height: trackerType === "pval" ? 120 : 80
    color: "#2c2c2c"
    border.color: "#444"
    border.width: 1
    radius: 6

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        // Header
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: trackerType === "pval" ? "📍" : "📊"
                font.pixelSize: 16
                Layout.preferredWidth: 20
            }

            Text {
                text: trackerName
                color: "#5ce1e6"
                font.pixelSize: 12
                font.bold: true
                Layout.preferredWidth: 100
            }

            Text {
                text: trackerType === "pval" ? `(${currentPoint.x.toFixed(2)}, ${currentPoint.y.toFixed(2)})` : currentValue.toFixed(3)
                color: "#44ff44"
                font.pixelSize: 11
                font.family: "Consolas, Monaco, monospace"
                Layout.fillWidth: true
            }

            Button {
                text: "🗑️"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 25

                background: Rectangle {
                    color: "#d4651a"
                    border.color: "#f47c3c"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: "#fff"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: removeSlider()
            }
        }

        // Value slider (for val type)
        RowLayout {
            visible: trackerType === "val"
            Layout.fillWidth: true

            Text {
                text: minValue.toFixed(1)
                color: "#888"
                font.pixelSize: 10
                font.family: "Consolas, Monaco, monospace"
                Layout.preferredWidth: 40
            }

            Slider {
                id: valueSlider
                from: minValue
                to: maxValue
                value: currentValue
                Layout.fillWidth: true

                background: Rectangle {
                    x: valueSlider.leftPadding
                    y: valueSlider.topPadding + valueSlider.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 4
                    width: valueSlider.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#444"

                    Rectangle {
                        width: valueSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#4a9eff"
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: valueSlider.leftPadding + valueSlider.visualPosition * (valueSlider.availableWidth - width)
                    y: valueSlider.topPadding + valueSlider.availableHeight / 2 - height / 2
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 8
                    color: valueSlider.pressed ? "#6bb6ff" : "#4a9eff"
                    border.color: "#fff"
                    border.width: 1
                }

                onValueChanged: {
                    if (Math.abs(value - currentValue) > 0.001) {
                        currentValue = value;
                        updateTracker();
                    }
                }
            }

            Text {
                text: maxValue.toFixed(1)
                color: "#888"
                font.pixelSize: 10
                font.family: "Consolas, Monaco, monospace"
                Layout.preferredWidth: 40
                horizontalAlignment: Text.AlignRight
            }
        }

        // Point sliders (for pval type)
        ColumnLayout {
            visible: trackerType === "pval"
            Layout.fillWidth: true
            spacing: 4

            // X coordinate slider
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "X:"
                    color: "#ccc"
                    font.pixelSize: 10
                    Layout.preferredWidth: 15
                }

                Text {
                    text: minPoint.x.toFixed(1)
                    color: "#888"
                    font.pixelSize: 10
                    font.family: "Consolas, Monaco, monospace"
                    Layout.preferredWidth: 35
                }

                MSlider {
                    id: xSlider
                    Layout.fillWidth: true
                    minValue:  minPoint.x
                    maxValue:  maxPoint.x
                    currentValue:  currentPoint.x
                    onValueUpdated: (val)=> {
                        if (Math.abs(val - currentPoint.x) > 0.001) {
                            currentPoint = Qt.point(val, currentPoint.y);
                            updatePtTracker();
                        }
                    }
                }

                Text {
                    text: maxPoint.x.toFixed(1)
                    color: "#888"
                    font.pixelSize: 10
                    font.family: "Consolas, Monaco, monospace"
                    Layout.preferredWidth: 35
                    horizontalAlignment: Text.AlignRight
                }
            }

            // Y coordinate slider
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Y:"
                    color: "#ccc"
                    font.pixelSize: 10
                    Layout.preferredWidth: 15
                }

                Text {
                    text: minPoint.y.toFixed(1)
                    color: "#888"
                    font.pixelSize: 10
                    font.family: "Consolas, Monaco, monospace"
                    Layout.preferredWidth: 35
                }

                Slider {
                    id: ySlider
                    from: minPoint.y
                    to: maxPoint.y
                    value: currentPoint.y
                    Layout.fillWidth: true

                    background: Rectangle {
                        x: ySlider.leftPadding
                        y: ySlider.topPadding + ySlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 3
                        width: ySlider.availableWidth
                        height: implicitHeight
                        radius: 1.5
                        color: "#444"

                        Rectangle {
                            width: ySlider.visualPosition * parent.width
                            height: parent.height
                            color: "#4ecdc4"
                            radius: 1.5
                        }
                    }

                    handle: Rectangle {
                        x: ySlider.leftPadding + ySlider.visualPosition * (ySlider.availableWidth - width)
                        y: ySlider.topPadding + ySlider.availableHeight / 2 - height / 2
                        implicitWidth: 12
                        implicitHeight: 12
                        radius: 6
                        color: ySlider.pressed ? "#6ee7dd" : "#4ecdc4"
                        border.color: "#fff"
                        border.width: 1
                    }

                    onValueChanged: {
                        if (Math.abs(value - currentPoint.y) > 0.001) {
                            currentPoint = Qt.point(currentPoint.x, value);
                            updatePtTracker();
                        }
                    }
                }

                Text {
                    text: maxPoint.y.toFixed(1)
                    color: "#888"
                    font.pixelSize: 10
                    font.family: "Consolas, Monaco, monospace"
                    Layout.preferredWidth: 35
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }

    // Update tracker value via parser
    function updateTracker() {
        if (!parser)
            return;

        parser.setTrackerValue(trackerName, currentValue);
    }
    function updatePtTracker() {
        if (!parser)
            return;

        parser.setPointTrackerValue(trackerName, currentPoint);
    }

    // Remove this slider (signal to parent)
    signal sliderRemoved(string name, string type)

    function removeSlider() {
        sliderRemoved(trackerName, trackerType);
    }
}
