import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// MPolygon Point Management Panel - Add, remove, edit polygon points
Column {
    id: root

    property var mprop: null
    property var canvasRef: null  // Pass canvas reference from parent

    visible: root.mprop && root.mprop.base && root.mprop.base.type === "MPolygon"
    width: parent ? parent.width : 0
    spacing: 8

    Text {
        text: "Polygon Points"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    Row {
        spacing: 8

        Button {
            id: addButton
            text: "Add Point"
            width: 80
            height: 25
            background: Rectangle {
                color: "#4CAF50"
                radius: 4
            }
            contentItem: Text {
                text: addButton.text
                color: "white"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                if (root.canvasRef && root.canvasRef.SelectedMobject && root.canvasRef.SelectedMobject.addPoint) {
                    // Add point at center of current bounds
                    root.canvasRef.SelectedMobject.addPoint(5, 1);
                }
            }
        }

        Button {
            id: removeButton
            text: "Remove Last"
            width: 80
            height: 25
            background: Rectangle {
                color: "#f44336"
                radius: 4
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                if (canvas.SelectedMobject && canvas.SelectedMobject.removePoint && canvas.SelectedMobject.pointCount() > 3) {
                    var lastIndex = canvas.SelectedMobject.pointCount() - 1;
                    canvas.SelectedMobject.removePoint(lastIndex);
                }
            }
        }

        Button {
            text: "Clear"
            width: 60
            height: 25
            background: Rectangle {
                color: "#FF9800"
                radius: 4
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                if (canvas.SelectedMobject && canvas.SelectedMobject.clearPoints) {
                    canvas.SelectedMobject.clearPoints();
                }
            }
        }
    }

    // Point coordinates display and editing
    ScrollView {
        width: parent.width
        height: Math.min(200, pointsList.count * 35)
        clip: true

        ListView {
            id: pointsList
            model: canvas.SelectedMobject && root.mprop && root.mprop.base && root.mprop.base.type === "MPolygon" ? canvas.SelectedMobject.pointCount() : 0
            delegate: Row {
                spacing: 4
                property int pointIndex: index

                Text {
                    text: "P" + (index + 1) + ":"
                    color: "#bbbbbb"
                    font.pixelSize: 10
                    width: 25
                    anchors.verticalCenter: parent.verticalCenter
                }

                StyledTextField {
                    id: xInput
                    width: 50
                    height: 20
                    font.pixelSize: 9
                    text: canvas.SelectedMobject ? canvas.SelectedMobject.getPoint(pointIndex).x.toFixed(1) : "0"
                    validator: DoubleValidator {
                        bottom: -9999
                        top: 9999
                        decimals: 2
                    }
                    onEditingFinished: {
                        if (canvas.SelectedMobject && canvas.SelectedMobject.movePoint) {
                            var currentPoint = canvas.SelectedMobject.getPoint(pointIndex);
                            canvas.SelectedMobject.movePoint(pointIndex, parseFloat(text), currentPoint.y);
                        }
                    }
                }

                Text {
                    text: ","
                    color: "#bbbbbb"
                    font.pixelSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                }

                StyledTextField {
                    id: yInput
                    width: 50
                    height: 20
                    font.pixelSize: 9
                    text: canvas.SelectedMobject ? canvas.SelectedMobject.getPoint(pointIndex).y.toFixed(1) : "0"
                    validator: DoubleValidator {
                        bottom: -9999
                        top: 9999
                        decimals: 2
                    }
                    onEditingFinished: {
                        if (canvas.SelectedMobject && canvas.SelectedMobject.movePoint) {
                            var currentPoint = canvas.SelectedMobject.getPoint(pointIndex);
                            canvas.SelectedMobject.movePoint(pointIndex, currentPoint.x, parseFloat(text));
                        }
                    }
                }

                Button {
                    text: "×"
                    width: 20
                    height: 20
                    background: Rectangle {
                        color: "#f44336"
                        radius: 2
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pixelSize: 8
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        if (canvas.SelectedMobject && canvas.SelectedMobject.removePoint && canvas.SelectedMobject.pointCount() > 3) {
                            canvas.SelectedMobject.removePoint(pointIndex);
                        }
                    }
                }
            }
        }
    }
}
