import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

// ToolbarButton - A button for the properties toolbar
Rectangle {
    id: root

    property string iconText: ""
    property string tooltipText: ""
    property int iconSize: 16
    property bool isActive: false

    signal clicked

    color: mouseArea.containsMouse ? "#2a2a2a" : (root.isActive ? "#3a3a3a" : "transparent")
    radius: 4

    Text {
        anchors.centerIn: parent
        text: root.iconText
        color: "#cccccc"
        font.pixelSize: root.iconSize
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }

    ToolTip {
        visible: mouseArea.containsMouse && root.tooltipText !== ""
        text: root.tooltipText
        delay: 500
    }
}
