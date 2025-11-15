pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0

Item {
    id: root
    width: 320
    height: 600

    property color backgroundColor: "#121212"
    property color listItemColor: "#2a2a2a"
    property color listItemHoverColor: "#383838"
    property color textColor: "#ffffff"
    property color secondaryTextColor: "#bbbbbb"
    property color deleteButtonColor: "#d32f2f"
    property color deleteButtonHoverColor: "#f44336"
    property AnimManager manager
    property var scene

    Rectangle {
        anchors.fill: parent
        color: root.backgroundColor
        radius: 6
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        Text {
            text: "Active Animations"
            color: root.textColor
            font.pixelSize: 16
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
            border.color: "#444444"
            border.width: 1
            radius: 4

            ScrollView {
                anchors.fill: parent
                anchors.margins: 4
                clip: true

                ListView {
                    id: animationListView
                    anchors.fill: parent
                    spacing: 6
                    model: root.manager ? root.manager.packets || [] : []

                    delegate: Rectangle {
                        id: animationItem
                        width: animationListView.width
                        height: 100
                        color: root.listItemColor
                        radius: 6
                        border.color: "#555555"
                        border.width: 1

                        property bool isHovered: false
                        required property int index
                        required property var modelData

                        states: [
                            State {
                                name: "hovered"
                                when: animationItem.isHovered
                                PropertyChanges {
                                    animationItem.color: root.listItemHoverColor
                                }
                            }
                        ]

                        transitions: [
                            Transition {
                                ColorAnimation {
                                    duration: 150
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        ]

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: parent.isHovered = true
                            onExited: parent.isHovered = false
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 12

                            // Animation info column
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                Text {
                                    text: animationItem.modelData && animationItem.modelData.name ? animationItem.modelData.name : "Animation #" + (animationItem.index + 1)
                                    color: root.textColor
                                    font.pixelSize: 12
                                    font.bold: true
                                }

                                Text {
                                    text: animationItem.modelData && animationItem.modelData.description ? animationItem.modelData.description : "No description"
                                    color: root.secondaryTextColor
                                    font.pixelSize: 10
                                    font.italic: true
                                }

                                RowLayout {
                                    spacing: 10

                                    Text {
                                        text: "Type: " + (animationItem.modelData && animationItem.modelData.animationType ? animationItem.modelData.animationType : "N/A")
                                        color: root.secondaryTextColor
                                        font.pixelSize: 9
                                    }

                                    Text {
                                        text: "Count: " + (animationItem.modelData && animationItem.modelData.animationCount !== undefined ? animationItem.modelData.animationCount : "0")
                                        color: root.secondaryTextColor
                                        font.pixelSize: 9
                                    }
                                }

                                RowLayout {
                                    spacing: 10

                                    Text {
                                        text: "Start: " + (animationItem.modelData && animationItem.modelData.startTime !== undefined ? animationItem.modelData.startTime.toFixed(2) + "s" : "N/A")
                                        color: root.secondaryTextColor
                                        font.pixelSize: 9
                                    }

                                    Text {
                                        text: "Duration: " + (animationItem.modelData && animationItem.modelData.duration !== undefined ? animationItem.modelData.duration.toFixed(2) + "s" : "N/A")
                                        color: root.secondaryTextColor
                                        font.pixelSize: 9
                                    }
                                }

                                Text {
                                    text: "Status: " + (root.manager && root.manager.activePacket === animationItem.modelData ? "Playing" : "Queued")
                                    color: root.manager && root.manager.activePacket === animationItem.modelData ? "#4CAF50" : root.secondaryTextColor
                                    font.pixelSize: 9
                                }
                            }

                            // Control buttons column
                            ColumnLayout {
                                Layout.preferredWidth: 50
                                spacing: 4

                                // Move Up button
                                Rectangle {
                                    id: moveUpButton
                                    Layout.preferredWidth: 40
                                    Layout.preferredHeight: 25
                                    color: animationItem.index === 0 ? "#666666" : "#4CAF50"
                                    radius: 3
                                    border.color: "#555555"
                                    border.width: 1
                                    enabled: animationItem.index > 0

                                    property bool moveUpHovered: false

                                    states: [
                                        State {
                                            name: "moveUpHovered"
                                            when: moveUpButton.moveUpHovered && moveUpButton.enabled
                                            PropertyChanges {
                                                moveUpButton.color: "#66BB6A"
                                            }
                                        }
                                    ]

                                    transitions: [
                                        Transition {
                                            ColorAnimation {
                                                duration: 150
                                                easing.type: Easing.InOutQuad
                                            }
                                        }
                                    ]

                                    Text {
                                        anchors.centerIn: parent
                                        text: "↑"
                                        color: "#ffffff"
                                        font.pixelSize: 12
                                        font.bold: true
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        enabled: parent.enabled
                                        onEntered: moveUpButton.moveUpHovered = true
                                        onExited: moveUpButton.moveUpHovered = false
                                        onClicked: {
                                            if (root.manager && animationItem.modelData) {
                                                root.manager.moveUp(animationItem.modelData);
                                            }
                                        }
                                    }
                                }

                                // Move Down button
                                Rectangle {
                                    id: moveDownButton
                                    Layout.preferredWidth: 40
                                    Layout.preferredHeight: 25
                                    color: (root.manager && animationItem.index === (root.manager.packets ? root.manager.packets.length - 1 : 0)) ? "#666666" : "#FF9800"
                                    radius: 3
                                    border.color: "#555555"
                                    border.width: 1
                                    enabled: root.manager && animationItem.index < (root.manager.packets ? root.manager.packets.length - 1 : 0)

                                    property bool moveDownHovered: false

                                    states: [
                                        State {
                                            name: "moveDownHovered"
                                            when: moveDownButton.moveDownHovered && moveDownButton.enabled
                                            PropertyChanges {
                                                moveDownButton.color: "#FFB74D"
                                            }
                                        }
                                    ]

                                    transitions: [
                                        Transition {
                                            ColorAnimation {
                                                duration: 150
                                                easing.type: Easing.InOutQuad
                                            }
                                        }
                                    ]

                                    Text {
                                        anchors.centerIn: parent
                                        text: "↓"
                                        color: "#ffffff"
                                        font.pixelSize: 12
                                        font.bold: true
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        enabled: parent.enabled
                                        onEntered: moveDownButton.moveDownHovered = true
                                        onExited: moveDownButton.moveDownHovered = false
                                        onClicked: {
                                            if (root.manager && animationItem.modelData) {
                                                root.manager.moveDown(animationItem.modelData);
                                            }
                                        }
                                    }
                                }

                                // Delete button
                                Rectangle {
                                    id: deleteButton
                                    Layout.preferredWidth: 40
                                    Layout.preferredHeight: 25
                                    color: root.deleteButtonColor
                                    radius: 3
                                    border.color: "#555555"
                                    border.width: 1

                                    property bool deleteHovered: false

                                    states: [
                                        State {
                                            name: "deleteHovered"
                                            when: deleteButton.deleteHovered
                                            PropertyChanges {
                                                deleteButton.color: root.deleteButtonHoverColor
                                            }
                                        }
                                    ]

                                    transitions: [
                                        Transition {
                                            ColorAnimation {
                                                duration: 150
                                                easing.type: Easing.InOutQuad
                                            }
                                        }
                                    ]

                                    Text {
                                        anchors.centerIn: parent
                                        text: "×"
                                        color: "#ffffff"
                                        font.pixelSize: 14
                                        font.bold: true
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onEntered: deleteButton.deleteHovered = true
                                        onExited: deleteButton.deleteHovered = false
                                        onClicked: {
                                            if (root.manager && animationItem.modelData) {
                                                root.manager.removePacket(animationItem.modelData);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Empty state message
        Text {
            visible: !root.manager || !root.manager.packets || root.manager.packets.length === 0
            text: "No active animations"
            color: root.secondaryTextColor
            font.pixelSize: 12
            font.italic: true
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 20
        }
    }
}
