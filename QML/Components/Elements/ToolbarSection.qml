import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

// ToolbarSection - A section in the properties toolbar with a title and content
Rectangle {
    id: root

    property string title: "Section"
    property bool expanded: false
    default property alias content: contentContainer.children

    color: "#1e1e1e"
    border.color: "#333333"
    border.width: 1
    radius: 4

    implicitHeight: header.height + (root.expanded ? contentContainer.implicitHeight + 10 : 0)
    Behavior on implicitHeight {
        NumberAnimation {
            duration: 150
            easing.type: Easing.InOutQuad
        }
    }

    Rectangle {
        id: header
        height: 30
        color: headerMouseArea.containsMouse ? "#2a2a2a" : "#1a1a1a"
        radius: 4
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        Text {
            text: root.title
            color: "#cccccc"
            font.pixelSize: 12
            font.bold: true
            anchors {
                left: parent.left
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            text: root.expanded ? "▼" : "▶"
            color: "#cccccc"
            font.pixelSize: 10
            anchors {
                right: parent.right
                rightMargin: 10
                verticalCenter: parent.verticalCenter
            }
        }

        MouseArea {
            id: headerMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.expanded = !root.expanded
        }
    }

    Item {
        id: contentContainer
        clip: true
        visible: root.expanded
        anchors {
            top: header.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }

        implicitHeight: childrenRect.height
    }
}
