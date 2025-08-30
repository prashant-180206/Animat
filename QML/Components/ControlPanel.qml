import QtQuick 2.15
import Animat 1.0
import QtQuick.Controls 2.15

Rectangle {
    color: Constants.darkGrayA
    anchors.top: taskbar.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 300

    Rectangle {
        id: rec1
        width: 200
        height: 100
        color: "yellow"

        function mobjectProperties() {
            if (canvas.SelectedMobject === null) {
                return "NO MObject Selected"
            }
            var props = canvas.SelectedMobject.getProperties()
            var str = ""
            for (var key in props) {
                str += key + ": " + props[key] + "\n"
            }
            return str
        }

        Text {
            id: test
            text: rec1.mobjectProperties()
            anchors.fill: parent
            wrapMode: Text.Wrap
        }

        Button {
            anchors.top: test.bottom
            text: "Check Active Mobject"
            onClicked: {
                console.log(canvas.SelectedMobject)
                console.log("Props:", canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : "none")
            }
        }
    }
}
