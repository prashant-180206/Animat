import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import "../Input"

// TrackerDataHeader - Header component for tracker data display
RowLayout {
    id: root

    // Properties
    property int trackerCount: 0
    property int pointTrackerCount: 0

    Layout.fillWidth: true

    Text {
        text: "� Tracker Data Overview"
        color: "#5ce1e6"
        font.pixelSize: 14
        font.bold: true
    }

    Item {
        Layout.fillWidth: true
    }

    Text {
        text: `Value Trackers: ${root.trackerCount} | Point Trackers: ${root.pointTrackerCount}`
        color: "#ccc"
        font.pixelSize: 11
    }
}
