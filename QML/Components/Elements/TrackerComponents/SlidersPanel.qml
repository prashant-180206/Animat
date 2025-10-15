pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Animat 1.0

// ActiveSlidersPanel - Panel for displaying and managing active sliders
Rectangle {
    id: root

    // Properties
    property var trackerManager: null
    property var parser: null
    property Scene scene: null

    // Reactive model properties for sliders
    property var valueTrackersWithSliders: []
    property var pointTrackersWithSliders: []
    
    // Force update counter for reactive bindings
    property int updateCounter: 0

    // Update filtered lists when trackerManager changes
    onTrackerManagerChanged: {
        updateSliderLists();
    }

    function updateSliderLists() {
        console.log("ActiveSlidersPanel: Updating slider lists");
        
        if (!root.trackerManager) {
            root.valueTrackersWithSliders = [];
            root.pointTrackersWithSliders = [];
            return;
        }

        // Update value trackers with sliders
        let trackerNames = root.trackerManager.getTrackerNames();
        let filteredValueNames = [];
        for (let i = 0; i < trackerNames.length; i++) {
            if (root.trackerManager.hasSliderConfiguration(trackerNames[i])) {
                filteredValueNames.push(trackerNames[i]);
            }
        }
        root.valueTrackersWithSliders = filteredValueNames;

        // Update point trackers with sliders
        let pointTrackerNames = root.trackerManager.getPointTrackerNames();
        let filteredPointNames = [];
        for (let i = 0; i < pointTrackerNames.length; i++) {
            if (root.trackerManager.hasPointSliderConfiguration(pointTrackerNames[i])) {
                filteredPointNames.push(pointTrackerNames[i]);
            }
        }
        root.pointTrackersWithSliders = filteredPointNames;

        console.log(`Found ${filteredValueNames.length} value sliders and ${filteredPointNames.length} point sliders`);
    }

    // Signals
    signal sliderRemoved(string name, string type)

    Component.onCompleted: {
        console.log("ActiveSlidersPanel: Component completed, trackerManager:", root.trackerManager);
        updateSliderLists();
    }

    // Connect to TrackerManager signals for reactive updates
    Connections {
        target: root.trackerManager
        function onTrackerAdded(name) {
            console.log(`ActiveSlidersPanel: Tracker added: ${name}`);
            updateSliderLists();
        }
        function onTrackerRemoved(name) {
            console.log(`ActiveSlidersPanel: Tracker removed: ${name}`);
            updateSliderLists();
        }
        function onSliderConfigurationChanged(name) {
            console.log(`ActiveSlidersPanel: Slider configuration changed for: ${name}`);
            if (root.trackerManager) {
                if (root.trackerManager.hasSliderConfiguration(name)) {
                    console.log(`  - Value tracker ${name}: min=${root.trackerManager.getTrackerMin(name)}, max=${root.trackerManager.getTrackerMax(name)}`);
                }
                if (root.trackerManager.hasPointSliderConfiguration(name)) {
                    let minPt = root.trackerManager.getPointTrackerMin(name);
                    let maxPt = root.trackerManager.getPointTrackerMax(name);
                    console.log(`  - Point tracker ${name}: min=(${minPt.x},${minPt.y}), max=(${maxPt.x},${maxPt.y})`);
                }
            }
            updateSliderLists();
            // Force reactive update of all bindings
            root.updateCounter++;
        }
    }

    // Styling
    color: "#2c2c2c"
    border.color: "#444"
    border.width: 1
    radius: 4

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Text {
            text: "🎛️ Active Sliders"
            color: "#5ce1e6"
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Debug info
        Text {
            text: {
                if (!root.trackerManager) return "TrackerManager: NULL";
                
                let allTrackers = root.trackerManager.getTrackerNames().length;
                let allPointTrackers = root.trackerManager.getPointTrackerNames().length;
                let configuredSliders = root.valueTrackersWithSliders.length;
                let configuredPointSliders = root.pointTrackersWithSliders.length;
                
                return `Trackers: ${allTrackers} (${configuredSliders} with sliders) | Point Trackers: ${allPointTrackers} (${configuredPointSliders} with sliders)`;
            }
            color: "#ff6666"
            font.pixelSize: 10
            Layout.alignment: Qt.AlignHCenter
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ColumnLayout {
                width: parent.width
                spacing: 8

                // Value trackers sliders - only show trackers with slider configuration (min != max)
                Repeater {
                    model: root.valueTrackersWithSliders
                    delegate: ValSlider {
                        required property string modelData
                        required property int index

                        Layout.fillWidth: true
                        trackerName: modelData
                        trackerType: "val"
                        minValue: {
                            // Use updateCounter to force reactive updates
                            root.updateCounter;
                            return root.trackerManager ? root.trackerManager.getTrackerMin(modelData) : 0;
                        }
                        maxValue: {
                            // Use updateCounter to force reactive updates
                            root.updateCounter;
                            return root.trackerManager ? root.trackerManager.getTrackerMax(modelData) : 0;
                        }
                        currentValue: {
                            // Use updateCounter to force reactive updates
                            root.updateCounter;
                            if (root.trackerManager) {
                                let tracker = root.trackerManager.getValueTracker(modelData);
                                return tracker ? tracker.value : 0;
                            }
                            return 0;
                        }
                        parser: root.parser
                        onSliderRemoved: (name, type) => {
                            root.sliderRemoved(name, type);
                        }
                    }
                }

                // Point trackers sliders - only show trackers with slider configuration (min != max)
                Repeater {
                    model: root.pointTrackersWithSliders
                    delegate: ValSlider {
                        required property string modelData
                        required property int index

                        Layout.fillWidth: true
                        trackerName: modelData
                        trackerType: "pval"
                        minPoint: {
                            // Use updateCounter to force reactive updates
                            root.updateCounter;
                            return root.trackerManager ? root.trackerManager.getPointTrackerMin(modelData) : Qt.point(0, 0);
                        }
                        maxPoint: {
                            // Use updateCounter to force reactive updates
                            root.updateCounter;
                            return root.trackerManager ? root.trackerManager.getPointTrackerMax(modelData) : Qt.point(0, 0);
                        }
                        currentPoint: {
                            // Use updateCounter to force reactive updates
                            root.updateCounter;
                            if (root.trackerManager) {
                                let tracker = root.trackerManager.getPtValueTracker(modelData);
                                return tracker ? tracker.value : Qt.point(0, 0);
                            }
                            return Qt.point(0, 0);
                        }
                        parser: root.parser
                        onSliderRemoved: (name, type) => {
                            root.sliderRemoved(name, type);
                        }
                    }
                }

                // Empty state
                Text {
                    visible: (!root.trackerManager) || 
                             (root.valueTrackersWithSliders.length === 0 && 
                              root.pointTrackersWithSliders.length === 0)
                    Layout.alignment: Qt.AlignHCenter
                    text: root.trackerManager ? "No sliders configured (create sliders by clicking trackers above)" : "TrackerManager not connected"
                    color: "#888"
                    font.pixelSize: 12
                    font.italic: true
                }
            }
        }
    }
    
    // Timer for periodic value updates
    Timer {
        interval: 100  // Update every 100ms for smooth value changes
        running: root.trackerManager !== null && (root.valueTrackersWithSliders.length > 0 || root.pointTrackersWithSliders.length > 0)
        repeat: true
        onTriggered: {
            // Increment update counter to force property re-evaluation for current values
            root.updateCounter++;
        }
    }
}
