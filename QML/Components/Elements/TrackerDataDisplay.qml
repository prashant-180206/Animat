import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import Animat 1.0
import "Input"
import "TrackerComponents"

Rectangle {
    id: root

    property Scene scene: null
    property var parser: scene ? scene.getParser() : null

    color: "#1f1f1f"
    radius: 4

    // Slider management
    property var activeSliders: ({})

    ListModel {
        id: sliderModel
    }

    ListModel {
        id: trackerModel
    }

    ListModel {
        id: pointTrackerModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Header
        Text {
            text: "📊 Value Tracker Data"
            color: "#5ce1e6"
            font.pixelSize: 16
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Main content - split into left (data) and right (sliders)
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            spacing: 10

            // Left side - Tracker Data Display
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: root.height * 0.5
                Layout.minimumHeight: 300
                color: "transparent"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    // Header with refresh button
                    TrackerDataHeader {
                        trackerCount: trackerModel.count
                        pointTrackerCount: pointTrackerModel.count
                        onRefreshRequested: refreshData()
                    }

                    // Tabs panel for trackers
                    TrackerTabsPanel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        valueTrackersModel: trackerModel
                        pointTrackersModel: pointTrackerModel

                        onValueTrackerClicked: (name, value) => {
                            sliderConfigPopup.trackerName = name;
                            sliderConfigPopup.trackerType = "val";
                            sliderConfigPopup.currentValue = value;
                            sliderConfigPopup.open();
                        }

                        onPointTrackerClicked: (name, xval, yval) => {
                            sliderConfigPopup.trackerName = name;
                            sliderConfigPopup.trackerType = "pval";
                            sliderConfigPopup.currentPoint = Qt.point(xval, yval);
                            sliderConfigPopup.open();
                        }

                        onValueTrackerDeleteRequested: name => {
                            deleteTracker(name);
                        }

                        onPointTrackerDeleteRequested: name => {
                            deletePointTracker(name);
                        }
                    }
                }
            }

            // Right side - Active Sliders
            ActiveSlidersPanel {
                Layout.fillWidth: true
                Layout.preferredHeight: root.height * 0.3
                Layout.minimumHeight: 250
                sliderModel: sliderModel
                parser: root.parser
                onSliderRemoved: (name, type) => {
                    removeSlider(name, type);
                }
            }
        }
    }

    // Popup for slider configuration
    SliderConfigPopup {
        id: sliderConfigPopup

        onSliderRequested: (name, type, minVal, maxVal, currentVal, currentPt, minPt, maxPt) => {
            createSlider(name, type, minVal, maxVal, currentVal, currentPt, minPt, maxPt);
        }
    }

    // JavaScript functions
    function refreshData() {
        if (!root.parser) {
            console.log("Parser not available");
            return;
        }

        // Clear existing models
        trackerModel.clear();
        pointTrackerModel.clear();

        // Get value trackers
        let trackerNames = root.parser.getTrackerNames();
        for (let i = 0; i < trackerNames.length; i++) {
            let name = trackerNames[i];
            let value = root.parser.getTrackerValue(name);
            trackerModel.append({
                name: name,
                value: value
            });
        }

        // Get point trackers
        let pointTrackerNames = root.parser.getPointTrackerNames();
        for (let i = 0; i < pointTrackerNames.length; i++) {
            let name = pointTrackerNames[i];
            let point = root.parser.getPointTrackerValue(name);
            pointTrackerModel.append({
                name: name,
                xval: point.x,
                yval: point.y
            });
        }

        console.log(`Loaded ${trackerNames.length} value trackers and ${pointTrackerNames.length} point trackers`);
    }

    function createSlider(name, type, minVal, maxVal, currentVal, currentPt, minPt, maxPt) {
        console.log(`createSlider called: ${name}, ${type}, minVal: ${minVal}, maxVal: ${maxVal}`);

        // Check if slider already exists
        let key = name + "_" + type;
        if (root.activeSliders[key]) {
            console.log(`Slider for ${name} (${type}) already exists`);
            return;
        }

        // Get current values from parser
        let actualCurrentVal = currentVal;
        let actualCurrentPt = currentPt || Qt.point(0, 0);
        let actualMinPt = minPt || Qt.point(0, 0);
        let actualMaxPt = maxPt || Qt.point(0, 0);

        if (type === "val") {
            actualCurrentVal = root.parser.getTrackerValue(name);
        } else {
            actualCurrentPt = root.parser.getPointTrackerValue(name);
        }

        // Add to slider model
        let sliderData = {
            name: name,
            type: type,
            minVal: minVal,
            maxVal: maxVal,
            currentVal: actualCurrentVal,
            currentPt: actualCurrentPt,
            minPt: type === "pval" ? actualMinPt : Qt.point(0, 0),
            maxPt: type === "pval" ? actualMaxPt : Qt.point(0, 0)
        };

        sliderModel.append(sliderData);
        root.activeSliders[key] = sliderData;

        console.log(`Created slider for ${name} (${type}) with ranges: minPt=${actualMinPt.x},${actualMinPt.y} maxPt=${actualMaxPt.x},${actualMaxPt.y}`);
        console.log(`SliderModel count is now: ${sliderModel.count}`);
    }

    function removeSlider(name, type) {
        let key = name + "_" + type;
        delete root.activeSliders[key];

        // Remove from model
        for (let i = 0; i < sliderModel.count; i++) {
            let item = sliderModel.get(i);
            if (item.name === name && item.type === type) {
                sliderModel.remove(i);
                break;
            }
        }

        console.log(`Removed slider for ${name} (${type})`);
    }

    function deleteTracker(name) {
        if (!root.parser)
            return;

        // Use direct delete method instead of command
        let success = root.parser.deleteTracker(name);
        if (success) {
            console.log(`Deleted value tracker: ${name}`);
            refreshData();
            removeSlider(name, "val"); // Remove associated slider
        } else {
            console.log(`Failed to delete value tracker: ${name}`);
        }
    }

    function deletePointTracker(name) {
        if (!root.parser)
            return;

        // Use direct delete method instead of command
        let success = root.parser.deletePointTracker(name);
        if (success) {
            console.log(`Deleted point tracker: ${name}`);
            refreshData();
            removeSlider(name, "pval"); // Remove associated slider
        } else {
            console.log(`Failed to delete point tracker: ${name}`);
        }
    }

    // Component initialization
    Component.onCompleted: {
        refreshData();
    }

    // Auto-refresh when scene changes
    onSceneChanged: {
        refreshData();
    }
}
