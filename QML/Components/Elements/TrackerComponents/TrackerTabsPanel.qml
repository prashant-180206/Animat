import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15

// TrackerTabsPanel - Tabbed panel for displaying tracker data
ColumnLayout {
    id: root

    // Properties
    property alias valueTrackersModel: valueTrackersList.model
    property alias pointTrackersModel: pointTrackersList.model

    // Signals
    signal valueTrackerClicked(string name, real value)
    signal pointTrackerClicked(string name, real xval, real yval)
    signal valueTrackerDeleteRequested(string name)
    signal pointTrackerDeleteRequested(string name)

    spacing: 10

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
        ValueTrackersList {
            id: valueTrackersList
            onTrackerClicked: (name, value) => {
                root.valueTrackerClicked(name, value);
            }
            onDeleteRequested: name => {
                root.valueTrackerDeleteRequested(name);
            }
        }

        // Point Trackers Tab
        PointTrackersList {
            id: pointTrackersList
            onTrackerClicked: (name, xval, yval) => {
                root.pointTrackerClicked(name, xval, yval);
            }
            onDeleteRequested: name => {
                root.pointTrackerDeleteRequested(name);
            }
        }
    }
}
