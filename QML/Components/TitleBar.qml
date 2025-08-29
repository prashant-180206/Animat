import QtQuick 2.15
import Animat 1.0

Rectangle{
    color: Constants.darkBlack
    anchors.top: parent.top
    width: parent.width
    height: 50
    Text {
        id: titletext
        text: qsTr("Animat - Animation Framework")
        color: Constants.textLight
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 40
        anchors.left: parent.left
    }
}
