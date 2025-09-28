import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0
import "Input"

Rectangle {
    id: root
    property MProperties mprop: canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : canvas.getProperties()

    color: "#121212"
    radius: 8
    border.color: "#2a2a2a"
    border.width: 1

    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 16

            // ID Display
            Rectangle {
                visible: canvas.activeId
                Layout.fillWidth: true
                height: 36
                color: "#1e1e1e"
                radius: 6
                border.color: "#444444"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "ID: " + (canvas.activeId || "")
                        color: "#ffffff"
                        font.pixelSize: 13
                        font.bold: true
                        Layout.fillWidth: true
                    }

                    Button {
                        text: "Delete"
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 24
                        background: Rectangle {
                            color: parent.pressed ? "#b71c1c" : parent.hovered ? "#d32f2f" : "#c62828"
                            radius: 4
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "#ffffff"
                            font.pixelSize: 10
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            if (canvas.activeId) {
                                canvas.removeMobject(canvas.activeId);
                            }
                        }
                    }
                }
            }

            // Name Input
            ColumnLayout {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text: "Name"
                    color: "#bbbbbb"
                    font.pixelSize: 13
                }

                StyledTextField {
                    Layout.fillWidth: true
                    placeholderText: "Enter name"
                    text: mprop && mprop.base ? mprop.base.name : ""
                    onEditingFinished: {
                        if (mprop && mprop.base)
                            mprop.base.name = text;
                    }
                }
            }

            // Position
            ColumnLayout {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text: "Position"
                    color: "#bbbbbb"
                    font.pixelSize: 13
                }

                PointInput {
                    Layout.fillWidth: true
                    pt: mprop && mprop.base ? mprop.base.pos : Qt.point(0, 0)
                    func: function () {
                        if (mprop && mprop.base)
                            mprop.base.pos = pt2;
                    }
                }
            }

            // Size (not for Line/Curve)
            ColumnLayout {
                visible: mprop && mprop.base && mprop.type !== "Line" && mprop.type !== "Curve"
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text: "Size"
                    color: "#bbbbbb"
                    font.pixelSize: 13
                }

                PointInput {
                    Layout.fillWidth: true
                    pt: mprop && mprop.base ? mprop.base.size : Qt.point(0, 0)
                    func: function () {
                        if (mprop && mprop.base)
                            mprop.base.size = pt2;
                    }
                }
            }

            // Colors Section
            GridLayout {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                columns: 2
                rowSpacing: 12
                columnSpacing: 16

                Text {
                    text: "Fill Color"
                    color: "#bbbbbb"
                    font.pixelSize: 13
                }

                ColorPicker {
                    selectedColor: mprop && mprop.base ? mprop.base.color : "transparent"
                    func: () => {
                        if (mprop && mprop.base)
                            mprop.base.color = newColor;
                    }
                }

                Text {
                    visible: mprop && mprop.polygon
                    text: "Border Color"
                    color: "#bbbbbb"
                    font.pixelSize: 13
                }

                ColorPicker {
                    visible: mprop && mprop.polygon
                    selectedColor: mprop && mprop.polygon ? mprop.polygon.borderColor : "transparent"
                    func: () => {
                        if (mprop && mprop.polygon)
                            mprop.polygon.borderColor = newColor;
                    }
                }
            }

            // Numeric Properties
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                NumberInput {
                    visible: mprop && mprop.base
                    label: "Opacity:"
                    value: mprop && mprop.base ? mprop.base.opacity : 1.0
                    func: () => {
                        if (mprop && mprop.base)
                            mprop.base.opacity = newValue;
                    }
                }

                NumberInput {
                    visible: mprop && mprop.polygon
                    label: "Thickness:"
                    value: mprop && mprop.polygon ? mprop.polygon.thickness : 0
                    func: () => {
                        if (mprop && mprop.polygon)
                            mprop.polygon.thickness = newValue;
                    }
                }

                NumberInput {
                    visible: mprop && mprop.circle
                    label: "Radius:"
                    value: mprop && mprop.circle ? mprop.circle.radius : 0
                    func: () => {
                        if (mprop && mprop.circle)
                            mprop.circle.radius = newValue;
                    }
                }

                NumberInput {
                    visible: mprop && (mprop.type === "Curve" || mprop.type === "Circle")
                    label: "Segments:"
                    value: mprop && mprop.curve ? mprop.curve.segments : 0
                    integersOnly: true
                    func: () => {
                        if (mprop && mprop.curve)
                            mprop.curve.segments = newValue;
                    }
                }
            }

            // Line Points
            ColumnLayout {
                visible: mprop && mprop.line
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Line Points"
                    color: "#ffffff"
                    font.pixelSize: 14
                    font.bold: true
                }

                PointInput {
                    pt: mprop && mprop.line ? mprop.line.lineStart : Qt.point(0, 0)
                    label: "Start:"
                    func: function () {
                        if (mprop && mprop.line)
                            mprop.line.lineStart = pt2;
                    }
                }

                PointInput {
                    pt: mprop && mprop.line ? mprop.line.lineEnd : Qt.point(0, 0)
                    label: "End:"
                    func: function () {
                        if (mprop && mprop.line)
                            mprop.line.lineEnd = pt2;
                    }
                }
            }

            // Curve Functions
            ColumnLayout {
                visible: mprop && mprop.curve
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Curve Functions"
                    color: "#ffffff"
                    font.pixelSize: 14
                    font.bold: true
                }

                StyledTextField {
                    Layout.fillWidth: true
                    placeholderText: "X Function (e.g., t * cos(t))"
                    text: mprop && mprop.curve ? mprop.curve.curveXFunc : ""
                    onEditingFinished: {
                        if (mprop && mprop.curve)
                            mprop.curve.curveXFunc = text;
                    }
                }

                StyledTextField {
                    Layout.fillWidth: true
                    placeholderText: "Y Function (e.g., t * sin(t))"
                    text: mprop && mprop.curve ? mprop.curve.curveYFunc : ""
                    onEditingFinished: {
                        if (mprop && mprop.curve)
                            mprop.curve.curveYFunc = text;
                    }
                }

                PointInput {
                    pt: mprop && mprop.curve ? mprop.curve.tRange : Qt.point(0, 1)
                    label: "t Range:"
                    func: function () {
                        if (mprop && mprop.curve)
                            mprop.curve.tRange = pt2;
                    }
                }
            }

            // Text Properties
            ColumnLayout {
                visible: mprop && mprop.text
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Text Properties"
                    color: "#ffffff"
                    font.pixelSize: 14
                    font.bold: true
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Text Value"
                        color: "#bbbbbb"
                        font.pixelSize: 13
                    }

                    StyledTextField {
                        Layout.fillWidth: true
                        placeholderText: "Enter text content"
                        text: mprop && mprop.text ? mprop.text.textValue : ""
                        onEditingFinished: {
                            if (mprop && mprop.text)
                                mprop.text.textValue = text;
                        }
                    }
                }

                NumberInput {
                    label: "Font Size:"
                    value: mprop && mprop.text ? mprop.text.fontSize : 24
                    integersOnly: true
                    func: () => {
                        if (mprop && mprop.text)
                            mprop.text.fontSize = newValue;
                    }
                }

                NumberInput {
                    label: "Font Weight:"
                    value: mprop && mprop.text ? mprop.text.fontWeight : 50
                    integersOnly: true
                    func: () => {
                        if (mprop && mprop.text)
                            mprop.text.fontWeight = newValue;
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Font Family"
                        color: "#bbbbbb"
                        font.pixelSize: 13
                    }

                    StyledTextField {
                        Layout.fillWidth: true
                        placeholderText: "Enter font family"
                        text: mprop && mprop.text ? mprop.text.fontFamily : ""
                        onEditingFinished: {
                            if (mprop && mprop.text)
                                mprop.text.fontFamily = text;
                        }
                    }
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    rowSpacing: 12
                    columnSpacing: 16

                    Text {
                        text: "Text Color"
                        color: "#bbbbbb"
                        font.pixelSize: 13
                    }

                    ColorPicker {
                        selectedColor: mprop && mprop.text ? mprop.text.textColor : "white"
                        func: () => {
                            if (mprop && mprop.text)
                                mprop.text.textColor = newColor;
                        }
                    }

                    CheckBox {
                        text: "Bold"
                        checked: mprop && mprop.text ? mprop.text.bold : false
                        onCheckedChanged: {
                            if (mprop && mprop.text)
                                mprop.text.bold = checked;
                        }
                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 13
                            color: "#bbbbbb"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                        indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            x: parent.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 3
                            border.color: "#555555"
                            color: parent.checked ? "#4CAF50" : "transparent"

                            Rectangle {
                                width: 6
                                height: 6
                                anchors.centerIn: parent
                                radius: 1
                                color: "white"
                                visible: parent.parent.checked
                            }
                        }
                    }

                    CheckBox {
                        text: "Italic"
                        checked: mprop && mprop.text ? mprop.text.italic : false
                        onCheckedChanged: {
                            if (mprop && mprop.text)
                                mprop.text.italic = checked;
                        }
                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 13
                            color: "#bbbbbb"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                        indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            x: parent.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 3
                            border.color: "#555555"
                            color: parent.checked ? "#4CAF50" : "transparent"

                            Rectangle {
                                width: 6
                                height: 6
                                anchors.centerIn: parent
                                radius: 1
                                color: "white"
                                visible: parent.parent.checked
                            }
                        }
                    }
                }
            }
        }
    }
}
