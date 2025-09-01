import QtQuick
import Animat 1.0
import QtQuick.Controls 2.15
import "Components"

Window {
    id:globeroot
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Animat")
    color: Constants.darkBlack
    TitleBar {
        id: title
    }
    Taskbar {
        id: taskbar
    }
    Menubar {
        id: menu

    }
    ControlPanel {
        id: control
    }
    Rectangle {

        anchors.top: taskbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: menu.right
        anchors.right: control.left
        color: Constants.darkBlack

        SceneManager {
            id: scenemanager
        }

        Rectangle {
            anchors.top: scenemanager.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: Constants.darkBlack



            Scene {
                id: canvas

                objectName: "canvas"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 40
            }
        }
    }
}
