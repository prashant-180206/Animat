import QtQuick 2.15
import Animat 1.0
import QtQuick.Layouts
import QtQuick.Dialogs
import "Elements"

Rectangle {
    id: root
    color: Constants.darkGrayA
    anchors.top: title.bottom
    width: parent.width
    height: 40

    // Property to receive scene from parent
    property var scene: null

    // FileHandler instance
    FileHandler {
        id: fileHandler

        onSaveCompleted: function (success, message) {
            console.log("Save completed:", success, message);
        }

        onLoadCompleted: function (success, message) {
            console.log("Load completed:", success, message);
        }

        onSaveProgress: function (percentage) {
            console.log("Save progress:", percentage);
        }

        onLoadProgress: function (percentage) {
            console.log("Load progress:", percentage);
        }
    }

    // Debug scene property changes
    onSceneChanged: {
        console.log("Taskbar scene changed to:", scene);
    }

    // File dialogs
    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Animat Scene files (*.json)", "All files (*)"]
        onAccepted: {
            console.log("Save dialog accepted, selectedFile:", selectedFile);
            console.log("Scene object:", root.scene);
            if (root.scene) {
                // Convert URL to local path - handle both Windows and Unix paths
                var filePath = selectedFile.toString();
                if (filePath.startsWith("file:///")) {
                    filePath = filePath.substring(8); // Remove "file:///"
                } else if (filePath.startsWith("file://")) {
                    filePath = filePath.substring(7); // Remove "file://"
                }
                console.log("Converted file path:", filePath);
                fileHandler.saveScene(root.scene, filePath);
            } else {
                console.log("No scene available for saving");
            }
        }
    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Animat Scene files (*.json)", "All files (*)"]
        onAccepted: {
            console.log("Open dialog accepted, selectedFile:", selectedFile);
            if (root.scene) {
                // Convert URL to local path
                var filePath = selectedFile.toString();
                if (filePath.startsWith("file:///")) {
                    filePath = filePath.substring(8); // Remove "file:///"
                } else if (filePath.startsWith("file://")) {
                    filePath = filePath.substring(7); // Remove "file://"
                }
                console.log("Converted file path:", filePath);
                fileHandler.loadScene(root.scene, filePath);
            } else {
                console.log("No scene available for loading");
            }
        }
    }

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 50
        x: 50
        // alignment: "AlignLeft"
        Repeater {
            model: ["Open", "Save", "Save As", "Play"]

            TextButton {
                id: txt
                required property string modelData
                textval: modelData
                txtsize: 12
                height: 30
                basecolor: root.color

                callfunc: () => {
                    switch (modelData) {
                    case "Open":
                        openDialog.open();
                        break;
                    case "Save":
                    case "Save As":
                        saveDialog.open();
                        break;
                    case "Play":
                        // Add play functionality here
                        break;
                    }
                }
            }
        }
    }
}
