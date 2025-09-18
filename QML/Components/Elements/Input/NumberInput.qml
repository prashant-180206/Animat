// NumberInput.qml - Reusable number input component
import QtQuick 2.15
import QtQuick.Controls.Basic

Row {
    property string label: ""
    property real value: 0
    property int decimals: 2
    property bool integersOnly: false
    property real newValue: value
    property var func: ()=>{}

    spacing: 8

    Text {
        text: label
        color: "#bbbbbb"
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 13
    }

    StyledTextField {
        width: 70
        inputMethodHints: integersOnly ? Qt.ImhDigitsOnly : Qt.ImhFormattedNumbersOnly
        text: integersOnly ? value.toString() : value.toFixed(decimals)

        onEditingFinished: {
            newValue = integersOnly ? parseInt(text) : parseFloat(text)
            if (!isNaN(newValue)) {
                func()
            }
        }
    }
}
