pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import Animat 1.0
import "../Input"

// PointTrackersList - Component for displaying point trackers
Rectangle {
    id: root

    // Properties
    property var trackerManager: null
    property var pointTrackerNames: []

    // Update tracker names when trackerManager changes
    onTrackerManagerChanged: {
        updatePointTrackerNames();
    }

    // Connect to TrackerManager signals
    Connections {
        target: root.trackerManager
        function onTrackerAdded() {
            updatePointTrackerNames();
        }
        function onTrackerRemoved() {
            updatePointTrackerNames();
        }
    }

    function updatePointTrackerNames() {
        if (root.trackerManager) {
            root.pointTrackerNames = root.trackerManager.getPointTrackerNames();
        } else {
            root.pointTrackerNames = [];
        }
    }

    // Signals
    signal trackerClicked(string name, real xval, real yval)
    signal deleteRequested(string name)

    color: "#2c2c2c"
    border.color: "#444"
    border.width: 1
    radius: 4

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 4

        Text {
            text: "Point Trackers (QPointF):"
            color: "#ccc"
            font.pixelSize: 12
            font.bold: true
        }

        // Debug information
        Text {
            text: `TrackerManager: ${root.trackerManager ? "OK" : "NULL"} | Point Names: ${root.pointTrackerNames.length}`
            color: "#ff6666"
            font.pixelSize: 10
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: pointTrackerListView
                spacing: 2
                model: root.pointTrackerNames

                delegate: Rectangle {
                    id: delegateItem
                    required property int index
                    required property string modelData

                    property string name: modelData
                    property point trackerPoint: {
                        if (root.trackerManager) {
                            let tracker = root.trackerManager.getPtValueTracker(name);
                            return tracker ? tracker.value : Qt.point(0, 0);
                        }
                        return Qt.point(0, 0);
                    }
                    property real xval: trackerPoint.x
                    property real yval: trackerPoint.y

                    width: pointTrackerListView.width
                    height: 40
                    color: index % 2 === 0 ? "#1a1a1a" : "#222"
                    radius: 4

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            root.trackerClicked(delegateItem.name, delegateItem.xval, delegateItem.yval);
                        }

                        Rectangle {
                            anchors.fill: parent
                            color: "#4a9eff"
                            opacity: parent.containsMouse ? 0.1 : 0
                            radius: 4

                            Behavior on opacity {
                                NumberAnimation {
                                    duration: 150
                                }
                            }
                        }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 12

                        Text {
                            text: "📍"
                            font.pixelSize: 16
                            Layout.preferredWidth: 20
                        }

                        Text {
                            text: delegateItem.name
                            color: "#5ce1e6"
                            font.pixelSize: 12
                            font.bold: true
                            Layout.preferredWidth: 60
                        }

                        Text {
                            text: "="
                            color: "#ccc"
                            font.pixelSize: 12
                            Layout.preferredWidth: 15
                        }

                        Text {
                            text: `(${delegateItem.xval.toFixed(2)}, ${delegateItem.yval.toFixed(2)})`
                            color: "#44ff44"
                            font.pixelSize: 12
                            font.family: "Consolas, Monaco, monospace"
                            Layout.fillWidth: true
                        }

                        StyledButton {
                            text: "🗑️"
                            backgroundColor: "#d4651a"
                            Layout.preferredWidth: 30
                            Layout.preferredHeight: 25
                            onClicked: root.deleteRequested(delegateItem.name)
                        }
                    }
                }
            }
        }
    }
}
