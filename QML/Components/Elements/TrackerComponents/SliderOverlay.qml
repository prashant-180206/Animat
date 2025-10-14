import QtQuick 2.15
import QtQuick.Controls.Basic

// SliderOverlay - Displays time-triggered sliders on top of the scene
Item {
    id: root
    
    // Properties
    property var parser: null
    property alias manager: slidersManager
    
    // Signals  
    signal sliderCompleted()
    
    anchors.fill: parent
    
    // Time-triggered sliders manager
    TimeTriggeredSlidersManager {
        id: slidersManager
        parser: root.parser
        
        onSliderTriggered: (sliderConfig) => {
            showSlider(sliderConfig);
        }
    }
    
    // Container for active overlay sliders
    Item {
        id: overlayContainer
        anchors.fill: parent
        z: 1000  // High z-order to appear on top
    }
    
    // Function to show a triggered slider
    function showSlider(config) {
        console.log(`Showing overlay slider for ${config.trackerName}`);
        
        // Create the slider component
        let sliderComponent = Qt.createComponent("TimeTriggeredSlider.qml");
        if (sliderComponent.status === Component.Ready) {
            let slider = sliderComponent.createObject(overlayContainer, {
                trackerName: config.trackerName,
                trackerType: config.trackerType,
                minValue: config.minValue || 0,
                maxValue: config.maxValue || 5,
                currentValue: config.currentValue || 2,
                currentPoint: config.currentPoint || Qt.point(0, 0),
                minPoint: config.minPoint || Qt.point(-5, -5),
                maxPoint: config.maxPoint || Qt.point(5, 5),
                parser: root.parser,
                triggerTime: config.triggerTime,
                isVisible: true
            });
            
            // Position slider in top-right corner
            slider.anchors.top = overlayContainer.top;
            slider.anchors.right = overlayContainer.right;
            slider.anchors.topMargin = 20;
            slider.anchors.rightMargin = 20;
            
            // Connect signals
            slider.continueRequested.connect(() => {
                hideSlider(slider);
                slidersManager.continuePlayback();
                root.sliderCompleted();
            });
            
            // Add smooth appearance animation
            slider.opacity = 0;
            slider.scale = 0.8;
            
            // Animate appearance
            let appearAnimation = Qt.createQmlObject(`
                import QtQuick 2.15
                ParallelAnimation {
                    NumberAnimation {
                        target: slider
                        property: "opacity"
                        to: 1.0
                        duration: 300
                        easing.type: Easing.OutQuart
                    }
                    NumberAnimation {
                        target: slider
                        property: "scale"
                        to: 1.0
                        duration: 300
                        easing.type: Easing.OutBack
                    }
                }
            `, overlayContainer);
            
            appearAnimation.start();
            
        } else {
            console.error("Failed to create TimeTriggeredSlider component:", sliderComponent.errorString());
        }
    }
    
    function hideSlider(slider) {
        if (!slider) return;
        
        // Animate disappearance
        let disappearAnimation = Qt.createQmlObject(`
            import QtQuick 2.15
            ParallelAnimation {
                NumberAnimation {
                    target: slider
                    property: "opacity"
                    to: 0.0
                    duration: 200
                    easing.type: Easing.InQuart
                }
                NumberAnimation {
                    target: slider
                    property: "scale"
                    to: 0.8
                    duration: 200
                    easing.type: Easing.InQuart
                }
                onFinished: {
                    slider.destroy();
                }
            }
        `, overlayContainer);
        
        disappearAnimation.start();
    }
    
    // Public API functions
    function addScheduledSlider(config) {
        slidersManager.addScheduledSlider(config);
    }
    
    function removeScheduledSlider(trackerName, triggerTime) {
        slidersManager.removeScheduledSlider(trackerName, triggerTime);
    }
    
    function clearScheduledSliders() {
        slidersManager.clearScheduledSliders();
        
        // Clear any existing overlay sliders
        for (let i = overlayContainer.children.length - 1; i >= 0; i--) {
            let child = overlayContainer.children[i];
            if (child.objectName === "timeTriggeredSlider") {
                child.destroy();
            }
        }
    }
    
    function setPlayer(player) {
        slidersManager.player = player;
    }
    
    // Example function to demonstrate usage
    function addExampleSliders() {
        // Add a value slider at 2 seconds
        addScheduledSlider({
            triggerTime: 2.0,
            trackerName: "exampleValue",
            trackerType: "val",
            minValue: 0,
            maxValue: 100
        });
        
        // Add a point slider at 4 seconds  
        addScheduledSlider({
            triggerTime: 4.0,
            trackerName: "examplePoint",
            trackerType: "pval",
            minPoint: Qt.point(-50, -50),
            maxPoint: Qt.point(50, 50)
        });
        
        console.log("Added example time-triggered sliders");
    }
}