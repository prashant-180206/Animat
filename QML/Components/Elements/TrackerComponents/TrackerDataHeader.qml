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

    // Signals
    signal refreshRequested

    Layout.fillWidth: true

    StyledButton {
        text: "🔄 Refresh Data"
        backgroundColor: "#4a9eff"
        onClicked: root.refreshRequested()
    }

    Item {
        Layout.fillWidth: true
    }

    Text {
        text: `Trackers: ${root.trackerCount} | Point Trackers: ${root.pointTrackerCount}`
        color: "#ccc"
        font.pixelSize: 10
    }
}
