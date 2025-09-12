import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15
import "Elements"

Rectangle {
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 300

    Text {
        id: txt
        text: qsTr("Properties") /* mobjectProperties()*/
        font.pointSize: 14
        // font.bold: true
        color: "#5ce1e6"
        y:30
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MpropertiesEditor{
        anchors.top: txt.bottom
        // anchors.horizontalCenter: parent.horizontalCenter
        id:editor
        height: 400
    }



    Button{
        anchors.top: editor.bottom
        id: upbtn
        text: val2.value.toString() || "Start changing"
        onClicked: {
            val2.updateVal(Qt.point(1,2))
        }
    }
    Button{
        anchors.top: upbtn.bottom
        id: upbt
        text: val2.value.toString() || "Start changing"
        onClicked: {
            val2.updateVal(Qt.point(6,6))
        }
    }

    TextField{
        color: "black"
        anchors.top: upbt.bottom
        placeholderText: "Enter Code "
    }

    PtValueTracker {
        id: val2
    }
}
