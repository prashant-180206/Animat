import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "./ToolbarSections"
import "./Elements"
import Animat 1.0

Rectangle {
    id: root
    property Scene scene: null
    visible: scene && scene.SelectedMobject

    color: Qt.transparent
    anchors.topMargin:  10

    ColumnLayout {
        spacing: 10
        anchors.centerIn: parent

        // ====== TOP SECTION ======
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            color: "#252525"
            radius: 6
            border.color: "#333333"
            border.width: 1
            Layout.preferredHeight: tabBar.implicitHeight + 10
            Layout.preferredWidth: tabBar.implicitWidth + 50

            Rectangle {
                anchors.centerIn: parent
                height: tabBar.implicitHeight
                width: tabBar.implicitWidth
                color: parent.color
                RowLayout {
                    id: tabBar
                    spacing: 10
                    // --- ID Section ---
                    Rectangle {
                        height: 30
                        Layout.preferredWidth: 150
                        Layout.alignment: Qt.AlignVCenter
                        color: "#1e1e1e"
                        radius: 6
                        border.color: "#444444"
                        Text {
                            text: "ID: " + (scene ? scene.activeId : "")
                            anchors.centerIn: parent
                            color: "#ffffff"
                            font.pixelSize: 14
                            font.bold: true
                            Layout.fillWidth: true
                        }
                    }

                    // --- Appearance Section ---
                    Rectangle {
                        Layout.preferredWidth: 750
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        color: "transparent"

                        AppearanceToolbarSection {
                            id: appearanceSection
                            mprop: scene && scene.SelectedMobject ? root.scene.SelectedMobject.getProperties() : null
                            anchors.fill: parent // This is fine if AppearanceToolbarSection isn't in a layout
                        }
                    }

                    // --- Delete Button ---
                    Button {
                        id: deleteButton
                        text: "🗑️"
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                        Layout.alignment: Qt.AlignVCenter
                        background: Rectangle {
                            color: deleteButton.pressed ? "#b71c1c" : deleteButton.hovered ? "#d32f2f" : "#c62828"
                            radius: 4
                        }
                        contentItem: Text {
                            text: deleteButton.text
                            color: "#ffffff"
                            font.pixelSize: 14
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            if (scene && scene.activeId)
                                scene.removeMobject(scene.activeId);
                        }
                    }
                }
            }
        }
        // ====== BOTTOM SECTION ======
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: specialsection.implicitHeight + 10
            Layout.preferredWidth: specialsection.implicitWidth + 10
            color: "#252525"
            radius: 6
            border.color: "#333333"
            border.width: 1
            Layout.margins: 4

            Rectangle {
                width: specialsection.implicitWidth
                height: specialsection.implicitHeight
                color: parent.color
                anchors.centerIn: parent
                SpecialSection {
                    id: specialsection
                    mprop: scene && scene.SelectedMobject ? root.scene.SelectedMobject.getProperties() : null
                    anchors.centerIn: parent // This is allowed here since SpecialSection is not in a layout
                    scene: root.scene
                }
            }
        }
    }
}
