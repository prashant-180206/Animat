import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0
import "Elements/Input"

Rectangle {

    color: Constants.darkGrayA
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.margins: 40
    height: 150

    // ScrollView{
    //     width: 300
    //     height: 140
    //     anchors.centerIn: parent
    //     AnimInput{
    //         z:100
    //     }
    //     ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    //     ScrollBar.vertical.policy: ScrollBar.AlwaysOn
    // }

    // AnimInput{
    //     packet: canvas.animator()
    //     z:100
    // }


    PlaybackInput{
        player: canvas.player()
        z:100
    }


}
