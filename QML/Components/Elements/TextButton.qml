import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Effects

Item {

    id: root
    property color basecolor: "#19222f"
    property var callfunc: ()=>{
                               console.log("qwertyuiop")
                           }
    property string textval: "value"

    property int txtsize : 12
    property color txtcol: "#5ce1e6"

    height: 40
    width: 100

    property int rad: root.height/2


    Button{
        id:btn
        anchors.fill: parent

        hoverEnabled:true

        onClicked: callfunc()

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

            Text{
                id:txtbox
                text: textval
                font.pointSize: txtsize
                color: txtcol
                anchors.centerIn:  bg
            }
        }


    }

}
