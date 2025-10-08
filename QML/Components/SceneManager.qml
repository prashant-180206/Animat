import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0
import "Elements/Input"

Rectangle {
    property Scene scene: null
    property int updateCallCount: 0

    color: Constants.darkGrayA
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.margins: 10
    height: 180

    Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        // Scene Manager Title (left half)
        Rectangle {
            width: (parent.width - 15) / 2  // Half width minus spacing
            height: parent.height
            color: "transparent"

            Text {
                text: "Scene Manager"
                color: Constants.textLight
                font.bold: true
                font.pixelSize: 14
                anchors.centerIn: parent
            }
        }

        // Tracker Values Display (right half)
        Rectangle {
            width: (parent.width - 15) / 2  // Half width minus spacing
            height: parent.height
            color: Constants.darkGrayB
            radius: 5
            border.color: Constants.textLight
            border.width: 1

            Column {
                anchors.fill: parent
                anchors.margins: 8

                Text {
                    text: "Tracker Values"
                    color: Constants.textLight
                    font.bold: true
                    font.pixelSize: 12
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
                    height: parent.height - 30
                    clip: true

                    ListView {
                        id: trackerValuesList
                        model: ListModel {
                            id: trackerValuesModel
                        }

                        delegate: Rectangle {
                            width: trackerValuesList.width
                            height: 20
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
                                font.pixelSize: 9
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }
        }
    }

    // Timer to refresh tracker values
    Timer {
        id: refreshTimer
        interval: 1000 // Update every second
        running: true
        repeat: true
        onTriggered: updateTrackerDisplays()
    }

    // Signal connections for immediate updates

    function updateTrackerDisplays() {
        updateCallCount++;

        try {
            if (!scene || !scene.getParser().trackerManager) {
                return;
            }

            var trackerManager = scene.getParser();

            // Clear and update tracker list
            trackerValuesModel.clear();

            // Add value trackers
            var valueNames = trackerManager.getAllTrackerValues();
            for (var i = 0; i < valueNames.length; i++) {
                var name = valueNames[i];
                var value = trackerManager.getTrackerValue(name);
                trackerValuesModel.append({
                                              "name": name,
                                              "type": "value",
                                              "value": value,
                                              "x": 0,
                                              "y": 0
                                          });
            }

            // Add point trackers
            // var pointNames = trackerManager.getAllPointTrackerNames();
            // for (var j = 0; j < pointNames.length; j++) {
            //     var pointName = pointNames[j];
            //     var point = trackerManager.getTrackerPoint(pointName);
            //     trackerValuesModel.append({
            //                                   "name": pointName,
            //                                   "type": "point",
            //                                   "value": 0,
            //                                   "x": point.x,
            //                                   "y": point.y
            //                               });
            // }
        } catch (error) {
            console.log("SceneManager: Error updating trackers:", error.toString());
        }
    }

    Component.onCompleted: {
        Qt.callLater(updateTrackerDisplays);
    }
}
