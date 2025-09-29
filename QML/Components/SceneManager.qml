import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0
import "Elements/Input"

Rectangle {

    property var scene: null  // Add scene property like ValueManagement
    property int updateCallCount: 0  // Track how many times updateTrackerDisplays is called

    color: Constants.darkGrayA
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.margins: 40
    height: 150

    Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        // Playback controls - restore original size
        PlaybackInput {
            player: canvas.player()
            z: 100
            width: parent.width * 0.4  // Restored to 40% width
            height: parent.height * 0.8
        }

        // All Trackers Display (Combined)
        Rectangle {
            width: parent.width * 0.55  // Use remaining space
            height: parent.height
            color: Constants.darkGrayB
            radius: 5
            border.color: Constants.textLight
            border.width: 1

            Column {
                anchors.fill: parent
                anchors.margins: 8

                Text {
                    text: "All Trackers"
                    color: Constants.textLight
                    font.bold: true
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: Constants.textLight
                    opacity: 0.3
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ScrollView {
                    width: parent.width
                    height: parent.height - 40
                    clip: true

                    ListView {
                        id: allTrackersList
                        model: ListModel {
                            id: allTrackersModel
                        }

                        delegate: Rectangle {
                            width: allTrackersList.width
                            height: 25
                            color: index % 2 === 0 ? "transparent" : Constants.darkGrayA

                            Text {
                                text: {
                                    if (model.type === "value") {
                                        return model.name + " = " + model.value.toFixed(3);
                                    } else {
                                        return model.name + " = (" + model.x.toFixed(1) + ", " + model.y.toFixed(1) + ")";
                                    }
                                }
                                color: Constants.textLight
                                font.pixelSize: 11
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }
        }
    }    // Timer to refresh tracker values (DISABLED for debugging)
    Timer {
        id: refreshTimer
        interval: 500 // Update every 500ms (slower to be more stable)
        running: false  // DISABLED to debug repeated calls
        repeat: true
        onTriggered: updateTrackerDisplays()
    }

    // Signal connections for immediate updates
    Connections {
        target: scene ? scene.trackers() : null
        function onTrackersListChanged() {
            console.log("SceneManager: Received trackersListChanged signal!");
            updateTrackerDisplays();
        }
    }

    function updateTrackerDisplays() {
        updateCallCount++;
        console.log("SceneManager: updateTrackerDisplays() called #" + updateCallCount);

        try {
            if (!scene || !scene.trackers()) {
                console.log("SceneManager: Scene or trackers not available yet");
                return;
            }

            var trackerManager = scene.trackers();
            console.log("SceneManager: TrackerManager found, updating displays...");

            // Clear and update combined tracker list
            allTrackersModel.clear();

            // Add value trackers
            var valueNames = trackerManager.getAllValueTrackerNames();
            console.log("SceneManager: Found", valueNames.length, "value trackers:", valueNames);
            for (var i = 0; i < valueNames.length; i++) {
                var name = valueNames[i];
                var value = trackerManager.getTrackerValue(name);
                console.log("SceneManager: Adding value tracker:", name, "=", value);
                allTrackersModel.append({
                    "name": name,
                    "type": "value",
                    "value": value,
                    "x": 0,
                    "y": 0
                });
            }

            // Add point trackers
            var pointNames = trackerManager.getAllPointTrackerNames();
            console.log("SceneManager: Found", pointNames.length, "point trackers:", pointNames);
            for (var j = 0; j < pointNames.length; j++) {
                var pointName = pointNames[j];
                var point = trackerManager.getTrackerPoint(pointName);
                console.log("SceneManager: Adding point tracker:", pointName, "= (", point.x, ",", point.y, ")");
                allTrackersModel.append({
                    "name": pointName,
                    "type": "point",
                    "value": 0,
                    "x": point.x,
                    "y": point.y
                });
            }

            console.log("SceneManager: Total trackers in model:", allTrackersModel.count);
        } catch (error) {
            console.log("SceneManager: Error updating trackers:", error.toString());
        }
    }

    Component.onCompleted: {
        // Delay initial update to ensure everything is initialized
        Qt.callLater(updateTrackerDisplays);
    }
}
