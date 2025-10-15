pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Animat 1.0

// ActiveSlidersPanel - Panel for displaying sliders for currently active trackers (time-filtered)
Rectangle {
    id: root

    // Properties
    property var trackerManager: null
    property var parser: null
    property Scene scene: null


    property int updateCounter: 0

    // Signals
    signal sliderRemoved(string name, string type)

    Component.onCompleted: {
        console.log("ActiveSlidersPanel: Component completed, trackerManager:", root.trackerManager);
    }

    // Connect to TrackerManager signals for reactive updates
    Connections {
        target: root.trackerManager
        function onActiveTrackersChanged() {
            console.log(`ActiveSlidersPanel: Active value trackers changed: ${root.trackerManager ? root.trackerManager.activeTrackers.length : 0}`);
            root.updateCounter++;
        }
        function onActivePtTrackersChanged() {
            console.log(`ActiveSlidersPanel: Active point trackers changed: ${root.trackerManager ? root.trackerManager.activePtTrackers.length : 0}`);
            root.updateCounter++;
        }
        function onSliderConfigurationChanged(name) {
            console.log(`ActiveSlidersPanel: Slider configuration changed for: ${name}`);
            root.updateCounter++;
        }
    }

    // Styling
    color: "#2c2c2c"
    border.color: "#ff8844"
    border.width: 1
    radius: 4

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Text {
            text: "🎯 Active Sliders (Time-Based)"
            color: "#ff8844"
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: {
                if (!root.trackerManager) return "TrackerManager: NULL";
                let activeValueTrackers = root.trackerManager.activeTrackers ? root.trackerManager.activeTrackers.length : 0;
                let activePointTrackers = root.trackerManager.activePtTrackers ? root.trackerManager.activePtTrackers.length : 0;
                return `Active Now: ${activeValueTrackers} Value | ${activePointTrackers} Point`;
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

                Repeater {
                    model: root.trackerManager ? root.trackerManager.activeTrackers : []
                    delegate: ValSlider {
                        required property var modelData
                        required property int index

                        Layout.fillWidth: true
                        trackerName: {
                            if (root.trackerManager) {
                                let trackerNames = root.trackerManager.getTrackerNames();
                                for (let i = 0; i < trackerNames.length; i++) {
                                    if (root.trackerManager.getValueTracker(trackerNames[i]) === modelData.tracker()) {
                                        return trackerNames[i];
                                    }
                                }
                            }
                            return `Active Tracker ${index}`;
                        }
                        trackerType: "val"
                        minValue: {
                            root.updateCounter;
                            return modelData.min;
                        }
                        maxValue: {
                            root.updateCounter;
                            return modelData.max;
                        }
                        currentValue: {
                            root.updateCounter;
                            return modelData.tracker().value;
                        }
                        parser: root.parser
                        
                        Rectangle {
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: 4
                            height: parent.height
                            color: "#44ff44"
                            radius: 2
                        }
                        
                        onSliderRemoved: (name, type) => {
                            root.sliderRemoved(name, type);
                        }
                    }
                }

                Repeater {
                    model: root.trackerManager ? root.trackerManager.activePtTrackers : []
                    delegate: ValSlider {
                        required property var modelData
                        required property int index

                        Layout.fillWidth: true
                        trackerName: {
                            if (root.trackerManager) {
                                let trackerNames = root.trackerManager.getPointTrackerNames();
                                for (let i = 0; i < trackerNames.length; i++) {
                                    if (root.trackerManager.getPtValueTracker(trackerNames[i]) === modelData.tracker()) {
                                        return trackerNames[i];
                                    }
                                }
                            }
                            return `Active Point Tracker ${index}`;
                        }
                        trackerType: "pval"
                        minPoint: {
                            root.updateCounter;
                            return modelData.min;
                        }
                        maxPoint: {
                            root.updateCounter;
                            return modelData.max;
                        }
                        currentPoint: {
                            root.updateCounter;
                            return modelData.tracker().value;
                        }
                        parser: root.parser
                        
                        Rectangle {
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: 4
                            height: parent.height
                            color: "#4488ff"
                            radius: 2
                        }
                        
                        onSliderRemoved: (name, type) => {
                            root.sliderRemoved(name, type);
                        }
                    }
                }

                Text {
                    visible: (!root.trackerManager) || 
                             ((root.trackerManager.activeTrackers ? root.trackerManager.activeTrackers.length : 0) === 0 && 
                              (root.trackerManager.activePtTrackers ? root.trackerManager.activePtTrackers.length : 0) === 0)
                    Layout.alignment: Qt.AlignHCenter
                    text: root.trackerManager ? "No trackers active at current time" : "TrackerManager not connected"
                    color: "#888"
                    font.pixelSize: 12
                    font.italic: true
                }

                Text {
                    visible: root.trackerManager && 
                             ((root.trackerManager.activeTrackers ? root.trackerManager.activeTrackers.length : 0) > 0 || 
                              (root.trackerManager.activePtTrackers ? root.trackerManager.activePtTrackers.length : 0) > 0)
                    Layout.alignment: Qt.AlignHCenter
                    text: "⏱️ Showing sliders for trackers active at current timeline position"
                    color: "#999"
                    font.pixelSize: 10
                    font.italic: true
                    Layout.topMargin: 10
                }
            }
        }
    }
    
    Timer {
        interval: 100
        running: root.trackerManager !== null && 
                 ((root.trackerManager.activeTrackers ? root.trackerManager.activeTrackers.length : 0) > 0 || 
                  (root.trackerManager.activePtTrackers ? root.trackerManager.activePtTrackers.length : 0) > 0)
        repeat: true
        onTriggered: {
            root.updateCounter++;
        }
    }
}
