import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Animat 1.0
import "Input"

Rectangle {
    id: root

    property var scene: null

    color: "#1f1f1f"
    radius: 4

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Header
        Text {
            text: "🧮 Value Management"
            color: "#5ce1e6"
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Code Input Section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            color: "#2c2c2c"
            border.color: "#444"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Text {
                    text: "Expression Input:"
                    color: "#ccc"
                    font.pixelSize: 12
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    StyledTextArea {
                        id: codeInput
                        font.family: "Consolas, Monaco, monospace"
                        font.pixelSize: 11
                        backgroundColor: "#1a1a1a"
                        textColor: "#ffffff"
                        borderColor: "#333333"
                        placeholderColor: "#666666"
                        placeholderText: "Enter tracker commands and expressions...\n\n" + "Value Trackers:\n" + "val x = 5\n" + "val radius = 10 + 5 * sin(t)\n\n" + "Point Trackers:\n" + "pval center = (100, 200)\n" + "pval position = (x * 2, radius)\n\n" + "Expressions:\n" + "area = pi * radius^2\n" + "distance = sqrt(x^2 + y^2)"
                    }
                }
                // Action buttons
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    StyledButton {
                        text: "▶️ Evaluate"
                        enabled: root.scene !== null && codeInput.text.trim() !== ""
                        backgroundColor: "#4a9eff"
                        onClicked: evaluateExpression()
                    }

                    StyledButton {
                        text: "🗑️ Clear Input"
                        backgroundColor: "#666666"
                        onClicked: codeInput.text = ""
                    }

                    Item {
                        Layout.fillWidth: true
                    } // Spacer

                    StyledButton {
                        text: "📋 Clear Logs"
                        backgroundColor: "#d4651a"
                        isDestructive: true
                        onClicked: {
                            logModel.clear();
                            addLog("📋", "Logs cleared", "#5ce1e6");
                        }
                    }
                }
            }
        }

        // Quick Examples Section
        // Rectangle {
        //     Layout.fillWidth: true
        //     Layout.preferredHeight: 80
        //     color: "#2c2c2c"
        //     border.color: "#444"
        //     border.width: 1
        //     radius: 4

        //     ColumnLayout {
        //         anchors.fill: parent
        //         anchors.margins: 8
        //         spacing: 4

        //         Text {
        //             text: "🚀 Quick Examples:"
        //             color: "#ccc"
        //             font.pixelSize: 12
        //         }

        //         Flow {
        //             Layout.fillWidth: true
        //             spacing: 4

        //             Repeater {
        //                 model: [
        //                     {
        //                         text: "val x",
        //                         expr: "val x = 5"
        //                     },
        //                     {
        //                         text: "val r",
        //                         expr: "val radius = 10"
        //                     },
        //                     {
        //                         text: "pval",
        //                         expr: "pval center = (100, 200)"
        //                     },
        //                     {
        //                         text: "expr",
        //                         expr: "area = pi * radius^2"
        //                     },
        //                     {
        //                         text: "func",
        //                         expr: "val wave = 50 * sin(t)"
        //                     },
        //                     {
        //                         text: "point",
        //                         expr: "pval pos = (x, wave)"
        //                     },
        //                     {
        //                         text: "connect",
        //                         expr: "connect x myCircle radius"
        //                     }
        //                 ]

        //                 StyledButton {
        //                     required property var modelData
        //                     text: modelData.text
        //                     width: 60
        //                     height: 24
        //                     backgroundColor: "#4a9eff"
        //                     font.pixelSize: 9
        //                     onClicked: {
        //                         codeInput.text = modelData.expr;
        //                         evaluateExpression();
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }

        // Logs Section
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 150
            color: "#2c2c2c"
            border.color: "#444"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Text {
                    text: "📊 Evaluation Logs:"
                    color: "#ccc"
                    font.pixelSize: 12
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    ListView {
                        id: logListView
                        model: ListModel {
                            id: logModel
                        }
                        spacing: 2

                        delegate: Rectangle {
                            width: logListView.width
                            height: logText.implicitHeight + 8
                            color: index % 2 === 0 ? "#1a1a1a" : "#222"
                            radius: 2

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 4
                                spacing: 8

                                Text {
                                    text: model.timestamp
                                    color: "#666"
                                    font.pixelSize: 9
                                    font.family: "Consolas, Monaco, monospace"
                                    Layout.preferredWidth: 60
                                }

                                Text {
                                    text: model.icon
                                    font.pixelSize: 12
                                    Layout.preferredWidth: 20
                                }

                                Text {
                                    id: logText
                                    text: model.message
                                    color: model.color
                                    font.pixelSize: 10
                                    font.family: "Consolas, Monaco, monospace"
                                    wrapMode: Text.Wrap
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        // Auto-scroll to bottom when new items are added
                        onCountChanged: {
                            Qt.callLater(function () {
                                if (count > 0) {
                                    positionViewAtEnd();
                                }
                            });
                        }
                    }
                }
            }
        }
    }

    // JavaScript functions
    function evaluateExpression() {
        if (!root.scene) {
            addLog("❌", "Error: Scene not available", "#ff4444");
            return;
        }

        let expression = codeInput.text.trim();
        if (expression === "") {
            addLog("⚠️", "Warning: Empty expression", "#ffaa44");
            return;
        }

        try {
            // Check if it's a tracker command (val, pval, connect, etc.)
            if (expression.startsWith("val ") || expression.startsWith("pval ") || expression.startsWith("connect ") || expression.includes("=")) {
                // Execute as tracker script
                root.scene.executeTrackerScript(expression);
                addLog("🎯", `Executed: ${expression}`, "#44ff44");

                // Show current tracker values
                let trackerNames = root.scene.getTrackerNames();
                if (trackerNames.length > 0) {
                    addLog("📊", `Active trackers: ${trackerNames.join(", ")}`, "#5ce1e6");
                }
            } else {
                // Evaluate as mathematical expression
                let result = root.scene.evaluate(expression);
                addLog("✅", `${expression} = ${result}`, "#44ff44");
            }
        } catch (error) {
            addLog("❌", `Error: ${error}`, "#ff4444");
        }
    }

    function addLog(icon, message, color) {
        let now = new Date();
        let timestamp = now.getHours().toString().padStart(2, '0') + ":" + now.getMinutes().toString().padStart(2, '0') + ":" + now.getSeconds().toString().padStart(2, '0');

        logModel.append({
            timestamp: timestamp,
            icon: icon,
            message: message,
            color: color || "#ccc"
        });

        // Keep only last 100 entries
        if (logModel.count > 100) {
            logModel.remove(0, logModel.count - 100);
        }
    }

    // Component initialization
    Component.onCompleted: {
        addLog("🚀", "Tracker Management initialized", "#5ce1e6");
        if (root.scene) {
            addLog("🔗", "Scene and TrackerManager connected", "#44ff44");
            addLog("💡", "Ready for 'val', 'pval', 'connect' commands", "#5ce1e6");
        } else {
            addLog("⚠️", "Scene not connected", "#ffaa44");
        }
    }

    onSceneChanged: {
        if (root.scene) {
            addLog("🔗", "Scene and TrackerManager connected", "#44ff44");
            addLog("💡", "Ready for tracker commands", "#5ce1e6");
        } else {
            addLog("💔", "Scene disconnected", "#ff4444");
        }
    }
}
