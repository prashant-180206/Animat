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
    property var trackerManager: parser ? parser.trackerManager() : null

    color: "#1f1f1f"
    radius: 4

    // Monitor active tracker changes
    Connections {
        target: root.trackerManager
        function onActiveTrackersChanged() {
            if (root.trackerManager) {
                console.log(`Active Value Trackers changed: ${root.trackerManager.activeTrackers.length} trackers`);
                for (let i = 0; i < root.trackerManager.activeTrackers.length; i++) {
                    let tracker = root.trackerManager.activeTrackers[i];
                    console.log(`  - Value Tracker ${i}: min=${tracker.min}, max=${tracker.max}, appearTime=${tracker.appeartime}`);
                }
            }
        }
        function onActivePtTrackersChanged() {
            if (root.trackerManager) {
                console.log(`Active Point Trackers changed: ${root.trackerManager.activePtTrackers.length} trackers`);
                for (let i = 0; i < root.trackerManager.activePtTrackers.length; i++) {
                    let tracker = root.trackerManager.activePtTrackers[i];
                    console.log(`  - Point Tracker ${i}: min=(${tracker.min.x},${tracker.min.y}), max=(${tracker.max.x},${tracker.max.y}), appearTime=${tracker.appeartime}`);
                }
            }
        }
        function onTrackerAdded(name) {
            console.log(`Tracker added: ${name}`);
            // Force UI update
            root.trackerManager = root.trackerManager;
        }
        function onTrackerRemoved(name) {
            console.log(`Tracker removed: ${name}`);
            // Force UI update
            root.trackerManager = root.trackerManager;
        }
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

        // Debug information
        Text {
            text: `Scene: ${root.scene ? "OK" : "NULL"} | Parser: ${root.parser ? "OK" : "NULL"} | TrackerManager: ${root.trackerManager ? "OK" : "NULL"}`
            color: "#ff6666"
            font.pixelSize: 10
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

                    // Header with count display
                    TrackerDataHeader {
                        trackerCount: {
                            if (trackerManager) {
                                let names = trackerManager.getTrackerNames();
                                return names ? names.length : 0;
                            }
                            return 0;
                        }
                        pointTrackerCount: {
                            if (trackerManager) {
                                let names = trackerManager.getPointTrackerNames();
                                return names ? names.length : 0;
                            }
                            return 0;
                        }
                    }

                    // Tabs panel for trackers
                    TrackerTabsPanel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        trackerManager: root.trackerManager

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
            SlidersPanel {
                Layout.fillWidth: true
                Layout.preferredHeight: root.height * 0.3
                Layout.minimumHeight: 250
                trackerManager: root.trackerManager
                parser: root.parser
                onSliderRemoved: (name, type) => {
                    // Sliders are now managed by TrackerManager directly
                    console.log(`Slider removed for ${name} (${type})`);
                }
            }
        }
    }

    // Popup for slider configuration
    SliderConfigPopup {
        id: sliderConfigPopup
        trackerManager: root.trackerManager
    }

    // JavaScript functions
    function deleteTracker(name) {
        if (!root.trackerManager)
            return;

        root.trackerManager.removeValueTracker(name);
        console.log(`Deleted value tracker: ${name}`);
    }

    function deletePointTracker(name) {
        if (!root.trackerManager)
            return;

        root.trackerManager.removePtValueTracker(name);
        console.log(`Deleted point tracker: ${name}`);
    }
}
