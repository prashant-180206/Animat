pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// ActiveSlidersPanel - Panel for displaying and managing active sliders
Rectangle {
    id: root

    // Properties
    property alias sliderModel: listView.model
    property var parser: null
    property Scene scene: null

    // Signals
    signal sliderRemoved(string name, string type)

    Component.onCompleted: {
        console.log("ActiveSlidersPanel: Component completed, model:", listView.model);
    }

    onSliderModelChanged: {
        console.log("ActiveSlidersPanel: sliderModel changed, new count:", sliderModel ? sliderModel.count : "null");
    }

    // Styling
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

        // Debug info
        Text {
            text: `Model count: ${listView.model ? listView.model.count : "null"}`
            color: "#ff6666"
            font.pixelSize: 10
            Layout.alignment: Qt.AlignHCenter
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            clip: true

            delegate: ValSlider {
                required property string type
                required property string name
                required property real minVal
                required property real maxVal
                required property real currentVal
                required property point currentPt
                required property point minPt
                required property point maxPt

                width: ListView.view.width
                trackerName: name
                trackerType: type
                minValue: minVal
                maxValue: maxVal
                currentValue: currentVal
                currentPoint: currentPt
                minPoint: minPt
                maxPoint: maxPt
                parser: root.parser
                onSliderRemoved: (name, type) => {
                    root.sliderRemoved(name, type);
                }
            }

            // Empty state
            Text {
                visible: listView.model && listView.model.count === 0
                anchors.centerIn: parent
                text: "Click on a tracker to create a slider"
                color: "#888"
                font.pixelSize: 12
                font.italic: true
            }
        }
    }
}
