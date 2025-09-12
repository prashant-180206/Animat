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

        Loader {
            id: nameLoader
            active: mprop && mprop.base !== null
            sourceComponent: nameComponent
        }
        Component {
            id: nameComponent
            TextField {
                placeholderText: "Name"
                text: mprop.base.name ? mprop.base.name : ""
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onEditingFinished: {
                    if (mprop && mprop.base) mprop.base.name = text
                }
            }
        }

        Loader {
            id: posLoader
            active: mprop && mprop.base !== null
            sourceComponent: posComponent
        }
        Component {
            id: posComponent
            PointInput {
                pt: mprop.base.pos
                label: "Pos:"
                func: ()=> {
                          if (mprop && mprop.base) mprop.base.pos = pt2
                      }
            }
        }

        Loader {
            id: sizeLoader
            active: mprop && mprop.base !== null && mprop.type !== "Line" && mprop.type !== "Curve"
            sourceComponent: sizeComponent
        }
        Component {
            id: sizeComponent
            PointInput {
                pt: mprop.base.size
                label: "Size:"
                func: ()=> {
                          if (mprop && mprop.base) mprop.base.size = pt2
                      }
            }
        }

        Loader {
            id: colorLoader
            active: mprop && mprop.base !== null
            sourceComponent: colorComponent
        }
        Component {
            id: colorComponent
            Rectangle {
                width: 100; height: 30
                radius: 4
                color: mprop.base.color !== undefined && mprop.base.color !== null ? mprop.base.color : "transparent"
                border.color: borderColor
                border.width: 1
                MouseArea {
                    anchors.fill: parent
                    onClicked: colorDialog.open()
                }
                ColorDialog {
                    id: colorDialog
                    selectedColor: mprop.base.color ? mprop.base.color : "transparent"
                    onAccepted: {
                        if(mprop && mprop.base) mprop.base.color = selectedColor
                    }
                }
            }
        }

        Loader {
            id: borderColorLoader
            active: mprop && mprop.polygon !== null
            sourceComponent: borderColorComponent
        }
        Component {
            id: borderColorComponent
            Rectangle {
                width: 100; height: 30
                radius: 4
                color: mprop.polygon.borderColor !== undefined && mprop.polygon.borderColor !== null ? mprop.polygon.borderColor : "transparent"
                border.color: borderColor
                border.width: 1
                MouseArea {
                    anchors.fill: parent
                    onClicked: borderColorDialog.open()
                }
                ColorDialog {
                    id: borderColorDialog
                    selectedColor: mprop.polygon.borderColor ? mprop.polygon.borderColor : "transparent"
                    onAccepted: {
                        if(mprop && mprop.polygon) mprop.polygon.borderColor = selectedColor
                    }
                }
            }
        }

        Loader {
            id: linePointsLoader
            active: mprop && mprop.line !== null
            sourceComponent: linePointsComponent
        }
        Component {
            id: linePointsComponent
            Column {
                spacing: 4
                Label { text: "Line Points:"; color: labelColor }
                PointInput {
                    pt: mprop.line.lineStart
                    label: "Start:"
                    func: ()=> {
                              if(mprop && mprop.line) mprop.line.lineStart = pt2
                          }
                }
                PointInput {
                    pt: mprop.line.lineEnd
                    label: "End:"
                    func: ()=> {
                              if(mprop && mprop.line) mprop.line.lineEnd = pt2
                          }
                }
            }
        }

        Loader {
            id: thicknessLoader
            active: mprop && mprop.polygon !== null
            sourceComponent: thicknessComponent
        }
        Component {
            id: thicknessComponent
            Row {
                spacing: 4
                Label { text: "Thickness:"; color: labelColor }
                TextField {
                    width: 60
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: mprop.polygon.thickness !== undefined ? mprop.polygon.thickness.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onEditingFinished: {
                        if (!mprop) return;
                        var val = parseFloat(text)
                        if (!isNaN(val) && mprop.polygon) mprop.polygon.thickness = val
                    }
                }
            }
        }

        Loader {
            id: opacityLoader
            active: mprop && mprop.base !== null
            sourceComponent: opacityComponent
        }
        Component {
            id: opacityComponent
            Row {
                spacing: 4
                Label { text: "opacity:"; color: labelColor }
                TextField {
                    width: 60
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: mprop.base.opacity !== undefined ? mprop.base.opacity.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onEditingFinished: {
                        if (!mprop) return;
                        var val = parseFloat(text)
                        if (!isNaN(val) && mprop.base) mprop.base.opacity = val
                    }
                }
            }
        }

        Loader {
            id: curveLoader
            active: mprop && mprop.curve !== null
            sourceComponent: curveComponent
        }
        Component {
            id: curveComponent
            Column {
                spacing: 4
                TextField {
                    placeholderText: "curveXFunc"
                    text: mprop.curve.curveXFunc !== undefined ? mprop.curve.curveXFunc : ""
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onEditingFinished: {
                        if(mprop && mprop.curve) mprop.curve.curveXFunc = text
                    }
                }
                TextField {
                    placeholderText: "curveYFunc"
                    text: mprop.curve.curveYFunc !== undefined ? mprop.curve.curveYFunc : ""
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onEditingFinished: {
                        if(mprop && mprop.curve) mprop.curve.curveYFunc = text
                    }
                }
                PointInput {
                    pt: mprop.curve.tRange
                    label: "t Range"
                    func: ()=> {
                              if(mprop && mprop.curve) mprop.curve.tRange = pt2
                          }
                }
            }
        }

        Loader {
            id: segmentsLoader
            active: mprop && (mprop.type === "Curve" || mprop.type === "Circle")
            sourceComponent: segmentsComponent
        }
        Component {
            id: segmentsComponent
            Row {
                spacing: 4
                Label { text: "Segments:"; color: labelColor }
                TextField {
                    width: 60
                    inputMethodHints: Qt.ImhDigitsOnly
                    text: mprop.curve.segments !== undefined ? mprop.curve.segments.toString() : "0"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onEditingFinished: {
                        if(!mprop) return;
                        var val = parseInt(text)
                        if(!isNaN(val) && mprop.curve) mprop.curve.segments = val
                    }
                }
            }
        }

        Loader {
            id: radiusLoader
            active: mprop && mprop.circle !== null
            sourceComponent: radiusComponent
        }
        Component {
            id: radiusComponent
            Row {
                spacing: 4
                Label { text: "Radius:"; color: labelColor }
                TextField {
                    width: 60
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: mprop.circle.radius !== undefined ? mprop.circle.radius.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onEditingFinished: {
                        if (mprop && mprop.circle) {
                            var val = parseFloat(text)
                            if (!isNaN(val)) mprop.circle.radius = val
                        }
                    }
                }
            }
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
