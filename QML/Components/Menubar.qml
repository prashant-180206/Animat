import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects
import "Elements"

Rectangle {
    id: root
    property var scene: null  // Add scene property for tracker functionality
    property int updateCallCount: 0  // Track how many times updateTrackerDisplays is called

    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: 200

    ColumnLayout {
        anchors.top: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        spacing: 5

        Text {
            id: txt
            text: qsTr("Menu")
            font.pointSize: 14
            color: "#5ce1e6"
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 30
        }

        // Tab buttons
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#2c2c2c"
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.margins: 2
                spacing: 2

                Rectangle {
                    id: mobjectTab
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: stackLayout.currentIndex === 0 ? "#5ce1e6" : "transparent"
                    radius: 2

                    Text {
                        text: "Objects"
                        color: stackLayout.currentIndex === 0 ? "#000000" : "#5ce1e6"
                        font.pixelSize: 11
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: stackLayout.currentIndex = 0
                    }
                }

                Rectangle {
                    id: trackerTab
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: stackLayout.currentIndex === 1 ? "#5ce1e6" : "transparent"
                    radius: 2

                    Text {
                        text: "Trackers"
                        color: stackLayout.currentIndex === 1 ? "#000000" : "#5ce1e6"
                        font.pixelSize: 11
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: stackLayout.currentIndex = 1
                    }
                }
            }
        }

        // Content area
        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: 0

            // Objects tab content
            Rectangle {
                color: "transparent"

                GridLayout {
                    id: gridLayout
                    columns: 3
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.margins: 5
                    columnSpacing: 8
                    rowSpacing: 8

                    Repeater {
                        model: MobjectRegistry {}
                        delegate: MButton {
                            basecolor: root.color
                            textval: name
                            path: iconpath
                            callfunc: () => {
                                console.log("QWERTYUIOP", name);
                                canvas.add_mobject(name);
                            }
                        }
                    }
                }
            }

            // Trackers tab content
            Rectangle {
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
                        height: parent.height - 40
                        clip: true

                        ListView {
                            id: allTrackersList
                            model: ListModel {
                                id: allTrackersModel
                            }

                            delegate: Rectangle {
                                width: allTrackersList.width
                                height: 22
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
                                    font.pixelSize: 10
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
    }

    // Timer to refresh tracker values
    Timer {
        id: refreshTimer
        interval: 500 // Update every 500ms
        running: stackLayout.currentIndex === 1  // Only run when tracker tab is active
        repeat: true
        onTriggered: updateTrackerDisplays()
    }

    // Signal connections for immediate updates
    Connections {
        target: scene ? scene.trackers() : null
        function onTrackersListChanged() {
            if (stackLayout.currentIndex === 1) {
                updateTrackerDisplays();
            }
        }
    }

    function updateTrackerDisplays() {
        updateCallCount++;

        try {
            if (!scene || !scene.trackers()) {
                return;
            }

            var trackerManager = scene.trackers();

            // Clear and update combined tracker list
            allTrackersModel.clear();

            // Add value trackers
            var valueNames = trackerManager.getAllValueTrackerNames();
            for (var i = 0; i < valueNames.length; i++) {
                var name = valueNames[i];
                var value = trackerManager.getTrackerValue(name);
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
            for (var j = 0; j < pointNames.length; j++) {
                var pointName = pointNames[j];
                var point = trackerManager.getTrackerPoint(pointName);
                allTrackersModel.append({
                    "name": pointName,
                    "type": "point",
                    "value": 0,
                    "x": point.x,
                    "y": point.y
                });
            }
        } catch (error) {
            console.log("Menubar: Error updating trackers:", error.toString());
        }
    }

    Component.onCompleted: {
        // Delay initial update to ensure everything is initialized
        if (stackLayout.currentIndex === 1) {
            Qt.callLater(updateTrackerDisplays);
        }
    }

    border.width: 2
    border.color: Qt.lighter(root.color, 1.1)

    RectangularShadow {
        anchors.fill: root
        radius: root.radius
        spread: 5
        color: Qt.lighter(root.color)
        z: -10
    }
}
