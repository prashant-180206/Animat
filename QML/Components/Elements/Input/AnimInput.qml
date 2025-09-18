import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0

Item {
    id: root
    width: 320
    height: 420

    property color backgroundColor: "#121212"
    property color inputBackgroundColor: "#1e1e1e"
    property color inputTextColor: "#ffffff"
    property color labelColor: "#bbbbbb"
    property color borderColor: "#444444"
    property color buttonColor: "#2a2a2a"
    property color buttonTextColor: "#eeeeee"
    property var packet

    Rectangle {
        anchors.fill: parent
        color: backgroundColor
        radius: 6
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        // ComboBox for animation type
        ComboBox {
            id: typeBox
            model: ["Move", "Create", "Destroy", "CustomScalar", "CustomPoint", "Value"]
            currentIndex: 0
            Layout.fillWidth: true
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14

        }

        // Styled TextFields with consistent background and border
        // function styledTextField(id, placeholder) {
        //     return (TextField {
        //         id: id
        //         placeholderText: placeholder
        //         Layout.fillWidth: true
        //         color: inputTextColor
        //         background: Rectangle {
        //             color: inputBackgroundColor
        //             radius: 4
        //             border.color: borderColor
        //         }
        //         font.pixelSize: 14
        //     })
        // }

        // Property name
        TextField {
            id: propField
            placeholderText: "Property name (if needed)"
            Layout.fillWidth: true
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
        }

        // Mobject Id name
        TextField {
            id: mobjField
            placeholderText: "Mobject Id name (if needed)"
            Layout.fillWidth: true
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
        }

        // Start value
        TextField {
            id: startValField
            placeholderText: "Start value (number or x,y)"
            Layout.fillWidth: true
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
        }

        // Target value
        TextField {
            id: targetValField
            placeholderText: "Target value (number or x,y)"
            Layout.fillWidth: true
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
        }

        // Start offset
        TextField {
            id: startOffsetField
            placeholderText: "Start offset"
            text: "0"
            Layout.fillWidth: true
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
        }

        // Duration
        TextField {
            id: durationField
            placeholderText: "Duration"
            text: "1"
            Layout.fillWidth: true
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 4
                border.color: borderColor
            }
            font.pixelSize: 14
        }

        // Add Animation Button - styled consistently
        Button {
            text: "Add Animation"
            Layout.fillWidth: true
            background: Rectangle {
                radius: 6
                color: buttonColor
                border.color: borderColor
            }
            contentItem: Text {
                text: "Add Animation"
                color: buttonTextColor
                font.pixelSize: 14
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
            }

            onClicked: {
                logArea.text=""
                logArea.text += "Add Animation clicked\n"
                let type = typeBox.currentText
                let prop = propField.text
                let startOffset = parseFloat(startOffsetField.text) || 0
                let duration = parseFloat(durationField.text) || 1
                function parseVal(txt) {
                    if (txt.indexOf(",") !== -1) {
                        let parts = txt.split(",")
                        return Qt.point(parseFloat(parts[0]), parseFloat(parts[1]))
                    }
                    return parseFloat(txt)
                }
                let startVal = startValField.text !== "" ? parseVal(startValField.text) : undefined
                let targetVal = targetValField.text !== "" ? parseVal(targetValField.text) : undefined
                logArea.text += `Type: ${type}, prop: ${prop}, startOffset: ${startOffset}, duration: ${duration}\n`
                console.log("Add Animation clicked")
                console.log("Type:", type, "Property:", prop, "Start Offset:", startOffset, "Duration:", duration)
                if (root.packet) {
                    let mobj = null
                    if (typeof canvas !== "undefined" && canvas.getMobject) {
                        mobj = canvas.getMobject(mobjField.text)
                        if (!mobj) {
                            logArea.text += `Warning: Mobject "${mobjField.text}" not found\n`
                            console.log(`Warning: Mobject "${mobjField.text}" not found`)
                            return
                        }
                    } else {
                        logArea.text += "Warning: canvas or getMobject() not available\n"
                        console.log("Warning: canvas or getMobject() not available")
                        return
                    }
                    root.packet.packetToAdd.addAnimation(type, mobj, startVal, targetVal, prop, startOffset, duration)
                    logArea.text += "Added animation to packetToAdd\n"
                    root.packet.add()
                    logArea.text += "Called add() on packet manager\n"
                    console.log("Animation added, packet added.")
                } else {
                    logArea.text += "Error: root.packet is not set\n"
                    console.log("Error: root.packet is not set")
                }
            }
        }

        // Log area to show messages
        TextArea {
            id: logArea
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            readOnly: true
            wrapMode: TextArea.Wrap
            font.family: "monospace"
            color: inputTextColor
            background: Rectangle {
                color: inputBackgroundColor
                radius: 6
                border.color: borderColor
            }
        }
    }
}
