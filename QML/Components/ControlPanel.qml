import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15

Rectangle {
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 300

    Rectangle {
        id: rec1
        width: 200
        height: 100
        color: "yellow"
        Text {
            id: test
            text: "Hello world"
            anchors.fill: parent
        }
    }

    Rectangle {
        color: "white"
        width: 200
        height: 100
        anchors.top: rec1.bottom
        Button {
            id: btn
            text: "Add MObject"
            anchors.fill: parent
            onClicked: {
                var item = canvas.add_mobject("Line");
                console.log("Mobject added")
            }
        }
    }
}
