import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import Animat 1.0

Item {
    id: root
    property var mprop : canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : null

    // Component.onCompleted: {
    //     mprop = canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : null
    //     if (!mprop) {
    //         mprop = Qt.createQmlObject('import Animat 1.0; MProperties {}', root)
    //         // or get a suitable default MProperties instance here
    //     }
    // }

    // Dark theme colors as properties for easy change
    property color backgroundColor: "#121212"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color labelColor: "#bbbbbb"
    property color borderColor: "#444444"
    property color buttonColor: "#2a2a2a"
    property color buttonTextColor: "#eeeeee"

    Rectangle {
        anchors.fill: parent
        color: backgroundColor
        radius: 6
    }

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        // Name: string
        TextField {
            placeholderText: "Name"
            text: mprop ? (mprop.name || "") : ""
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            onTextChanged: if(mprop) mprop.name = text
            visible: mprop !== null
        }

        // Pos: QPointF {x, y}
        Row {
            visible: mprop !== null
            spacing: 4
            Label {
                text: "Pos:"
                color: labelColor
                width: 30
            }
            TextField {
                width: 50
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.pos && mprop.pos.x !== undefined) ? mprop.pos.x.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onTextChanged: {
                    if (!mprop) return;
                    var x = parseFloat(text)
                    if (!isNaN(x)) {
                        var y = (mprop.pos && mprop.pos.y !== undefined) ? mprop.pos.y : 0
                        mprop.pos = Qt.point(x, y)
                    }
                }
            }
            TextField {
                width: 50
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.pos && mprop.pos.y !== undefined) ? mprop.pos.y.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onTextChanged: {
                    if (!mprop) return;
                    var y = parseFloat(text)
                    if (!isNaN(y)) {
                        var x = (mprop.pos && mprop.pos.x !== undefined) ? mprop.pos.x : 0
                        mprop.pos = Qt.point(x, y)
                    }
                }
            }
        }

        // Size: QPair<qreal, qreal> {first, second}
        Row {
            visible: mprop !== null
            spacing: 4
            Label {
                text: "Size:"
                color: labelColor
                width: 30
            }
            TextField {
                width: 50
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.size && mprop.size.first !== undefined) ? mprop.size.first.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onTextChanged: {
                    if (!mprop || !mprop.size) return;
                    var first = parseFloat(text)
                    if (!isNaN(first)) {
                        var second = (mprop.size.second !== undefined) ? mprop.size.second : 0
                        // Cannot assign plain object if C++ expects std::pair<double,double>
                        // Use a setter method if available, else try mutating copy if allowed
                        if (mprop.setSize) {
                            mprop.setSize(first, second)
                        } else {
                            // fallback (may not work if property enforces std::pair)
                            mprop.size = { first: first, second: second }
                        }
                    }
                }
            }
            TextField {
                width: 50
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.size && mprop.size.second !== undefined) ? mprop.size.second.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onTextChanged: {
                    if (!mprop || !mprop.size) return;
                    var second = parseFloat(text)
                    if (!isNaN(second)) {
                        var first = (mprop.size.first !== undefined) ? mprop.size.first : 0
                        if (mprop.setSize) {
                            mprop.setSize(first, second)
                        } else {
                            mprop.size = { first: first, second: second }
                        }
                    }
                }
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
            visible: mprop !== null
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
            visible: mprop !== null
            spacing: 4
            Label { text: "Line Points:"; color: labelColor }
            Row {
                spacing: 4
                Label { text: "Start"; color: labelColor }
                TextField {
                    width: 50
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: (mprop && mprop.linePoints && mprop.linePoints.first && mprop.linePoints.first.x !== undefined)
                        ? mprop.linePoints.first.x.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onTextChanged: {
                        if (!mprop || !mprop.linePoints || !mprop.linePoints.first) return;
                        var x = parseFloat(text)
                        if (!isNaN(x)) {
                            var lp = mprop.linePoints
                            var y = lp.first.y
                            var second = lp.second || Qt.point(0,0)
                            mprop.setLinePoints(Qt.point(x, y), second)
                        }
                    }
                }
                TextField {
                    width: 50
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: (mprop && mprop.linePoints && mprop.linePoints.first && mprop.linePoints.first.y !== undefined)
                        ? mprop.linePoints.first.y.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onTextChanged: {
                        if (!mprop || !mprop.linePoints || !mprop.linePoints.first) return;
                        var y = parseFloat(text)
                        if (!isNaN(y)) {
                            var lp = mprop.linePoints
                            var x = lp.first.x
                            var second = lp.second || Qt.point(0,0)
                            mprop.setLinePoints(Qt.point(x, y), second)
                        }
                    }
                }
            }
            Row {
                spacing: 4
                Label { text: "End"; color: labelColor }
                TextField {
                    width: 50
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: (mprop && mprop.linePoints && mprop.linePoints.second && mprop.linePoints.second.x !== undefined)
                        ? mprop.linePoints.second.x.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onTextChanged: {
                        if (!mprop || !mprop.linePoints || !mprop.linePoints.second) return;
                        var x = parseFloat(text)
                        if (!isNaN(x)) {
                            var lp = mprop.linePoints
                            var first = lp.first || Qt.point(0, 0)
                            var y = lp.second.y
                            mprop.setLinePoints(first, Qt.point(x, y))
                        }
                    }
                }
                TextField {
                    width: 50
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: (mprop && mprop.linePoints && mprop.linePoints.second && mprop.linePoints.second.y !== undefined)
                        ? mprop.linePoints.second.y.toFixed(2) : "0.00"
                    color: inputTextColor
                    background: Rectangle {
                        color: inputBackgroundColor
                        radius: 4
                        border.color: borderColor
                    }
                    onTextChanged: {
                        if (!mprop || !mprop.linePoints || !mprop.linePoints.second) return;
                        var y = parseFloat(text)
                        if (!isNaN(y)) {
                            var lp = mprop.linePoints
                            var first = lp.first || Qt.point(0, 0)
                            var x = lp.second.x
                            mprop.setLinePoints(first, Qt.point(x, y))
                        }
                    }
                }
            }
        }

        // Thickness: qreal
        Row {
            visible: mprop !== null
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
                onTextChanged: {
                    if (!mprop) return;
                    var val = parseFloat(text)
                    if (!isNaN(val)) mprop.thickness = val
                }
            }
        }

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
            onTextChanged: if(mprop) mprop.curveXFunc = text
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
            onTextChanged: if(mprop) mprop.curveYFunc = text
        }

        // tRange: QPair<qreal, qreal> {first, second}
        Row {
            visible: mprop !== null
            spacing: 4
            Label { text: "tRange:"; color: labelColor }
            TextField {
                width: 50
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.tRange && mprop.tRange.first !== undefined) ? mprop.tRange.first.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onTextChanged: {
                    if (!mprop) return;
                    var first = parseFloat(text)
                    if (!isNaN(first)) {
                        var second = (mprop.tRange && mprop.tRange.second !== undefined) ? mprop.tRange.second : 0
                        if(mprop.setTRange) {
                            mprop.setTRange(first, second)
                        } else if(mprop.tRange) {
                            mprop.tRange.first = first
                            mprop.tRange.second = second
                        }
                    }
                }
            }
            TextField {
                width: 50
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: (mprop && mprop.tRange && mprop.tRange.second !== undefined) ? mprop.tRange.second.toFixed(2) : "0.00"
                color: inputTextColor
                background: Rectangle {
                    color: inputBackgroundColor
                    radius: 4
                    border.color: borderColor
                }
                onTextChanged: {
                    if (!mprop) return;
                    var second = parseFloat(text)
                    if (!isNaN(second)) {
                        var first = (mprop.tRange && mprop.tRange.first !== undefined) ? mprop.tRange.first : 0
                        if(mprop.setTRange) {
                            mprop.setTRange(first, second)
                        } else if(mprop.tRange) {
                            mprop.tRange.first = first
                            mprop.tRange.second = second
                        }
                    }
                }
            }
        }

        // segments: int
        Row {
            visible: mprop !== null
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
                onTextChanged: {
                    if(!mprop) return;
                    var val = parseInt(text)
                    if(!isNaN(val)) mprop.segments = val
                }
            }
        }

        // endPoints: QList<QPointF> (array of points)
        Column {
            visible: mprop !== null
            spacing: 4
            Label { text: "End Points:"; color: labelColor }
            Repeater {
                model: (mprop && mprop.endPoints) ? mprop.endPoints.length : 0
                Row {
                    spacing: 4
                    TextField {
                        width: 50
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        text: modelData && modelData.x !== undefined ? modelData.x.toFixed(2) : "0.00"
                        color: inputTextColor
                        background: Rectangle {
                            color: inputBackgroundColor
                            radius: 4
                            border.color: borderColor
                        }
                        onEditingFinished: {
                            if (!mprop || !mprop.endPoints) return;
                            var newPoints = mprop.endPoints.slice()
                            var x = parseFloat(text)
                            if (!isNaN(x)) {
                                newPoints[index] = Qt.point(x, newPoints[index].y)
                                mprop.endPoints = newPoints
                            }
                        }
                    }
                    TextField {
                        width: 50
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        text: modelData && modelData.y !== undefined ? modelData.y.toFixed(2) : "0.00"
                        color: inputTextColor
                        background: Rectangle {
                            color: inputBackgroundColor
                            radius: 4
                            border.color: borderColor
                        }
                        onEditingFinished: {
                            if (!mprop || !mprop.endPoints) return;
                            var newPoints = mprop.endPoints.slice()
                            var y = parseFloat(text)
                            if (!isNaN(y)) {
                                newPoints[index] = Qt.point(newPoints[index].x, y)
                                mprop.endPoints = newPoints
                            }
                        }
                    }
                }
            }
            Button {
                text: "Add Point"
                background: Rectangle {
                    color: buttonColor
                    radius: 4
                }
                contentItem: Text {
                    text: control.text || ""
                    anchors.centerIn: parent
                    color: buttonTextColor
                }
                onClicked: {
                    if (!mprop || !mprop.endPoints) return;
                    var newPoints = mprop.endPoints.slice()
                    newPoints.push(Qt.point(0, 0))
                    mprop.endPoints = newPoints
                }
            }
        }

        // radius: qreal
        Row {
            visible: mprop !== null
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
                onTextChanged: {
                    if (!mprop) return;
                    var val = parseFloat(text)
                    if (!isNaN(val)) mprop.radius = val
                }
            }
        }
    }
}
