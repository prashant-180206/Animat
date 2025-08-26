import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15

Rectangle{
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 300
    Rectangle{
        color: "white"
        width: 200
        height: 100

    Button{
        id: btn
        text: "Add MObject"
        onClicked: {
            var item = canvas.add_mobject("Line");
        }


        anchors.fill: parent

    }
    }
}
