// DarkCheckBox.qml
import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

RowLayout {
    id: root
    property string label: "Label"
    property bool checked: false
    signal changed(bool checked)

    spacing: 6
    height: 20

    CheckBox {
        implicitHeight: 16
        implicitWidth: 16
        id: cb
        checked: root.checked
        onCheckedChanged: {
            root.checked = checked;
            root.changed(checked);
        }
        font.pixelSize: 12
        palette {
            text: "#dddddd"
            window: "#222222"
            button: "#333333"
            base: "#222222"
            highlight: "#4444aa"
            highlightedText: "#ffffff"
        }
        indicator: Rectangle {
            implicitWidth: 16
            implicitHeight: 16
            radius: 4
            color: cb.checked ? "#4444aa" : "#333333"
            border.color: "#888888"
            border.width: 1
            Rectangle {
                anchors.centerIn: parent
                width: 8
                height: 8
                radius: 2
                color: cb.checked ? "#ffffff" : "transparent"
                visible: cb.checked
            }
        }
    }

    Text {
        text: root.label
        color: "#dddddd"
        font.pixelSize: 12
        verticalAlignment: Text.AlignVCenter
    }
}
