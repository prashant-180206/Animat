// Demo: How to use the Time-Triggered Slider System
// Add this to any QML file or C++ code that has access to the Main window

// Example 1: Schedule a value tracker slider at 2 seconds
function demoValueSlider() {
    var trackerConfig = {
        trackerId: "demo_value",
        trackerType: "value",     // "value" for ValueTracker, "point" for PointValueTracker
        name: "Demo Value",
        min: 0,
        max: 100,
        currentValue: 50,
        step: 1
    };
    
    // Schedule to appear at 2000ms (2 seconds)
    scheduleSlider(2000, trackerConfig);
}

// Example 2: Schedule a point tracker slider at 5 seconds  
function demoPointSlider() {
    var trackerConfig = {
        trackerId: "demo_point",
        trackerType: "point",
        name: "Demo Point",
        xval: 0,
        yval: 0,
        minX: -10,
        maxX: 10,
        minY: -10,
        maxY: 10,
        stepX: 0.1,
        stepY: 0.1
    };
    
    // Schedule to appear at 5000ms (5 seconds)
    scheduleSlider(5000, trackerConfig);
}

// Example 3: Schedule multiple sliders at different times
function demoMultipleSliders() {
    // First slider at 1 second
    scheduleSlider(1000, {
        trackerId: "slider_1",
        trackerType: "value",
        name: "Speed",
        min: 0,
        max: 10,
        currentValue: 5,
        step: 0.5
    });
    
    // Second slider at 3 seconds
    scheduleSlider(3000, {
        trackerId: "slider_2", 
        trackerType: "point",
        name: "Position",
        xval: 2,
        yval: 3,
        minX: -5,
        maxX: 5,
        minY: -5,
        maxY: 5,
        stepX: 0.1,
        stepY: 0.1
    });
    
    // Third slider at 7 seconds
    scheduleSlider(7000, {
        trackerId: "slider_3",
        trackerType: "value",
        name: "Opacity",
        min: 0,
        max: 1,
        currentValue: 0.8,
        step: 0.1
    });
}

// Clear all scheduled sliders if needed
function clearAllSliders() {
    clearScheduledSliders();
}

/* 
USAGE FROM C++:
If you need to schedule sliders from C++ code, you can call QML functions:

QQuickItem* mainWindow = // get reference to Main.qml
QVariant trackerConfig = // create QVariantMap with tracker properties
QMetaObject::invokeMethod(mainWindow, "scheduleSlider", 
                         Q_ARG(QVariant, 2000),  // time in ms
                         Q_ARG(QVariant, trackerConfig));

INTEGRATION WITH PARSER:
The time-triggered sliders automatically connect to the parser to get real-time
values when the user interacts with them. The slider values are updated in the
parser and will affect the animation accordingly.

PLAYBACK BEHAVIOR:
1. Sliders appear at their scheduled time during playback
2. Playback automatically pauses when a slider appears
3. User can adjust the slider values as needed
4. Clicking "Continue" resumes playback and hides the slider
5. Multiple sliders can be scheduled for different times
*/