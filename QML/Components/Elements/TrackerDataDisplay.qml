import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import Animat 1.0
import "Input"
import "TrackerComponents"

Rectangle {
    id: root

    property Scene scene: null
    property var parser: scene ? scene.getParser() : null

    color: "#1f1f1f"
    radius: 4

    // Slider management
    property var activeSliders: ({})

    ListModel {
        id: sliderModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Header
        Text {
            text: "📊 Value Tracker Data"
            color: "#5ce1e6"
            font.pixelSize: 16
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Main content - split into left (data) and right (sliders)
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            spacing: 10

            // Left side - Tracker Data Display
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: root.height * 0.5
                Layout.minimumHeight: 300
                color: "transparent"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    // Refresh button
                    RowLayout {
                        Layout.fillWidth: true

                        StyledButton {
                            text: " Refresh Data"
                            backgroundColor: "#4a9eff"
                            onClicked: refreshData()
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: `Trackers: ${trackerModel.count} | Point Trackers: ${pointTrackerModel.count}`
                            color: "#ccc"
                            font.pixelSize: 10
                        }
                    }

                    // Tabs for different tracker types
                    TabBar {
                        id: trackerTabs
                        Layout.fillWidth: true

                        TabButton {
                            text: "📊 Value Trackers"
                        }

                        TabButton {
                            text: "📍 Point Trackers"
                        }
                    }

                    // Content area
                    StackLayout {
                        id: stackLayout
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        currentIndex: trackerTabs.currentIndex

                        // Value Trackers Tab
                        Rectangle {
                            color: "#2c2c2c"
                            border.color: "#444"
                            border.width: 1
                            radius: 4

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 4

                                Text {
                                    text: "Value Trackers (qreal):"
                                    color: "#ccc"
                                    font.pixelSize: 12
                                    font.bold: true
                                }

                                ScrollView {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    clip: true

                                    ListView {
                                        id: trackerListView
                                        model: ListModel {
                                            id: trackerModel
                                        }
                                        spacing: 2

                                        delegate: Rectangle {
                                            width: trackerListView.width
                                            height: 40
                                            color: index % 2 === 0 ? "#1a1a1a" : "#222"
                                            radius: 4

                                            MouseArea {
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: {
                                                    sliderConfigPopup.trackerName = model.name;
                                                    sliderConfigPopup.trackerType = "val";
                                                    sliderConfigPopup.currentValue = model.value;
                                                    sliderConfigPopup.open();
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
                                                    text: "📊"
                                                    font.pixelSize: 16
                                                    Layout.preferredWidth: 20
                                                }

                                                Text {
                                                    text: model.name
                                                    color: "#5ce1e6"
                                                    font.pixelSize: 12
                                                    font.bold: true
                                                    Layout.preferredWidth: 120
                                                }

                                                Text {
                                                    text: "="
                                                    color: "#ccc"
                                                    font.pixelSize: 12
                                                    Layout.preferredWidth: 15
                                                }

                                                Text {
                                                    text: model.value.toFixed(3)
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
                                                    onClicked: deleteTracker(model.name)
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Point Trackers Tab
                        Rectangle {
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
                                        model: ListModel {
                                            id: pointTrackerModel
                                        }
                                        spacing: 2

                                        delegate: Rectangle {
                                            width: pointTrackerListView.width
                                            height: 40
                                            color: index % 2 === 0 ? "#1a1a1a" : "#222"
                                            radius: 4

                                            MouseArea {
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: {
                                                    sliderConfigPopup.trackerName = model.name;
                                                    sliderConfigPopup.trackerType = "pval";
                                                    sliderConfigPopup.currentPoint = Qt.point(model.x, model.y);
                                                    sliderConfigPopup.open();
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
                                                    text: model.name
                                                    color: "#5ce1e6"
                                                    font.pixelSize: 12
                                                    font.bold: true
                                                    Layout.preferredWidth: 120
                                                }

                                                Text {
                                                    text: "="
                                                    color: "#ccc"
                                                    font.pixelSize: 12
                                                    Layout.preferredWidth: 15
                                                }

                                                Text {
                                                    text: `(${model.x.toFixed(2)}, ${model.y.toFixed(2)})`
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
                                                    onClicked: deletePointTracker(model.name)
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

            // Right side - Active Sliders
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: root.height * 0.3
                Layout.minimumHeight: 250
                color: "#2c2c2c"
                border.color: "#444"
                border.width: 1
                radius: 4

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "🎛️ Active Sliders"
                        color: "#5ce1e6"
                        font.pixelSize: 14
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: sliderModel
                        spacing: 8
                        clip: true

                        delegate: ValSlider {
                            width: ListView.view.width
                            trackerName: model.name
                            trackerType: model.type
                            minValue: model.minVal
                            maxValue: model.maxVal
                            currentValue: model.currentVal
                            currentPoint: model.currentPt
                            minPoint: model.minPt
                            maxPoint: model.maxPt
                            parser: root.parser
                            onSliderRemoved: (name, type) => {
                                removeSlider(name, type);
                            }
                        }

                        // Empty state
                        Text {
                            visible: sliderModel.count === 0
                            anchors.centerIn: parent
                            text: "Click on a tracker to create a slider"
                            color: "#888"
                            font.pixelSize: 12
                            font.italic: true
                        }
                    }
                }
            }
        }
    }

    // Popup for slider configuration
    SliderConfigPopup {
        id: sliderConfigPopup

        onSliderRequested: (name, type, minVal, maxVal, currentVal, currentPt, minPt, maxPt) => {
            createSlider(name, type, minVal, maxVal, currentVal, currentPt, minPt, maxPt);
        }
    }

    // JavaScript functions
    function refreshData() {
        if (!root.parser) {
            console.log("Parser not available");
            return;
        }

        // Clear existing models
        trackerModel.clear();
        pointTrackerModel.clear();

        // Get value trackers
        let trackerNames = root.parser.getTrackerNames();
        for (let i = 0; i < trackerNames.length; i++) {
            let name = trackerNames[i];
            let value = root.parser.getTrackerValue(name);
            trackerModel.append({
                name: name,
                value: value
            });
        }

        // Get point trackers
        let pointTrackerNames = root.parser.getPointTrackerNames();
        for (let i = 0; i < pointTrackerNames.length; i++) {
            let name = pointTrackerNames[i];
            let point = root.parser.getPointTrackerValue(name);
            pointTrackerModel.append({
                name: name,
                x: point.x,
                y: point.y
            });
        }

        console.log(`Loaded ${trackerNames.length} value trackers and ${pointTrackerNames.length} point trackers`);
    }

    function createSlider(name, type, minVal, maxVal, currentVal, currentPt, minPt, maxPt) {
        // Check if slider already exists
        let key = name + "_" + type;
        if (root.activeSliders[key]) {
            console.log(`Slider for ${name} (${type}) already exists`);
            return;
        }

        // Get current values from parser
        let actualCurrentVal = currentVal;
        let actualCurrentPt = currentPt || Qt.point(0, 0);
        let actualMinPt = minPt || Qt.point(0, 0);
        let actualMaxPt = maxPt || Qt.point(0, 0);

        if (type === "val") {
            actualCurrentVal = root.parser.getTrackerValue(name);
        } else {
            actualCurrentPt = root.parser.getPointTrackerValue(name);
        }

        // Add to slider model
        let sliderData = {
            name: name,
            type: type,
            minVal: minVal,
            maxVal: maxVal,
            currentVal: actualCurrentVal,
            currentPt: actualCurrentPt,
            minPt: type === "pval" ? actualMinPt : Qt.point(0, 0),
            maxPt: type === "pval" ? actualMaxPt : Qt.point(0, 0)
        };

        sliderModel.append(sliderData);
        root.activeSliders[key] = sliderData;

        console.log(`Created slider for ${name} (${type}) with ranges: minPt=${actualMinPt.x},${actualMinPt.y} maxPt=${actualMaxPt.x},${actualMaxPt.y}`);
    }

    function removeSlider(name, type) {
        let key = name + "_" + type;
        delete root.activeSliders[key];

        // Remove from model
        for (let i = 0; i < sliderModel.count; i++) {
            let item = sliderModel.get(i);
            if (item.name === name && item.type === type) {
                sliderModel.remove(i);
                break;
            }
        }

        console.log(`Removed slider for ${name} (${type})`);
    }

    function deleteTracker(name) {
        if (!root.parser)
            return;

        // Use direct delete method instead of command
        let success = root.parser.deleteTracker(name);
        if (success) {
            console.log(`Deleted value tracker: ${name}`);
            refreshData();
            removeSlider(name, "val"); // Remove associated slider
        } else {
            console.log(`Failed to delete value tracker: ${name}`);
        }
    }

    function deletePointTracker(name) {
        if (!root.parser)
            return;

        // Use direct delete method instead of command
        let success = root.parser.deletePointTracker(name);
        if (success) {
            console.log(`Deleted point tracker: ${name}`);
            refreshData();
            removeSlider(name, "pval"); // Remove associated slider
        } else {
            console.log(`Failed to delete point tracker: ${name}`);
        }
    }

    // Component initialization
    Component.onCompleted: {
        refreshData();
    }

    // Auto-refresh when scene changes
    onSceneChanged: {
        refreshData();
    }

    // Auto-refresh timer
    // Timer {
    //     id: refreshTimer
    //     interval: 2000 // Refresh every 2 seconds
    //     running: true
    //     repeat: true
    //     onTriggered: refreshData()
    // }
}
