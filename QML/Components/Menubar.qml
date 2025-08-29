import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id:root
    color: Qt.transparent
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: 200

    GridLayout {
        id: gridLayout
        columns: 3
        anchors.centerIn: parent
        columnSpacing: 15
        // color:Qt.transparent
        Repeater {
            model: MobjectRegistry {}
            delegate: Column {
                width: 50
                height: 50
                // color: Qt.transparent

                Button {
                    id: i1
                    width: 50
                    height: 50
                    icon.source:iconpath
                    icon.color:"white"
                    icon.width: 50
                    icon.height:50
                    // color: Qt.transparent


                    background: Rectangle {
                        id:bgrec
                        color: "purple"
                        radius: 10
                        anchors.fill: parent
                    }
                    onClicked: {
                        console.log("Btn ", name , "Clicked")
                        canvas.add_mobject(name)
                    }
                }
                Text {
                    text: name
                    color: "white"
                    anchors.top: i1.bottom
                    height: 20
                }

            }
        }
    }

}
