import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

// ID Display Panel - Shows selected mobject ID and delete button
Rectangle {
    id: root
    
    property string activeId: ""
    
    signal deleteRequested()
    
    visible: activeId !== ""
    Layout.fillWidth: true
    height: 36
    color: "#1e1e1e"
    radius: 6
    border.color: "#444444"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Text {
            text: "ID: " + root.activeId
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            Layout.fillWidth: true
        }

        Button {
            id: deleteButton
            text: "Delete"
            Layout.preferredWidth: 60
            Layout.preferredHeight: 24
            background: Rectangle {
                color: deleteButton.pressed ? "#b71c1c" : deleteButton.hovered ? "#d32f2f" : "#c62828"
                radius: 4
            }
            contentItem: Text {
                text: deleteButton.text
                color: "#ffffff"
                font.pixelSize: 10
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: root.deleteRequested()
        }
    }
}