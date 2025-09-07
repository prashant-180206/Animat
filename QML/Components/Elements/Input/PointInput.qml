import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import Animat 1.0

Row {

    property string label: "value"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color labelColor: "#bbbbbb"
    property point pt: Qt.point(0,0)
    property var func: ()=>{
                           console.log("Point changed")
                       }

    property point pt2: pt

    spacing: 4
    Label {
        text: label
        color: labelColor
        width: 30
    }
    TextField {
        width: 50
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        text: Number(pt.x).toFixed(2)
        color: inputTextColor

        background: Rectangle {
            color: inputBackgroundColor
            radius: 4
            border.color: borderColor
        }

        onEditingFinished: {
            var x = parseFloat(text)
            if (!isNaN(x)) {
                pt2=Qt.point(x,pt.y)
                func()
            }
        }
    }

    TextField {
        width: 50
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        text: Number(pt.y).toFixed(2)
        color: inputTextColor
        background: Rectangle {
            color: inputBackgroundColor
            radius: 4
            border.color: borderColor
        }

        onEditingFinished: {
            var y = parseFloat(text)
            if (!isNaN(y)) {
                var x = pt.x
                pt2=Qt.point(x,y)
                func()
            }
        }
    }
}
