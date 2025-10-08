import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../Elements"

// TextToolbarSection - Section for text-specific properties
ToolbarSection {
    id: root
    title: "Text"

    property var selectedObject: null

    function updateValues() {
        if (!selectedObject)
            return;

        // Update UI elements from the selected object's properties
        textInput.text = selectedObject.text || "";
        fontSizeInput.value = selectedObject.fontSize || 12;
        fontComboBox.currentIndex = fontComboBox.indexOfValue(selectedObject.fontFamily || "Arial");
        boldButton.isActive = selectedObject.fontBold || false;
        italicButton.isActive = selectedObject.fontItalic || false;
        underlineButton.isActive = selectedObject.fontUnderline || false;
    }

    // Update values when selected object changes
    onSelectedObjectChanged: {
        updateValues();
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        // Text input
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Text {
                text: "Text:"
                color: "#cccccc"
                font.pixelSize: 12
                Layout.preferredWidth: 60
            }

            TextField {
                id: textInput
                placeholderText: "Enter text..."
                Layout.fillWidth: true

                onEditingFinished: {
                    if (root.selectedObject) {
                        root.selectedObject.text = text;
                    }
                }

                background: Rectangle {
                    color: "#333333"
                    border.color: parent.focus ? "#6666ff" : "#555555"
                }

                color: "#ffffff"
                selectByMouse: true
            }
        }

        // Font properties
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Text {
                text: "Font:"
                color: "#cccccc"
                font.pixelSize: 12
                Layout.preferredWidth: 60
            }

            ComboBox {
                id: fontComboBox
                model: ["Arial", "Helvetica", "Times New Roman", "Courier New", "Verdana", "Georgia"]
                Layout.fillWidth: true

                onActivated: {
                    if (root.selectedObject) {
                        root.selectedObject.fontFamily = currentText;
                    }
                }

                background: Rectangle {
                    color: "#333333"
                    border.color: parent.focus ? "#6666ff" : "#555555"
                }

                contentItem: Text {
                    text: fontComboBox.displayText
                    color: "#ffffff"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                popup: Popup {
                    y: fontComboBox.height
                    width: fontComboBox.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: fontComboBox.popup.visible ? fontComboBox.delegateModel : null

                        ScrollBar.vertical: ScrollBar {}
                    }

                    background: Rectangle {
                        color: "#333333"
                        border.color: "#555555"
                    }
                }

                delegate: ItemDelegate {
                    width: fontComboBox.width
                    contentItem: Text {
                        text: modelData
                        color: "#ffffff"
                        font: fontComboBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: fontComboBox.highlightedIndex === index

                    background: Rectangle {
                        color: highlighted ? "#6666ff" : "#333333"
                    }
                }
            }

            Text {
                text: "Size:"
                color: "#cccccc"
                font.pixelSize: 12
            }

            SpinBox {
                id: fontSizeInput
                from: 6
                to: 72
                stepSize: 1
                value: 12
                editable: true
                Layout.preferredWidth: 60

                onValueModified: {
                    if (root.selectedObject) {
                        root.selectedObject.fontSize = value;
                    }
                }

                background: Rectangle {
                    color: "#333333"
                    border.color: parent.focus ? "#6666ff" : "#555555"
                }

                contentItem: TextInput {
                    text: fontSizeInput.value
                    color: "#ffffff"
                    selectByMouse: true
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }
        }

        // Text formatting
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Text {
                text: "Format:"
                color: "#cccccc"
                font.pixelSize: 12
                Layout.preferredWidth: 60
            }

            ToolbarButton {
                id: boldButton
                iconText: "B"
                tooltipText: "Bold"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30

                onClicked: {
                    isActive = !isActive;
                    if (root.selectedObject) {
                        root.selectedObject.fontBold = isActive;
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: "B"
                    color: "#cccccc"
                    font.pixelSize: 14
                    font.bold: true
                }
            }

            ToolbarButton {
                id: italicButton
                iconText: "I"
                tooltipText: "Italic"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30

                onClicked: {
                    isActive = !isActive;
                    if (root.selectedObject) {
                        root.selectedObject.fontItalic = isActive;
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: "I"
                    color: "#cccccc"
                    font.pixelSize: 14
                    font.italic: true
                }
            }

            ToolbarButton {
                id: underlineButton
                iconText: "U"
                tooltipText: "Underline"
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30

                onClicked: {
                    isActive = !isActive;
                    if (root.selectedObject) {
                        root.selectedObject.fontUnderline = isActive;
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: "U"
                    color: "#cccccc"
                    font.pixelSize: 14
                    font.underline: true
                }
            }

            Item {
                Layout.fillWidth: true
            } // Spacer
        }
    }
}
