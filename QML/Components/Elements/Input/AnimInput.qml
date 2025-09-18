import QtQuick
import QtQuick.Controls
import Animat 1.0   // your registered module

Item {
    id: root
    property AnimManager packet   // the packet we add animations into
    width: 300
    height: 400

    Column {
        spacing: 10
        anchors.centerIn: parent

        ComboBox {
            id: typeBox
            model: ["Move", "Create", "Destroy", "CustomScalar", "CustomPoint", "Value"]
            currentIndex: 0
            width: 200
        }
        TextField {
            id: propField
            placeholderText: "Property name (if needed)"
            width: 200
        }
        TextField {
            id: mobjField
            placeholderText: "Mobject Id name (if needed)"
            width: 200
        }
        TextField {
            id: startValField
            placeholderText: "Start value (number or x,y)"
            width: 200
        }
        TextField {
            id: targetValField
            placeholderText: "Target value (number or x,y)"
            width: 200
        }
        TextField {
            id: startOffsetField
            placeholderText: "Start offset"
            text: "0"
            width: 200
        }
        TextField {
            id: durationField
            placeholderText: "Duration"
            text: "1"
            width: 200
        }
        Button {
            text: "Add Animation"
            width: 200
            onClicked: {
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

        TextArea {
            id: logArea
            width: 300
            height: 100
            readOnly: true
            wrapMode: TextArea.Wrap
            font.family: "monospace"
        }
    }
}
