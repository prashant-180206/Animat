import QtQuick 2.15
import Animat 1.0
import QtQuick.Layouts
import "Elements"

Rectangle{
    id:root
    color: Constants.darkGrayA
    anchors.top: title.bottom
    width: parent.width
    height: 40

    RowLayout{
        anchors.verticalCenter: parent.verticalCenter
        spacing: 50
        x:50
        // alignment: "AlignLeft"
        Repeater{
            model: ["Open", "Save", "Save As","Play"]

            TextButton{
                required property string modelData
                textval:  modelData
                id:txt
                txtsize: 12
                height: 30
                basecolor: root.color


            }



        }
    }
}
