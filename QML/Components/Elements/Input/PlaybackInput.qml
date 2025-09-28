// PlaybackSlider.qml
import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0

Rectangle {
    id: root
    property var player
    width: 600
    height: 50
    color: "#17181A"
    radius: 10

    // --- Playback Controls (Play/Pause + Reset) ---
    Row {
        id: controls
        spacing: 12
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter

        // Play / Pause
        Button {
            width: 32
            height: 32
            background: Rectangle {
                anchors.fill: parent
                color: "#202127"
                radius: 8
            }
            contentItem: Text {
                text: player && player.playing ? "\u23F8" : "\u25B6"   // ⏸ / ▶
                font.pixelSize: 20
                color: "#F5F6F7"
                anchors.centerIn: parent
            }
            onClicked: {
                if (!player)
                    return;
                player.playing ? player.pause() : player.play();
            }
        }

        // Reset
        Button {
            width: 32
            height: 32
            background: Rectangle {
                anchors.fill: parent
                color: "#202127"
                radius: 8
            }
            contentItem: Text {
                text: "\u21BA"   // ↺
                font.pixelSize: 18
                color: "#F5F6F7"
                anchors.centerIn: parent
            }
            onClicked: if (player)
                player.reset()
        }
    }

    // --- Slider ---
    Slider {
        id: slider
        anchors {
            left: controls.right
            leftMargin: 20
            right: timeLabel.left
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }

        from: 0
        to: player ? player.maxDuration : 5000
        value: player ? player.value : 0
        stepSize: 10

        onPressedChanged: if (!pressed && player)
            player.value = value
        onMoved: if (player) {
            player.pause();
            player.value = value;
        }

        // === Corrected Background (Qt Docs style) ===
        background: Rectangle {
            x: slider.leftPadding
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 200
            implicitHeight: 6
            width: slider.availableWidth
            height: implicitHeight
            radius: 3
            color: "#23242A"   // Track color

            Rectangle {
                width: slider.visualPosition * parent.width
                height: parent.height
                color: "#E25555"   // Progress bar color
                radius: 3
            }
        }

        // === Corrected Handle (Qt Docs style) ===
        handle: Rectangle {
            x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 16
            implicitHeight: 16
            radius: 8
            color: slider.pressed ? "#FF615A" : "#E25555"
            border.color: "#17181A"
            border.width: 2
        }
    }

    // --- Time Display ---
    Text {
        id: timeLabel
        anchors.right: parent.right
        anchors.rightMargin: 18
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 13
        font.family: "Monospace"
        color: "#ADADAD"

        text: {
            function pad(n) {
                return n < 10 ? "0" + n : n;
            }
            let valueSec = player ? Math.floor(player.value / 1000) : 0;
            let maxSec = player ? Math.floor(player.maxDuration / 1000) : 5;

            // Show if duration is based on animations or default
            let durationLabel = (player && player.animationManager && player.animationManager.size() > 0) ? "" : " (no animations)";

            return pad(Math.floor(valueSec / 60)) + ":" + pad(valueSec % 60) + " / " + pad(Math.floor(maxSec / 60)) + ":" + pad(maxSec % 60) + durationLabel;
        }
    }
}
