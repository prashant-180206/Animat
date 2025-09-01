import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15

Rectangle {
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 300

    function mobjectProperties() {
        if (canvas.SelectedMobject === null) {
            return "NO MObject Selected"
        }
        var props = canvas.SelectedMobject
        var str = ""
        for (var key in props) {
            str += key + ": " + props[key] + "\n"
        }
        return str
    }

    Text {
        id: txt
        text: qsTr("Properties") /* mobjectProperties()*/
        font.pointSize: 14
        // font.bold: true
        color: "#5ce1e6"
        y:30
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: rec1
        width: 200
        height: 200
        color: "yellow"
        anchors.top: txt.bottom

        property var props: canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : {}
        property var keys: Object.keys(props)

        Column {
            id: propColumn
            anchors.fill: parent
            spacing: 4

            Repeater {
                model: rec1.keys.length

                delegate: Item {
                    property string key: rec1.keys[index]
                    property var value: rec1.props[key]

                    Row {
                        spacing: 4
                        Text { text: key + ":" }

                        // String input
                        Loader {
                            active: typeof value === "string"
                            sourceComponent: txtin
                        }
                        // Number input
                        Loader {
                            active: typeof value === "number"
                            sourceComponent: numin
                        }
                        // Boolean input
                        Loader {
                            active: typeof value === "boolean"
                            sourceComponent: boolin
                        }
                        // Add more types as needed
                    }

                    // Input field components
                    Component {
                        id: txtin
                        TextField { text: value }
                    }
                    Component {
                        id: numin
                        SpinBox { value: value }
                    }
                    Component {
                        id: boolin
                        CheckBox { checked: value }
                    }
                }
            }
        }
    }



}
