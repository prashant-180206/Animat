import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts

Column {
    id: root
    spacing: 8

    property color labelColor: "#bbbbbb"
    property color inputTextColor: "#ffffff"
    property color listItemColor: "#333333"
    property color borderColor: "#444444"
    property color deleteButtonColor: "#d32f2f"
    property alias model: animationListModel
    property int count: animationListModel.count

    signal itemRemoved(int index)

    Text {
        text: "Animation List (" + animationListModel.count + ")"
        color: root.labelColor
        font.pixelSize: 12
        font.bold: true
    }

    ScrollView {
        width: parent.width
        height: 100
        clip: true

        ListView {
            id: animationList
            model: ListModel {
                id: animationListModel
            }

            delegate: Rectangle {
                width: animationList.width
                height: 60
                color: root.listItemColor
                radius: 4
                border.color: root.borderColor

                Row {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Column {
                        width: parent.width - deleteBtn.width - 16
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            text: model.type + (model.mobject ? " - " + model.mobject : "") + (model.property ? " (" + model.property + ")" : "")
                            color: root.inputTextColor
                            font.pixelSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                            width: parent.width
                        }

                        Text {
                            text: "Duration: " + model.duration + "s, Easing: " + (model.easingCurve || "InOutQuad")
                            color: root.labelColor
                            font.pixelSize: 10
                        }

                        Text {
                            text: model.values ? "Values: " + model.values : ""
                            color: root.labelColor
                            font.pixelSize: 10
                            visible: model.values !== ""
                            elide: Text.ElideRight
                            width: parent.width
                        }
                    }

                    Button {
                        id: deleteBtn
                        text: "×"
                        width: 30
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter

                        background: Rectangle {
                            color: root.deleteButtonColor
                            radius: 15
                        }

                        contentItem: Text {
                            text: "×"
                            color: "white"
                            font.pixelSize: 16
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            animationListModel.remove(index);
                            root.itemRemoved(index);
                        }
                    }
                }
            }
        }
    }

    function addAnimation(animationData) {
        animationListModel.append(animationData);
    }

    function clearAll() {
        animationListModel.clear();
    }
}
