import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0

Rectangle {

    color: Constants.darkGrayA
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.margins: 40
    height: 150

    TrackerManager {
        id: trackerManager
    }

    Component.onCompleted: {
        trackerManager.parse("val qwer = 100\npval vaort = (50, 75)");
        // infoText.text = trackerManager.getTrackerInfoString()
        // console.log(trackerManager.getTrackerInfoString())
    }

    Text {
        id: infoText
        color: "white"
        anchors.centerIn: parent
        text: "qwsdfg"
        wrapMode: Text.WordWrap
    }
}

// PlaybackSlider {
//     id: playbackSlider
//     anchors.horizontalCenter: parent.horizontalCenter
//     width: parent.width * 0.8
//     onValueChanged: slider.value = value
// }

// Column {
//     spacing: 20
//     anchors.centerIn: parent
//     width: parent.width

//     Slider {
//         id: slider
//         from: 0
//         to: playbackSlider.maxDuration
//         value: 0
//         width: parent.width * 0.8
//         stepSize: 10
//         onMoved: {
//             playbackSlider.pause()
//             playbackSlider.value = value
//         }
//     }

//     Row {
//         spacing: 10
//         anchors.horizontalCenter: parent.horizontalCenter

//         Button {
//             text: playbackSlider.playing ? "Pause" : "Play"
//             onClicked: {
//                 if (playbackSlider.playing) {
//                     playbackSlider.pause()
//                 } else {
//                     playbackSlider.play()
//                 }
//             }
//         }

//         Button {
//             text: "Reset"
//             onClicked: playbackSlider.reset()
//         }
//     }

//     Text {
//         text: "Position: " + Math.floor(playbackSlider.value / 1000) + " sec / 60 sec"
//         font.pointSize: 14
//         horizontalAlignment: Text.AlignHCenter
//         width: parent.width
//     }
// }
