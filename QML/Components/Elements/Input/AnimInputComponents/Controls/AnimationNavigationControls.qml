import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

RowLayout {
    id: root
    spacing: 8

    property var manager
    property color buttonColor: "#2a2a2a"
    property color buttonTextColor: "#eeeeee"
    property color borderColor: "#444444"

    signal playFirstClicked
    signal playPreviousClicked
    signal playNextClicked
    signal playLastClicked

    Button {
        text: "⏮️ First"
        width: 60
        height: 30

        background: Rectangle {
            color: root.buttonColor
            radius: 4
            border.color: root.borderColor
        }
        contentItem: Text {
            text: "⏮️ First"
            color: root.buttonTextColor
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            if (root.manager) {
                root.manager.jumpToFirst();
                root.playFirstClicked();
            }
        }
    }

    Button {
        text: "⏪ Prev"
        width: 60
        height: 30

        background: Rectangle {
            color: root.buttonColor
            radius: 4
            border.color: root.borderColor
        }
        contentItem: Text {
            text: "⏪ Prev"
            color: root.buttonTextColor
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            if (root.manager) {
                root.manager.playPrevious();
                root.playPreviousClicked();
            }
        }
    }

    Button {
        text: "⏩ Next"
        width: 60
        height: 30

        background: Rectangle {
            color: root.buttonColor
            radius: 4
            border.color: root.borderColor
        }
        contentItem: Text {
            text: "⏩ Next"
            color: root.buttonTextColor
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            if (root.manager) {
                root.manager.playNext();
                root.playNextClicked();
            }
        }
    }

    Button {
        text: "⏭️ Last"
        width: 60
        height: 30

        background: Rectangle {
            color: root.buttonColor
            radius: 4
            border.color: root.borderColor
        }
        contentItem: Text {
            text: "⏭️ Last"
            color: root.buttonTextColor
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            if (root.manager) {
                root.manager.jumpToLast();
                root.playLastClicked();
            }
        }
    }

    Text {
        text: root.manager ? `${root.manager.getCurrentIndex() + 1} / ${root.manager.size()}` : "0 / 0"
        color: root.buttonTextColor
        font.pixelSize: 12
        Layout.alignment: Qt.AlignVCenter
    }
}
