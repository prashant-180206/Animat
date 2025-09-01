import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects
import "Elements"

Rectangle {
    id:root
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: 200

    Text {
        id: txt
        text: qsTr("Menu")
        font.pointSize: 14
        // font.bold: true
        color: "#5ce1e6"
        y:30
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }

    GridLayout {
        id: gridLayout
        columns: 3
        anchors.top:txt.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        columnSpacing: 15
        rowSpacing: 15
        anchors.margins: 15
        Repeater {
            model: MobjectRegistry {}
            delegate: MButton{
                basecolor: root.color

                textval: name
                path: iconpath
                callfunc: ()=>{
                              console.log("QWERTYUIOP", name)
                              canvas.add_mobject(name)
                              // console.log(ModelRegistry)
                          }
            }
        }
    }

    border.width: 2
    border.color: Qt.lighter(root.color,1.1)

    RectangularShadow {
        anchors.fill: root
        // offset.x: -10
        // offset.y: -5
        radius: root.radius
        // blur: 30
        spread: 5
        color: Qt.lighter(root.color)
        z:-10
    }

}
