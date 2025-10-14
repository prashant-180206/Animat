import QtQuick 2.15
import QtQuick.Controls.Basic

// TimeTriggeredSlidersManager - Manages time-triggered sliders during playback
Item {
    id: root
    
    // Properties
    property var player: null
    property var parser: null
    property var activeSliders: ({})  // From TrackerDataDisplay
    property bool enabled: true
    
    // List of time-triggered slider configurations
    property var scheduledSliders: []
    
    // Signals
    signal sliderTriggered(var sliderConfig)
    signal playbackPaused()
    signal playbackResumed()
    
    // Internal state
    property real lastCheckedTime: 0
    property var pendingSliders: []
    property bool isWaitingForContinue: false
    
    // Timer to check playback time
    Timer {
        id: timeChecker
        interval: 100  // Check every 100ms
        running: root.enabled && root.player && !root.isWaitingForContinue
        repeat: true
        
        onTriggered: {
            if (!root.player) return;
            
            let currentTime = root.player.currentTime();
            
            // Check if any scheduled sliders should be triggered
            checkForTriggeredSliders(currentTime);
            
            root.lastCheckedTime = currentTime;
        }
    }
    
    // Functions
    function addScheduledSlider(config) {
        /*
        config format:
        {
            triggerTime: 2.0,           // Time in seconds when to show slider
            trackerName: "myTracker",   // Name of the tracker
            trackerType: "val",         // "val" or "pval"
            minValue: 0,                // For value trackers
            maxValue: 100,
            minPoint: Qt.point(-50, -50), // For point trackers
            maxPoint: Qt.point(50, 50),
            message: "Adjust the value"  // Optional message
        }
        */
        root.scheduledSliders.push(config);
        console.log(`Scheduled slider for ${config.trackerName} at ${config.triggerTime}s`);
    }
    
    function removeScheduledSlider(trackerName, triggerTime) {
        root.scheduledSliders = root.scheduledSliders.filter(config => 
            !(config.trackerName === trackerName && config.triggerTime === triggerTime)
        );
    }
    
    function clearScheduledSliders() {
        root.scheduledSliders = [];
        root.pendingSliders = [];
        root.isWaitingForContinue = false;
    }
    
    function checkForTriggeredSliders(currentTime) {
        for (let i = 0; i < root.scheduledSliders.length; i++) {
            let config = root.scheduledSliders[i];
            
            // Check if this slider should be triggered
            if (config.triggerTime <= currentTime && 
                config.triggerTime > root.lastCheckedTime && 
                !isSliderAlreadyTriggered(config)) {
                
                triggerSlider(config);
            }
        }
    }
    
    function isSliderAlreadyTriggered(config) {
        return root.pendingSliders.some(slider => 
            slider.trackerName === config.trackerName && 
            slider.triggerTime === config.triggerTime
        );
    }
    
    function triggerSlider(config) {
        console.log(`Triggering slider for ${config.trackerName} at time ${config.triggerTime}`);
        
        // Pause playback
        if (root.player) {
            root.player.pause();
            root.playbackPaused();
        }
        
        // Get current value from tracker
        let currentValue = 0;
        let currentPoint = Qt.point(0, 0);
        
        if (root.parser) {
            if (config.trackerType === "val") {
                currentValue = root.parser.getTrackerValue(config.trackerName);
            } else {
                currentPoint = root.parser.getPointTrackerValue(config.trackerName);
            }
        }
        
        // Create slider configuration with current values
        let sliderConfig = {
            ...config,
            currentValue: currentValue,
            currentPoint: currentPoint
        };
        
        root.pendingSliders.push(sliderConfig);
        root.isWaitingForContinue = true;
        root.sliderTriggered(sliderConfig);
    }
    
    function continuePlayback() {
        root.isWaitingForContinue = false;
        root.pendingSliders = [];
        
        // Resume playback
        if (root.player) {
            root.player.play();
            root.playbackResumed();
        }
        
        console.log("Playback resumed");
    }
    
    // Example function to add some test sliders
    function addExampleSliders() {
        // Add a value slider at 2 seconds
        addScheduledSlider({
            triggerTime: 2.0,
            trackerName: "testValue", 
            trackerType: "val",
            minValue: 0,
            maxValue: 100,
            message: "Adjust the test value"
        });
        
        // Add a point slider at 5 seconds
        addScheduledSlider({
            triggerTime: 5.0,
            trackerName: "testPoint",
            trackerType: "pval", 
            minPoint: Qt.point(-50, -50),
            maxPoint: Qt.point(50, 50),
            message: "Adjust the test position"
        });
    }
    
    Component.onCompleted: {
        console.log("TimeTriggeredSlidersManager initialized");
        // Uncomment to add example sliders
        // addExampleSliders();
    }
}