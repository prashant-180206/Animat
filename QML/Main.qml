import QtQuick
import Animat 1.0
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "Components"
import "Components/Elements"
import "Components/Elements/Input"
import "Components/Elements/TrackerComponents"

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
    property bool companionPanelVisible: false
    property string companionPanelType: ""

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
        barvisible: sidebarVisible
        anchors.top: taskbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        
        onCompanionPanelChanged: (panelType, visible) => {
                                     globeroot.companionPanelType = panelType;
                                     globeroot.companionPanelVisible = visible;
                                 }
    }
    Rectangle {
        anchors.top: taskbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: sidebar.right
        anchors.right: parent.right
        color: Constants.darkBlack

        RowLayout {
            anchors.fill: parent
            spacing: 0
            
            // Main scene area
            Rectangle {
                id: sceneContainerDefault
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Constants.darkBlack

                // Properties toolbar above the scene

                PropertiesToolbar {
                    id: propbar
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 120
                    scene: canvas
                    z: 15
                    color: globeroot.color
                    scale: companionPanelVisible? 0.8 :1

                }

                Rectangle {
                    id: sceneParentContainer
                    anchors.top: propbar.bottom
                    anchors.bottom: parent.bottom
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
                        scale:companionPanelVisible? 0.6 : 0.7
                        clip: true
                        showBorders: true

                        ActiveSlidersPanel {
                            id: activeSlidersPanel
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.margins: 10
                            // width: 300
                            // height: Math.min(400, parent.height * 0.6)
                            z: 20
                            trackerManager: canvas ? canvas.getParser().trackerManager() : null
                            parser: canvas ? canvas.getParser() : null
                            scene: canvas

                            onSliderRemoved: (name, type) => {
                                                 console.log(`Main: Slider removed for ${name} (${type})`);
                                             }
                        }

                        // Continue button - only visible when ActiveSlidersPanel has content
                        MButton {
                            id: continueButton
                            textval: "▶ Continue"
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            anchors.margins: 15
                            width: 100
                            height: 35
                            z: 20

                            // Only show when there are active trackers
                            visible: activeSlidersPanel.trackerManager &&
                                     ((activeSlidersPanel.trackerManager.activeTrackers ?
                                           activeSlidersPanel.trackerManager.activeTrackers.length : 0) > 0 ||
                                      (activeSlidersPanel.trackerManager.activePtTrackers ?
                                           activeSlidersPanel.trackerManager.activePtTrackers.length : 0) > 0)

                            callfunc: ()=> {
                                          if (activeSlidersPanel.trackerManager) {
                                              console.log("Main: Calling Continue() on TrackerManager");
                                              activeSlidersPanel.trackerManager.Continue();
                                          }
                                      }
                        }

                        MouseArea{
                            id:playbackarea
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                            anchors.topMargin: 15
                            anchors.bottomMargin: 10
                            width: parent.width - 80
                            height: 40
                            hoverEnabled: true
                            PlaybackInput {
                                id: playbackControls
                                player: canvas.player()
                                opacity:playbackarea.containsMouse? 1:0
                                anchors.fill: parent
                                z: 100
                            }
                        }
                    }
                }
            }
            
            // Companion Panel Area - moved to be a sibling of sceneContainerDefault in RowLayout
            Rectangle {
                id: companionPanel
                Layout.preferredWidth: globeroot.companionPanelVisible ? 300 : 0
                Layout.fillHeight: true
                color: Constants.darkGrayA
                visible: globeroot.companionPanelVisible

                // Top bar with toggle button
                Rectangle {
                    id: companionTopBar
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 40
                    color: Constants.darkGrayC

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8

                        Text {
                            text: {
                                if (globeroot.companionPanelType === "activeAnimations") return "🎬 Active Animations";
                                if (globeroot.companionPanelType === "trackerData") return "📊 Tracker Data";
                                return "Companion Panel";
                            }
                            color: "#5ce1e6"
                            font.pixelSize: 14
                            font.bold: true
                            Layout.fillWidth: true
                        }

                        Button {
                            text: "✕"
                            width: 24
                            height: 24
                            background: Rectangle {
                                color: parent.pressed ? "#ff4444" : (parent.hovered ? "#ff6666" : "transparent")
                                radius: 12
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                globeroot.companionPanelVisible = false;
                                globeroot.companionPanelType = "";
                            }
                        }
                    }
                }

                // Content area
                Rectangle {
                    anchors.top: companionTopBar.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: "transparent"

                    // ActiveAnimationsList for animations tab
                    ActiveAnimationsList {
                        id: activeAnimationsPanel
                        anchors.fill: parent
                        anchors.margins: 10
                        visible: globeroot.companionPanelType === "activeAnimations"
                        manager: canvas ? canvas.animator() : null
                        scene: canvas
                    }

                    // TrackerDataDisplay for values tab
                    TrackerDataDisplay {
                        id: trackerDataPanel
                        anchors.fill: parent
                        anchors.margins: 10
                        visible: globeroot.companionPanelType === "trackerData"
                        scene: canvas
                    }
                }
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

        // Move ActiveSlidersPanel and Continue button to presentation overlay
        activeSlidersPanel.parent = sceneContainer;
        continueButton.parent = sceneContainer;

        // Hide companion panel in presentation mode but remember its state
        if (globeroot.companionPanelVisible) {
            companionPanel.visible = false;
        }

        // Reset canvas anchoring
        canvas.anchors.horizontalCenter = undefined;
        canvas.anchors.top = undefined;
        canvas.anchors.topMargin = 0;
        canvas.anchors.verticalCenter = undefined;
        canvas.anchors.left = undefined;
        canvas.anchors.leftMargin = 0;

        // Calculate scale to fit the screen (leaving space for playback controls)
        var availableHeight = sceneContainer.height;
        var scaleX = sceneContainer.width / canvas.width;
        var scaleY = availableHeight / canvas.height;
        var finalScale = Math.min(scaleX, scaleY);

        canvas.scale = finalScale;
        canvas.anchors.centerIn = sceneContainer;

    }

    function exitPresentationMode() {
        presentationMode = false;
        presentationOverlay.visible = false;

        // Restore borders
        canvas.showBorders = true;

        // Move canvas and playback controls back to original container
        canvas.parent = sceneParentContainer;

        // Move ActiveSlidersPanel and Continue button back to original container
        activeSlidersPanel.parent = canvas;
        continueButton.parent = canvas;

        // Restore companion panel visibility
        if (globeroot.companionPanelType !== "") {
            companionPanel.visible = globeroot.companionPanelVisible;
        }

        // Restore canvas properties
        canvas.scale = Qt.binding(function() {
            return globeroot.companionPanelVisible ? 0.6 : 0.7;
        });

        canvas.anchors.centerIn = undefined;
        canvas.anchors.verticalCenterOffset = 0;
        canvas.anchors.horizontalCenter = sceneParentContainer.horizontalCenter;
        canvas.anchors.verticalCenter = sceneParentContainer.verticalCenter;
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
        }
    }
}

