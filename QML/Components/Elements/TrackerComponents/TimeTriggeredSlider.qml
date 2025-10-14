import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15

// TimeTriggeredSlider - A slider that appears at a specific time during playback
Rectangle {
    id: root
    
    // Properties
    property string trackerName: ""
    property string trackerType: "val"
    property real minValue: 0
    property real maxValue: 100
    property real currentValue: 50
    property point currentPoint: Qt.point(0, 0)
    property point minPoint: Qt.point(-100, -100)
    property point maxPoint: Qt.point(100, 100)
    property var parser: null
    property real triggerTime: 0  // Time when this slider should appear
    property bool isVisible: false
    
    // Signals
    signal continueRequested()
    signal valueChanged(real newValue)
    signal pointChanged(point newPoint)
    
    visible: isVisible
    width: 280
    height: trackerType === "val" ? 120 : 180
    color: "#2a2a2a"
    border.color: "#555555"
    border.width: 2
    radius: 8
    
    // Drop shadow effect
    Rectangle {
        anchors.fill: parent
        anchors.margins: -4
        color: "#000000"
        opacity: 0.3
        radius: parent.radius + 2
        z: -1
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8
        
        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            
            Text {
                text: trackerType === "val" ? "📊" : "📍"
                font.pixelSize: 16
            }
            
            Text {
                text: `Adjust ${trackerName}`
                color: "#ffffff"
                font.pixelSize: 14
                font.bold: true
                Layout.fillWidth: true
            }
            
            Text {
                text: `@${triggerTime}s`
                color: "#888888"
                font.pixelSize: 12
            }
        }
        
        // Slider content
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            // Value slider
            ColumnLayout {
                visible: trackerType === "val"
                anchors.fill: parent
                spacing: 6
                
                Text {
                    text: `Value: ${currentValue.toFixed(2)}`
                    color: "#cccccc"
                    font.pixelSize: 12
                }
                
                Slider {
                    id: valueSlider
                    Layout.fillWidth: true
                    from: minValue
                    to: maxValue
                    value: currentValue
                    
                    onValueChanged: {
                        root.currentValue = value;
                        if (root.parser && trackerType === "val") {
                            root.parser.setTrackerValue(trackerName, value);
                        }
                        root.valueChanged(value);
                    }
                    
                    background: Rectangle {
                        x: valueSlider.leftPadding
                        y: valueSlider.topPadding + valueSlider.availableHeight / 2 - height / 2
                        width: valueSlider.availableWidth
                        height: 4
                        radius: 2
                        color: "#555555"
                        
                        Rectangle {
                            width: valueSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#4a9eff"
                            radius: 2
                        }
                    }
                    
                    handle: Rectangle {
                        x: valueSlider.leftPadding + valueSlider.visualPosition * (valueSlider.availableWidth - width)
                        y: valueSlider.topPadding + valueSlider.availableHeight / 2 - height / 2
                        width: 18
                        height: 18
                        radius: 9
                        color: valueSlider.pressed ? "#6699ff" : "#4a9eff"
                        border.color: "#ffffff"
                        border.width: 1
                    }
                }
            }
            
            // Point slider
            ColumnLayout {
                visible: trackerType === "pval"
                anchors.fill: parent
                spacing: 6
                
                Text {
                    text: `Position: (${currentPoint.x.toFixed(2)}, ${currentPoint.y.toFixed(2)})`
                    color: "#cccccc"
                    font.pixelSize: 12
                }
                
                // X coordinate slider
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    
                    Text {
                        text: "X:"
                        color: "#cccccc"
                        font.pixelSize: 12
                        Layout.preferredWidth: 15
                    }
                    
                    Slider {
                        id: xSlider
                        Layout.fillWidth: true
                        from: minPoint.x
                        to: maxPoint.x
                        value: currentPoint.x
                        
                        onValueChanged: {
                            let newPoint = Qt.point(value, root.currentPoint.y);
                            root.currentPoint = newPoint;
                            if (root.parser && trackerType === "pval") {
                                root.parser.setPointTrackerValue(trackerName, newPoint);
                            }
                            root.pointChanged(newPoint);
                        }
                        
                        background: Rectangle {
                            x: xSlider.leftPadding
                            y: xSlider.topPadding + xSlider.availableHeight / 2 - height / 2
                            width: xSlider.availableWidth
                            height: 4
                            radius: 2
                            color: "#555555"
                            
                            Rectangle {
                                width: xSlider.visualPosition * parent.width
                                height: parent.height
                                color: "#ff6666"
                                radius: 2
                            }
                        }
                        
                        handle: Rectangle {
                            x: xSlider.leftPadding + xSlider.visualPosition * (xSlider.availableWidth - width)
                            y: xSlider.topPadding + xSlider.availableHeight / 2 - height / 2
                            width: 16
                            height: 16
                            radius: 8
                            color: "#ff6666"
                            border.color: "#ffffff"
                            border.width: 1
                        }
                    }
                }
                
                // Y coordinate slider
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    
                    Text {
                        text: "Y:"
                        color: "#cccccc"
                        font.pixelSize: 12
                        Layout.preferredWidth: 15
                    }
                    
                    Slider {
                        id: ySlider
                        Layout.fillWidth: true
                        from: minPoint.y
                        to: maxPoint.y
                        value: currentPoint.y
                        
                        onValueChanged: {
                            let newPoint = Qt.point(root.currentPoint.x, value);
                            root.currentPoint = newPoint;
                            if (root.parser && trackerType === "pval") {
                                root.parser.setPointTrackerValue(trackerName, newPoint);
                            }
                            root.pointChanged(newPoint);
                        }
                        
                        background: Rectangle {
                            x: ySlider.leftPadding
                            y: ySlider.topPadding + ySlider.availableHeight / 2 - height / 2
                            width: ySlider.availableWidth
                            height: 4
                            radius: 2
                            color: "#555555"
                            
                            Rectangle {
                                width: ySlider.visualPosition * parent.width
                                height: parent.height
                                color: "#66ff66"
                                radius: 2
                            }
                        }
                        
                        handle: Rectangle {
                            x: ySlider.leftPadding + ySlider.visualPosition * (ySlider.availableWidth - width)
                            y: ySlider.topPadding + ySlider.availableHeight / 2 - height / 2
                            width: 16
                            height: 16
                            radius: 8
                            color: "#66ff66"
                            border.color: "#ffffff"
                            border.width: 1
                        }
                    }
                }
            }
        }
        
        // Continue button
        Button {
            text: "Continue ▶"
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            
            onClicked: {
                root.continueRequested();
            }
            
            background: Rectangle {
                color: parent.pressed ? "#5577ff" : (parent.hovered ? "#4466ee" : "#4a9eff")
                radius: 6
                border.color: "#ffffff"
                border.width: 1
            }
            
            contentItem: Text {
                text: parent.text
                color: "#ffffff"
                font.pixelSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}