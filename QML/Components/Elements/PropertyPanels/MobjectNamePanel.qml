import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Name Input Panel - Object name editing
ColumnLayout {
    id: root
    
    property var mprop: null
    spacing: 6

    Text {
        text: "Name"
        color: "#bbbbbb"
        font.pixelSize: 13
    }

    StyledTextField {
        Layout.fillWidth: true
        placeholderText: "Enter name"
        text: root.mprop && root.mprop.base ? root.mprop.base.name : ""
        onEditingFinished: {
            if (root.mprop && root.mprop.base)
                root.mprop.base.name = text;
        }
    }
}