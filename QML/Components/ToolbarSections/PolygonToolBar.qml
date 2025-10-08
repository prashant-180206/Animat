import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../../Components/Elements/Input"
import Animat 1.0

RowLayout {
    id: root
    property MProperties mprop: null
    property var canvas: null
    spacing: 10
    visible: mprop && mprop.base && mprop.base.type === "MPolygon" && mprop.polygon
    height: 32

    // Label & point count
    Rectangle {
        color: "#25262a"; radius: 8
        border.color: "#333"
        height: 32
        width: 115
        RowLayout {
            anchors.fill: parent
            Text {
                text: "Points:"
                color: "#fff"
                font.pixelSize: 13
                font.bold: true
                Layout.preferredWidth: 60
            }
            Text {
                anchors.margins: 20
                text: (canvas && canvas.SelectedMobject &&
                       mprop && mprop.polygon &&
                       typeof canvas.SelectedMobject.pointCount === "function")
                      ? canvas.SelectedMobject.pointCount()
                      : ""
                color: "#80ff80"
                font.pixelSize: 13
                font.bold: true
                Layout.preferredWidth: 30
            }

        }
    }

    // Actions: Add/Remove/Clear menu
    Button {
        text: "+"
        width: 28; height: 28
        background: Rectangle { color: "#4CAF50"; radius: 6 }
        contentItem: Text {
            text: "+"
            font.pixelSize: 18
            color: "#fff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        onClicked: actionsMenu.open()
    }
    Menu {
        parent: root
        x: 40
        y: 20
        id: actionsMenu

        property bool isPolygon: canvas && canvas.SelectedMobject && mprop && mprop.polygon

        MenuItem {
            text: "Add Point"
            enabled: actionsMenu.isPolygon==undefined && canvas.SelectedMobject.addPoint
            onTriggered: {
                if (actionsMenu.isPolygon && canvas.SelectedMobject.addPoint)
                    canvas.SelectedMobject.addPoint(5, 1)
            }
        }

        MenuItem {
            text: "Remove Last"
            enabled: actionsMenu.isPolygon==undefined &&
                     canvas.SelectedMobject.pointCount &&
                     canvas.SelectedMobject.removePoint &&
                     canvas.SelectedMobject.pointCount() > 3

            onTriggered: {
                if (actionsMenu.isPolygon &&
                        canvas.SelectedMobject.removePoint &&
                        canvas.SelectedMobject.pointCount &&
                        canvas.SelectedMobject.pointCount() > 3
                        ) {
                    let lastIndex = canvas.SelectedMobject.pointCount() - 1
                    canvas.SelectedMobject.removePoint(lastIndex)
                }
            }
        }

        MenuItem {
            text: "Clear All"
            enabled: actionsMenu.isPolygon==undefined&&
                     canvas.SelectedMobject.clearPoints &&
                     canvas.SelectedMobject.pointCount &&
                     canvas.SelectedMobject.pointCount() > 0

            onTriggered: {
                if (actionsMenu.isPolygon==undefined &&
                        canvas.SelectedMobject.clearPoints &&
                        canvas.SelectedMobject.pointCount
                        ) {
                    canvas.SelectedMobject.clearPoints()
                }
            }
        }
    }


    // Edit menu (like a grid)
    Button {
        text: "✎"
        width: 28; height: 28
        background: Rectangle { color: "#888"; radius: 6 }
        contentItem: Text {
            text: "✎"
            font.pixelSize: 15
            color: "#fff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        onClicked: pointsMenu.open()
    }
    Menu {
        parent: root
        x:40
        y:20
        id: pointsMenu
        width: 240
        background: Rectangle { color: "#23272e"; radius: 12 }
        Column {
            visible: mprop && mprop.polygon
            spacing: 7
            padding: 10
            Repeater {
                model: (canvas && canvas.SelectedMobject && canvas.SelectedMobject.pointCount)
                       ? canvas.SelectedMobject.pointCount()
                       : 0

                delegate: Row {
                    spacing: 5
                    property int idx: index

                    Text {
                        text: "P" + (idx + 1) + ":"
                        color: "#96c"
                        font.pixelSize: 11
                    }

                    StyledTextField {
                        width: 45
                        font.pixelSize: 10
                        text: (canvas.SelectedMobject && canvas.SelectedMobject.getPoint)
                              ? canvas.SelectedMobject.getPoint(idx).x.toFixed(1)
                              : "0"
                        validator: DoubleValidator { bottom: -9999; top: 9999; decimals: 2 }
                        onEditingFinished: {
                            if (canvas.SelectedMobject && canvas.SelectedMobject.movePoint && mprop.polygon) {
                                let pt = canvas.SelectedMobject.getPoint(idx)
                                canvas.SelectedMobject.movePoint(idx, parseFloat(text), pt.y)
                            }
                        }
                    }

                    Text { text: ","; color: "#bbb"; font.pixelSize: 11 }

                    StyledTextField {
                        width: 45
                        font.pixelSize: 10
                        text: (canvas.SelectedMobject && canvas.SelectedMobject.getPoint)
                              ? canvas.SelectedMobject.getPoint(idx).y.toFixed(1)
                              : "0"
                        validator: DoubleValidator { bottom: -9999; top: 9999; decimals: 2 }
                        onEditingFinished: {
                            if (canvas.SelectedMobject && canvas.SelectedMobject.movePoint && mprop.polygon) {
                                let pt = canvas.SelectedMobject.getPoint(idx)
                                canvas.SelectedMobject.movePoint(idx, pt.x, parseFloat(text))
                            }
                        }
                    }

                    Button {
                        text: "×"
                        width: 18; height: 18
                        background: Rectangle { color: "#f44336"; radius: 4 }
                        contentItem: Text {
                            text: "×"
                            color: "#fff"
                            font.pixelSize: 8
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        enabled: (canvas.SelectedMobject &&
                                  canvas.SelectedMobject.pointCount &&
                                  canvas.SelectedMobject.pointCount() > 3)

                        onClicked: {
                            if (canvas.SelectedMobject &&
                                    canvas.SelectedMobject.removePoint &&
                                    canvas.SelectedMobject.pointCount &&
                                    canvas.SelectedMobject.pointCount() > 3 &&
                                    mprop.polygon
                                    ) {
                                canvas.SelectedMobject.removePoint(idx)
                            }
                        }
                    }
                }
            }

        }
    }
}
