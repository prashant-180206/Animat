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

    // function mobjectProperties() {
    //     if (canvas.SelectedMobject === null) {
    //         return "NO MObject Selected"
    //     }
    //     var props = canvas.SelectedMobject.getProperties()
    //     var str = ""
    //     for (var key in props) {
    //         if(props){
    //             str += key + ": " + props[key] + "\n"
    //         }
    //     }
    //     return str
    // }

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

    // Text {
    //     id: txter
    //     text: qsTr("text")+mobjectProperties()
    //     anchors.fill: parent
    //     color: "white"
    // }

    MpropertiesEditor{
        id:editor
        anchors{
            top: txt.bottom
            left: parent.left
        }
    }

    ValueTracker {
        id: val2
        value: 2
        onValueChanged: {
            canvas.SelectedMobject.getProperties().size = Qt.point(3,val2.value)
        }
    }

    Button{
        anchors.top: editor.bottom
        text: val2.value || "Start changing"
        onClicked: {
            val2.updateVal(4)
        }
    }
}
