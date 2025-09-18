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

  AnimInput{
      anchors.top: editor.bottom
      packet: canvas.animator()
  }

}
