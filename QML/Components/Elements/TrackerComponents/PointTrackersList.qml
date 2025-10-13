pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import "../Input"

// PointTrackersList - Component for displaying point trackers
Rectangle {
    id: root

    // Properties
    property alias model: pointTrackerListView.model

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

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: pointTrackerListView
                spacing: 2

                delegate: Rectangle {
                    id: delegateItem
                    required property int index
                    required property string name
                    required property real xval
                    required property real yval

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
