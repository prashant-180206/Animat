// MSlider.qml
import QtQuick 2.15
import QtQuick.Controls.Basic

Slider {
    id: valueSlider
    property real minValue: 0
    property real maxValue: 100
    property real currentValue: 0

    from: minValue
    to: maxValue
    value: currentValue
    width: 200
    height: 40

    signal valueUpdated(real val)

    background: Rectangle {
        x: valueSlider.leftPadding
        y: valueSlider.topPadding + valueSlider.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 4
        width: valueSlider.availableWidth
        height: implicitHeight
        radius: 2
        color: "#444"

        Rectangle {
            width: valueSlider.visualPosition * parent.width
            height: parent.height
            color: "#4a9eff"
            radius: 2
        }
    }

    handle: Rectangle {
        x: valueSlider.leftPadding + valueSlider.visualPosition * (valueSlider.availableWidth - width)
        y: valueSlider.topPadding + valueSlider.availableHeight / 2 - height / 2
        implicitWidth: 16
        implicitHeight: 16
        radius: 8
        color: valueSlider.pressed ? "#6bb6ff" : "#4a9eff"
        border.color: "#fff"
        border.width: 1
    }

    onValueChanged: {
        if (Math.abs(value - currentValue) > 0.001) {
            currentValue = value
            valueUpdated(value)
        }
    }
}
