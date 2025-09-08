import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0

Rectangle{
    color: Constants.darkGrayA
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.margins: 40

    height: 150

    ValueTracker{
        id: val1
    }
    Text{
        id:txt1
        text: val1.value
        color: "white"
    }
    Button{
        anchors.top: txt1.bottom
        text: "changeval"
        onClicked: {
            val1.updateVal(6)
            console.log("Updating")
        }
    }
}
