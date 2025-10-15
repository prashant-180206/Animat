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
    property var trackerManager: null

    // For "val" tracker
    property real minValue: -10.0
    property real maxValue: 10.0

    // Independent properties for point range
    property real minX: -5.0
    property real maxX: 5.0
    property real minY: -5.0
    property real maxY: 5.0

    // Appeartime parameter
    property real appearTime: 1.0

    // Load existing values when popup opens
    onAboutToShow: {
        loadExistingValues();
    }

    function loadExistingValues() {
        if (!trackerManager || !trackerName) return;
        
        if (trackerType === "val" && trackerManager.hasSliderConfiguration(trackerName)) {
            minValue = trackerManager.getTrackerMin(trackerName);
            maxValue = trackerManager.getTrackerMax(trackerName);
            console.log(`Loaded existing values for ${trackerName}: min=${minValue}, max=${maxValue}`);
        } else if (trackerType === "pval" && trackerManager.hasPointSliderConfiguration(trackerName)) {
            let minPt = trackerManager.getPointTrackerMin(trackerName);
            let maxPt = trackerManager.getPointTrackerMax(trackerName);
            minX = minPt.x;
            minY = minPt.y;
            maxX = maxPt.x;
            maxY = maxPt.y;
            console.log(`Loaded existing point values for ${trackerName}: min=(${minX},${minY}), max=(${maxX},${maxY})`);
        }
    }

    width: 180
    height: trackerType === "pval" ? 350 : 250
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

        // Status indicator for existing configuration
        Text {
            visible: {
                if (!trackerManager || !trackerName) return false;
                if (trackerType === "val") {
                    return trackerManager.hasSliderConfiguration(trackerName);
                } else {
                    return trackerManager.hasPointSliderConfiguration(trackerName);
                }
            }
            text: "🔧 Editing existing slider configuration"
            color: "#ffaa44"
            font.pixelSize: 10
            font.italic: true
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

        // Appear Time Configuration (common for both types)
        ColumnLayout {
            spacing: 8

            Text {
                text: "Slider Configuration:"
                color: "#ccc"
                font.pixelSize: 12
                font.bold: true
            }

            NumberInput {
                value: appearTime
                label: "Appear Time:"
                func: () => {
                          appearTime = newValue
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
                    if (!trackerManager) {
                        console.log("TrackerManager not available");
                        popup.close();
                        return;
                    }

                    console.log(`Creating slider for ${trackerName} (${trackerType}) with appearTime=${appearTime}`);
                    
                    if (trackerType === "val") {
                        console.log(`Value tracker: min=${minValue}, max=${maxValue}`);
                        trackerManager.addSlider(trackerName, maxValue, minValue, appearTime*1000);
                    } else if (trackerType === "pval") {
                        console.log(`Point tracker: min=(${minX},${minY}), max=(${maxX},${maxY})`);
                        trackerManager.addSlider(trackerName, Qt.point(maxX, maxY), Qt.point(minX, minY), appearTime*1000);
                    }
                    
                    console.log(`Slider created for ${trackerName} (${trackerType})`);
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
