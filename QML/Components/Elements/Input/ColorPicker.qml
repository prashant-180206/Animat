import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Dialogs

Rectangle {
    id: root
    property color selectedColor: "transparent"
    property string label: ""
    property color newColor: selectedColor
    property var func: ()=>{}

    width: 32
    height: 32
    radius: 16
    color: selectedColor
    border.color: "#444444"
    border.width: 1
    property bool hovered: false

    Rectangle {
        id:colrec
        anchors.fill: parent
        radius: parent.radius
        color: "transparent"
        border.color: "#0078d4"
        border.width: 2
        opacity: hovered ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onEntered: hovered = true
        onExited: hovered = false
        onClicked: colorMenu.open()
    }

    Menu {
        parent: root
        x:30
        y:30
        id: colorMenu
        width: 150
        implicitHeight: 100
        background: Rectangle {
            color: "#23282e"
            radius: 10
            border.color: "#555"
            border.width: 1
        }

        Column {
            width: parent.width
            spacing: 4
            padding: 4

            Grid {
                id: colorGrid
                columns: 6
                spacing: 2

                Repeater {
                    model: [
                        "#000000", "#444444", "#8e8e8e", "#cccccc",
                        "#ffffff", "#e53935", "#fb8c00", "#fdd835",
                        "#43a047", "#1e88e5", "#8e24aa", "#009688",
                        "#e91e63", "#ff9800", "#fbc02d", "#a1887f"
                    ]
                    delegate: Item {
                        width: 20
                        height: 20
                        Rectangle {
                            width: 20; height: 20; radius: 10
                            anchors.centerIn: parent
                            color: modelData
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    root.selectedColor = modelData
                                    root.newColor = modelData
                                    root.func()
                                    colorMenu.close()
                                }
                            }
                        }
                    }
                }
                Button {
                    background: Rectangle {
                        color: "#34425a"
                        width: 20; height: 20; radius: 10
                    }
                    contentItem: Text{
                        text: "+"
                        color: "white"
                    }

                    onClicked: {
                        colorMenu.close()
                        colorDialog.open()
                    }
                }
            }


        }
    }

    ColorDialog {
        id: colorDialog
        selectedColor: root.selectedColor
        // showAlphaChannel: true
        onAccepted: {
            newColor = selectedColor
            selectedColor = selectedColor
            colrec.color = selectedColor
            func()
        }
    }
}
