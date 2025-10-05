import QtQuick
import Animat 1.0
import QtQuick.Controls
import "Components"
import "Components/Elements/Input"

Window {
    id: globeroot
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Animat")
    color: Constants.darkBlack

    // Presentation mode properties
    property bool presentationMode: false

    // Panel visibility properties
    property bool sidebarVisible: true

    // Ensure window can receive key events

    TitleBar {
        id: title
    }
    Taskbar {
        id: taskbar
        scene: canvas
        mainWindow: globeroot
    }
    Sidebar {
        id: sidebar
        scene: canvas
        barvisible:  sidebarVisible
        anchors.top: taskbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
    Rectangle {

        anchors.top: taskbar.bottom
        anchors.bottom: parent.bottom
        anchors.left:  sidebar.right
        anchors.right: parent.right
        color: Constants.darkBlack

        // SceneManager {
        //     id: scenemanager
        //     scene: canvas
        // }

        Rectangle {
            id: sceneContainerDefault
            // anchors.top: scenemanager.bottom
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: Constants.darkBlack


            Rectangle {
                id: sceneParentContainer
                anchors.top: parent.top
                anchors.bottom: playbackControls.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 10
                color: globeroot.color
                z: 10
                Scene {
                    id: canvas
                    objectName: "canvas"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    // Add property to control border visibility
                    showBorders: true
                }
            }
            // PlaybackInput positioned below the scene
            PlaybackInput {
                id: playbackControls
                player: canvas.player()
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 15
                anchors.bottomMargin: 10
                width: Math.min(parent.width - 80, 800) // Use most available width, max 800
                opacity: playbackarea.containsMouse || !presentationMode ? 1 : 0
                height: 40
                z: 100
            }

            MouseArea {
                id: playbackarea
                hoverEnabled: true
                anchors.centerIn: playbackControls
                height: 40
                width: parent.width
            }
        }
    }

    // Functions for presentation mode
    function enterPresentationMode() {
        presentationMode = true;

        presentationOverlay.visible = true;

        // Hide borders in presentation mode
        canvas.showBorders = false;
        canvas.parent = sceneContainer;
        playbackControls.parent = sceneContainer;

        // Reset canvas anchoring
        canvas.anchors.horizontalCenter = undefined;
        canvas.anchors.top = undefined;
        canvas.anchors.topMargin = 0;
        canvas.anchors.verticalCenter = undefined;
        canvas.anchors.left = undefined;
        canvas.anchors.leftMargin = 0;

        // Calculate scale to fit the screen (leaving space for playback controls)
        var availableHeight = sceneContainer.height - 50; // Reserve 50px for playback controls
        var scaleX = sceneContainer.width / canvas.width;
        var scaleY = availableHeight / canvas.height;
        var finalScale = Math.min(scaleX, scaleY);

        canvas.scale = finalScale;
        canvas.anchors.centerIn = sceneContainer;
        canvas.anchors.verticalCenterOffset = -25; // Move up slightly for playback controls

        // Position playback controls at bottom of screen
        playbackControls.anchors.horizontalCenter = sceneContainer.horizontalCenter;
        playbackControls.anchors.top = undefined;
        playbackControls.anchors.topMargin = 0;
        playbackControls.anchors.bottom = sceneContainer.bottom;
        playbackControls.anchors.bottomMargin = 10;
        playbackControls.width = sceneContainer.width - 20; // Full width with minimal margin

        playbackarea.anchors.centerIn = playbackControls;
    }

    function exitPresentationMode() {
        presentationMode = false;
        presentationOverlay.visible = false;

        // Restore borders
        canvas.showBorders = true;

        // Move canvas and playback controls back to original container
        canvas.parent = sceneParentContainer;
        playbackControls.parent = sceneParentContainer;

        // Restore canvas properties
        canvas.scale = 0.5;
        canvas.anchors.centerIn = undefined;
        canvas.anchors.verticalCenterOffset = 0;
        canvas.anchors.horizontalCenter = sceneParentContainer.horizontalCenter;
        canvas.anchors.verticalCenter = sceneParentContainer.verticalCenter;
        // canvas.anchors.verticalCenter = undefined;

        // Restore playback controls position
        playbackControls.anchors.horizontalCenter = sceneParentContainer.horizontalCenter;
        playbackControls.anchors.bottom = sceneParentContainer.bottom;
        playbackControls.width = Math.min(sceneParentContainer.width - 80, 800); // Restore original width
        playbackarea.anchors.centerIn = playbackControls;
    }

    // Fullscreen presentation overlay
    Rectangle {
        id: presentationOverlay
        anchors.fill: parent
        color: "black"
        visible: false
        z: 100 // High z-order to be on top
        focus: true  // Ensure it can receive key events

        // Handle ESC key in overlay as well

        // Close button with mouse proximity detection
        Rectangle {
            id: closeButton
            width: 40
            height: 40
            radius: 20
            color: "#20ffffff"
            opacity: closeButtonArea.containsMouse ? 1 : 0
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 20
            z: 200  // Higher z-index to ensure visibility

            // Mouse proximity detection area (larger than button)
            MouseArea {
                id: closeButtonArea
                width: parent.width
                height: parent.height
                anchors.centerIn: parent
                hoverEnabled: true

                onClicked: exitPresentationMode()

                // Make button more visible when mouse is nearby
                onContainsMouseChanged: {
                    if (containsMouse) {
                        closeButton.color = "#80ffffff";
                    } else {
                        closeButton.color = "#20ffffff";
                    }
                }
            }
            Text {
                text: "×"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                anchors.centerIn: parent
            }
        }

        // Scaled scene container
        Item {
            id: sceneContainer
            anchors.fill: parent
            anchors.margins: 0  // No margins for full scaling

            // The main canvas will be moved here during presentation mode
        }
    }
}
