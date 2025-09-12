import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import Animat 1.0
import "Input"

Item {
    id: root
    property MProperties mprop : canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : canvas.getProperties()

    // Dark theme colors as properties for easy change
    property color backgroundColor: "#121212"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color labelColor: "#bbbbbb"
    property color borderColor: "#444444"
    property color buttonColor: "#2a2a2a"
    property color buttonTextColor: "#eeeeee"

    Rectangle {

        color: backgroundColor
        radius: 6
    }

    Column {

        anchors.margins: 12
        spacing: 10

        // Name: string
        TextField {
            placeholderText: "Name"
            text:  mprop.name || ""
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            onEditingFinished: {
                if (mprop) mprop.name = text
            }
        }

        // Pos: QPointF {x, y}

        PointInput{
            pt: mprop.pos
            label: "Pos:"
            func: ()=>{
                      mprop.pos = pt2;
                  }
        }

        // Size: QPair<qreal, qreal> {first, second}
        PointInput{
            visible: mprop.type !=="Line" ||mprop.type !=="Curve"
            pt: mprop.size
            label: "Size:"
            func: ()=>{
                      mprop.size =pt2
                  }
        }

        // Color: QColor
        Rectangle {
            visible: mprop !== null
            width: 100; height: 30
            radius: 4
            color: mprop && mprop.color !== undefined && mprop.color !== null ? mprop.color : "transparent"
            border.color: borderColor
            border.width: 1
            MouseArea {
                anchors.fill: parent
                onClicked: colorDialog.open()
            }
            ColorDialog {
                id: colorDialog
                selectedColor: mprop ? mprop.color : "transparent"
                onAccepted: {
                    if(mprop) mprop.color = selectedColor
                }
            }
        }

        // BorderColor: QColor
        Rectangle {
            visible: mprop.type ==="Rectangle" || mprop.type ==="Circle"
            width: 100; height: 30
            radius: 4
            color: mprop && mprop.borderColor !== undefined && mprop.borderColor !== null ? mprop.borderColor : "transparent"
            border.color: borderColor
            border.width: 1
            MouseArea {
                anchors.fill: parent
                onClicked: borderColorDialog.open()
            }
            ColorDialog {
                id: borderColorDialog
                selectedColor: mprop ? mprop.borderColor : "transparent"
                onAccepted: {
                    if(mprop) mprop.borderColor = selectedColor
                }
            }
        }

        // linePoints: QPair<QPointF, QPointF> {first, second}
        Column {
            visible: mprop.type === "Line"
            spacing: 4
            Label { text: "Line Points:"; color: labelColor }
            PointInput{
                pt: mprop.lineStart
                label: "Start:"
                func: ()=>{
                          mprop.lineStart = pt2
                      }
            }
            PointInput{
                pt: mprop.lineEnd
                label: "End:"
                func: ()=>{
                          mprop.lineEnd = pt2
                      }
            }
        }

        // Thickness: qreal
        Row {
            visible: mprop.thickness !== 0;
            spacing: 4
            Label { text: "Thickness:"; color: labelColor }
            TextField {
                width: 60
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.thickness !== undefined) ? mprop.thickness.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if (!mprop) return;
                    var val = parseFloat(text)
                    if (!isNaN(val)) mprop.thickness = val
                }
            }
        }

        Row {
            spacing: 4
            Label { text: "opacity:"; color: labelColor }
            TextField {
                width: 60
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.opacity !== undefined) ? mprop.opacity.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if (!mprop) return;
                    var val = parseFloat(text)
                    if (!isNaN(val)) mprop.opacity = val
                }
            }
        }


        Column{
            visible: mprop.type ==="Curve"
            spacing: 4
            // curveXFunc: string
            TextField {
                visible: mprop !== null
                placeholderText: "curveXFunc"
                text: mprop ? (mprop.curveXFunc !== undefined ? mprop.curveXFunc : "") : ""
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if(mprop) mprop.curveXFunc = text
                }
            }

            // curveYFunc: string
            TextField {
                visible: mprop !== null
                placeholderText: "curveYFunc"
                text: mprop ? (mprop.curveYFunc !== undefined ? mprop.curveYFunc : "") : ""
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if(mprop) mprop.curveYFunc = text
                }
            }

            // tRange: QPair<qreal, qreal> {first, second}
            PointInput{
                pt:mprop.tRange
                label: "t Range"
                func: ()=>{
                          mprop.tRange = pt2
                      }
            }
        }

        // segments: int
        Row {
            visible: mprop.type === "Curve" || mprop.type ==="Circle"
            spacing: 4
            Label { text: "Segments:"; color: labelColor }
            TextField {
                width: 60
                inputMethodHints: Qt.ImhDigitsOnly
                text: (mprop && mprop.segments !== undefined) ? mprop.segments.toString() : "0"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if(!mprop) return;
                    var val = parseInt(text)
                    if(!isNaN(val)) mprop.segments = val
                }
            }
        }

        // endPoints: QList<QPointF> (array of points)
        // Column {
        //     visible: mprop !== null
        //     spacing: 4
        //     Label { text: "End Points:"; color: labelColor }
        //     Repeater {
        //         model: (mprop && mprop.endPoints) ? mprop.endPoints.length : 0
        //         Row {
        //             spacing: 4
        //             TextField {
        //                 width: 50
        //                 inputMethodHints: Qt.ImhFormattedNumbersOnly
        //                 text: modelData && modelData.x !== undefined ? modelData.x.toFixed(2) : "0.00"
        //                 color: inputTextColor
        //                 background: Rectangle {
        //                     color: inputBackgroundColor
        //                     radius: 4
        //                     border.color: borderColor
        //                 }
        //                 onEditingFinished: {
        //                     if (!mprop || !mprop.endPoints) return;
        //                     var newPoints = mprop.endPoints.slice()
        //                     var x = parseFloat(text)
        //                     if (!isNaN(x)) {
        //                         newPoints[index] = Qt.point(x, newPoints[index].y)
        //                         mprop.endPoints = newPoints
        //                     }
        //                 }
        //             }
        //             TextField {
        //                 width: 50
        //                 inputMethodHints: Qt.ImhFormattedNumbersOnly
        //                 text: modelData && modelData.y !== undefined ? modelData.y.toFixed(2) : "0.00"
        //                 color: inputTextColor
        //                 background: Rectangle {
        //                     color: inputBackgroundColor
        //                     radius: 4
        //                     border.color: borderColor
        //                 }
        //                 onEditingFinished: {
        //                     if (!mprop || !mprop.endPoints) return;
        //                     var newPoints = mprop.endPoints.slice()
        //                     var y = parseFloat(text)
        //                     if (!isNaN(y)) {
        //                         newPoints[index] = Qt.point(newPoints[index].x, y)
        //                         mprop.endPoints = newPoints
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     Button {
        //         text: "Add Point"
        //         background: Rectangle {
        //             color: buttonColor
        //             radius: 4
        //         }
        //         contentItem: Text {
        //             text: parent.text
        //             anchors.centerIn: parent
        //             color: buttonTextColor
        //         }
        //         onClicked: {
        //             if (!mprop || !mprop.endPoints) return;
        //             var newPoints = mprop.endPoints.slice()
        //             newPoints.push(Qt.point(0, 0))
        //             mprop.endPoints = newPoints
        //         }
        //     }
        // }

        // radius: qreal
        Row {
            visible: mprop.type ==="Circle"
            spacing: 4
            Label { text: "Radius:"; color: labelColor }
            TextField {
                width: 60
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.radius !== undefined) ? mprop.radius.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if (!mprop) return;
                    var val = parseFloat(text)
                    if (!isNaN(val)) mprop.radius = val
                }
            }
        }
    }
}
