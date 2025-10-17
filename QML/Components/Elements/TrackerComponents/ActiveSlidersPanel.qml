pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Animat 1.0

// ActiveSlidersPanel - Panel for displaying sliders for currently active trackers (time-filtered)
Rectangle {
    id: root

    // Properties
    property TrackerManager trackerManager: null
    property Parser parser: null
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
            root.updateCounter++;
        }
        function onActivePtTrackersChanged() {
            root.updateCounter++;
        }
        function onSliderConfigurationChanged(name) {
            root.updateCounter++;
        }
    }

    // Styling
    color: Constants.darkBlack
    width: 300
    
    // Dynamic height based on number of trackers
    height: {
        if (!root.trackerManager) return 0;
        let activeValueTrackers = root.trackerManager.activeTrackers ? root.trackerManager.activeTrackers.length : 0;
        let activePointTrackers = root.trackerManager.activePtTrackers ? root.trackerManager.activePtTrackers.length : 0;
        let totalTrackers = activeValueTrackers + activePointTrackers;
        if (totalTrackers === 0) return 0;
        // Base height for header (80px) + dynamic height per tracker (60px each) + padding
        let calculatedHeight = (totalTrackers * 8 - 8)+ activePointTrackers*120+activeValueTrackers*80;
        return Math.min(calculatedHeight, parent.height * 0.8); // Max 80% of parent height
    }
    
    // Invisible when no trackers are active
    visible: root.trackerManager &&
             ((root.trackerManager.activeTrackers ? root.trackerManager.activeTrackers.length : 0) > 0 ||
              (root.trackerManager.activePtTrackers ? root.trackerManager.activePtTrackers.length : 0) > 0)


    ScrollView {
        anchors.fill: parent
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
                        return `${index}`;
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

                    onSliderRemoved: (name, type) => {
                        root.sliderRemoved(name, type);
                    }
                }
            }
        }
    }
}
