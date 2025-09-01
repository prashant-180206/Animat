import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Effects

Item {

    property color basecolor: "#19222f"
    property int rad: 15
    property var callfunc: ()=>{
                               console.log("qwertyuiop")
                           }
    property string textval: "value"
    property string path: "qrc:assets/icons/rect"
    property int  size: 50

    id: root
    width: size
    height: size+10

    Button{
        id:btn
        icon.source: path
        icon.color: "white"

        icon.height: size-10
        icon.width: size-10
        width: size
        height: size

        hoverEnabled:true

        onClicked:
            callfunc()


        RectangularShadow {
            anchors.fill: bg
            // offset.x: -10
            // offset.y: -5
            radius: bg.radius
            // blur: 30
            spread: 5
            color: Qt.lighter(bg.color,1.4)
            z:-10
        }

        background:Rectangle{
            id:bg
            color:if(btn.down){
                      Qt.lighter(basecolor)
                  }else if(btn.hovered){
                      Qt.darker(basecolor,1.1)
                  }else{
                      basecolor
                  }
            radius: rad
            border.color: Qt.lighter(bg.color,1.15)
            border.width: 2

            anchors.fill: btn
        }
        Text {
            id: txt1
            text: qsTr(textval)
            color: "white"
            anchors.top: btn.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }


    }

}
