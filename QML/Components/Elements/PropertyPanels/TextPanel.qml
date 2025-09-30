import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Input"

// Text Properties Panel - Text content, font settings, and styling
ColumnLayout {
    id: root

    property var mprop: null
    spacing: 12

    Text {
        text: "Text Properties"
        color: "#ffffff"
        font.pixelSize: 14
        font.bold: true
    }

    // Text Content
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 8

        Text {
            text: "Content"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        StyledTextField {
            Layout.fillWidth: true
            placeholderText: "Enter text content"
            text: root.mprop && root.mprop.text ? root.mprop.text.textValue : ""
            onEditingFinished: {
                if (root.mprop && root.mprop.text)
                    root.mprop.text.textValue = text;
            }
        }
    }

    // Font Settings Grid
    GridLayout {
        Layout.fillWidth: true
        columns: 2
        rowSpacing: 12
        columnSpacing: 16

        // Font Size
        Text {
            text: "Font Size"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.text ? root.mprop.text.fontSize : 24
            integersOnly: true
            func: () => {
                if (root.mprop && root.mprop.text)
                    root.mprop.text.fontSize = newValue;
            }
        }

        // Font Family
        Text {
            text: "Font Family"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        StyledTextField {
            Layout.fillWidth: true
            placeholderText: "Font family (e.g., Arial)"
            text: root.mprop && root.mprop.text ? root.mprop.text.fontFamily : ""
            onEditingFinished: {
                if (root.mprop && root.mprop.text)
                    root.mprop.text.fontFamily = text;
            }
        }

        // Font Weight
        Text {
            text: "Font Weight"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        NumberInput {
            Layout.fillWidth: true
            value: root.mprop && root.mprop.text ? root.mprop.text.fontWeight : 50
            integersOnly: true
            func: () => {
                if (root.mprop && root.mprop.text)
                    root.mprop.text.fontWeight = newValue;
            }
        }
    }

    // Font Style Checkboxes
    RowLayout {
        Layout.fillWidth: true
        spacing: 20

        CheckBox {
            id: boldCheck
            text: "Bold"
            checked: root.mprop && root.mprop.text ? root.mprop.text.bold : false
            onCheckedChanged: {
                if (root.mprop && root.mprop.text)
                    root.mprop.text.bold = checked;
            }

            indicator: Rectangle {
                implicitWidth: 20
                implicitHeight: 20
                x: 6
                y: (boldCheck.height - height) / 2
                radius: 4
                color: boldCheck.checked ? "#4a9eff" : "#2a2a2a"
                border.color: boldCheck.checked ? "#4a9eff" : "#555555"
                border.width: 1

                Rectangle {
                    width: 12
                    height: 8
                    x: 4
                    y: 6
                    radius: 2
                    color: "#ffffff"
                    visible: boldCheck.checked
                }
            }

            contentItem: Text {
                text: boldCheck.text
                font.pixelSize: 12
                font.bold: boldCheck.checked
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 30
            }
        }

        CheckBox {
            id: italicCheck
            text: "Italic"
            checked: root.mprop && root.mprop.text ? root.mprop.text.italic : false
            onCheckedChanged: {
                if (root.mprop && root.mprop.text)
                    root.mprop.text.italic = checked;
            }

            indicator: Rectangle {
                implicitWidth: 20
                implicitHeight: 20
                x: 6
                y: (italicCheck.height - height) / 2
                radius: 4
                color: italicCheck.checked ? "#4a9eff" : "#2a2a2a"
                border.color: italicCheck.checked ? "#4a9eff" : "#555555"
                border.width: 1

                Rectangle {
                    width: 12
                    height: 8
                    x: 4
                    y: 6
                    radius: 2
                    color: "#ffffff"
                    visible: italicCheck.checked
                }
            }

            contentItem: Text {
                text: italicCheck.text
                font.pixelSize: 12
                font.italic: italicCheck.checked
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 30
            }
        }
    }

    // Font Family Quick Select (Common fonts)
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 8

        Text {
            text: "Quick Font Select"
            color: "#bbbbbb"
            font.pixelSize: 13
        }

        Row {
            spacing: 8

            Button {
                id: arialBtn
                text: "Arial"
                implicitWidth: 70
                implicitHeight: 28

                background: Rectangle {
                    color: arialBtn.down ? "#4a9eff" : (arialBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: arialBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    font.family: "Arial"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.text)
                        root.mprop.text.fontFamily = "Arial";
                }
            }

            Button {
                id: timesBtn
                text: "Times"
                implicitWidth: 70
                implicitHeight: 28

                background: Rectangle {
                    color: timesBtn.down ? "#4a9eff" : (timesBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: timesBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    font.family: "Times New Roman"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.text)
                        root.mprop.text.fontFamily = "Times New Roman";
                }
            }

            Button {
                id: courierBtn
                text: "Courier"
                implicitWidth: 70
                implicitHeight: 28

                background: Rectangle {
                    color: courierBtn.down ? "#4a9eff" : (courierBtn.hovered ? "#333333" : "#2a2a2a")
                    border.color: "#555555"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: courierBtn.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    font.family: "Courier New"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.mprop && root.mprop.text)
                        root.mprop.text.fontFamily = "Courier New";
                }
            }
        }
    }
}
