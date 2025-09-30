import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15
import Animat 1.0
import "Input"

Rectangle {
    id: root

    property var scene: null

    color: "#1f1f1f"
    radius: 4

    // Syntax highlighter object with proper HTML formatting
    QtObject {
        id: syntaxHighlighter

        function highlightText(text) {
            if (!text)
                return "";

            let highlighted = text;

            // Keywords (val, pval, connect) - blue
            // Quick Examples Section

            // Quick Examples Section       // Quick Examples Section highlighted = highlighted.replace(/\b(val|pval|connect)\b/g, '<span style="color: #569cd6; font-weight: bold;">$1</span>');

            // Mathematical functions - yellow
            highlighted = highlighted.replace(/\b(sin|cos|tan|sqrt|abs|pow|exp|log|floor|ceil|round|min|max)\b/g, '<span style="color: #dcdcaa;">$1</span>');

            // Constants - light blue
            highlighted = highlighted.replace(/\b(pi|e)\b/g, '<span style="color: #4fc1ff;">$1</span>');

            // Numbers - light green
            highlighted = highlighted.replace(/\b\d+\.?\d*\b/g, '<span style="color: #b5cea8;">$&</span>');

            // Operators - white
            highlighted = highlighted.replace(/([+\-*/%^=<>!&|])/g, '<span style="color: #d4d4d4;">$1</span>');

            // Parentheses and brackets - gold
            highlighted = highlighted.replace(/([()[\]{}])/g, '<span style="color: #ffd700;">$1</span>');

            // Variable names - light blue
            highlighted = highlighted.replace(/\b([a-zA-Z_][a-zA-Z0-9_]*)\s*=/g, '<span style="color: #9cdcfe;">$1</span>=');

            // String literals - orange
            highlighted = highlighted.replace(/"([^"]*)"/g, '<span style="color: #ce9178;">"$1"</span>');

            return highlighted;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Header
        Text {
            text: "🧮 Value Management"
            color: "#5ce1e6"
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Code Input Section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            color: "#2c2c2c"
            border.color: "#444"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Text {
                    text: "Expression Input:"
                    color: "#ccc"
                    font.pixelSize: 12
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    Rectangle {
                        width: parent.width
                        height: Math.max(codeInput.contentHeight + 16, parent.height)
                        color: "#1a1a1a"
                        border.color: codeInput.activeFocus ? "#0078d4" : "#333333"
                        border.width: 1
                        radius: 6

                        Behavior on border.color {
                            ColorAnimation {
                                duration: 150
                            }
                        }

                        // Enhanced TextArea with syntax highlighting
                        TextArea {
                            id: codeInput
                            anchors.fill: parent
                            anchors.margins: 8
                            font.family: "Consolas, Monaco, monospace"
                            font.pixelSize: 11
                            color: "#ffffff"
                            selectByMouse: true
                            wrapMode: TextArea.Wrap
                            placeholderTextColor: "#666666"
                            placeholderText: "Enter tracker commands and expressions...\n\n" + "Value Trackers:\n" + "val x = 5\n" + "val radius = 10 + 5 * sin(t)\n\n" + "Point Trackers:\n" + "pval center = (100, 200)\n" + "pval position = (x * 2, radius)\n\n" + "Expressions:\n" + "area = pi * radius^2\n" + "distance = sqrt(x^2 + y^2)"

                            background: Rectangle {
                                color: "transparent"
                            }

                            // Syntax highlighting overlay - using multiple Text components approach
                            Rectangle {
                                id: syntaxHighlight
                                anchors.fill: parent
                                color: "transparent"
                                z: -1
                                clip: true

                                Text {
                                    id: highlightedText
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    font: codeInput.font
                                    color: "transparent"
                                    textFormat: Text.RichText
                                    wrapMode: codeInput.wrapMode
                                    text: syntaxHighlighter.highlightText(codeInput.text)
                                    renderType: Text.NativeRendering
                                }
                            }

                            // Auto-completion popup
                            Popup {
                                id: completionPopup
                                width: 250
                                height: Math.min(completionModel.count * 28, 200)
                                x: 0
                                y: codeInput.cursorRectangle.y + codeInput.cursorRectangle.height + 5
                                visible: false
                                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
                                z: 1000

                                background: Rectangle {
                                    color: "#2a2a2a"
                                    border.color: "#4a9eff"
                                    border.width: 1
                                    radius: 4
                                    

                                    anchors.fill: parent






                                }
                            }

                            ListView {
                                id: completionListView
                                anchors.fill: parent
                                anchors.margins: 4
                                model: ListModel {
                                    id: completionModel
                                }
                                currentIndex: 0

                                delegate: Rectangle {
                                    width: completionListView.width
                                    height: 28
                                    color: ListView.isCurrentItem ? "#4a9eff" : (mouseArea.containsMouse ? "#333333" : "transparent")
                                    radius: 2

                                    Rectangle {
                                        width: 3
                                        height: parent.height
                                        color: model.typeColor
                                        visible: ListView.isCurrentItem
                                    }

                                    Row {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 12
                                        anchors.verticalCenter: parent.verticalCenter
                                        spacing: 10

                                        Rectangle {
                                            width: 24
                                            height: 16
                                            color: model.typeColor
                                            radius: 2

                                            Text {
                                                anchors.centerIn: parent
                                                text: model.type
                                                color: "#000"
                                                font.pixelSize: 8
                                                font.bold: true
                                            }
                                        }

                                        Text {
                                            text: model.text
                                            color: ListView.isCurrentItem ? "#fff" : "#e0e0e0"
                                            font.pixelSize: 11
                                            font.family: codeInput.font.family
                                            font.bold: ListView.isCurrentItem
                                        }
                                    }

                                    MouseArea {
                                        id: mouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onClicked: {
                                            completionListView.currentIndex = index;
                                            codeInput.insertCompletion();
                                        }
                                        onEntered: {
                                            completionListView.currentIndex = index;
                                        }
                                    }
                                }
                            }
                        }

                        // Auto-completion state
                        property bool completionActive: false

                        // Key handling for auto-completion
                        Keys.onPressed: event => {
                                            if (completionPopup.visible) {
                                                if (event.key === Qt.Key_Up) {
                                                    completionListView.decrementCurrentIndex();
                                                    event.accepted = true;
                                                } else if (event.key === Qt.Key_Down) {
                                                    completionListView.incrementCurrentIndex();
                                                    event.accepted = true;
                                                } else if (event.key === Qt.Key_Return || event.key === Qt.Key_Tab) {
                                                    codeInput.insertCompletion();
                                                    event.accepted = true;
                                                } else if (event.key === Qt.Key_Escape) {
                                                    completionPopup.close();
                                                    event.accepted = true;
                                                }
                                            } else {
                                                if ((event.modifiers & Qt.ControlModifier) && event.key === Qt.Key_Space) {
                                                    codeInput.showCompletions();
                                                    event.accepted = true;
                                                }

                                            }

                                            // Auto-completion triggers
                                            onTextChanged: {
                                                Qt.callLater(function () {
                                                    if (text.length > 0) {
                                                        checkAutoCompletion();
                                                    } else {
                                                        completionPopup.close();
                                                    }
                                                });
                                            }

                                            onCursorPositionChanged: {
                                                if (!completionPopup.visible)
                                                return;

                                                // Close popup if cursor moves away from trigger position
                                                let currentWord = getCurrentWord();
                                                if (currentWord.length < 1) {
                                                    completionPopup.close();
                                                }
                                            }

                                            // Helper functions
                                            function getCurrentWord() {
                                                let pos = cursorPosition;
                                                let text = codeInput.text;

                                                // Find start of current word
                                                let start = pos;
                                                while (start > 0 && /[a-zA-Z_]/.test(text[start - 1])) {
                                                    start--;
                                                }

                                                return text.substring(start, pos);
                                            }

                                            function checkAutoCompletion() {
                                                let currentWord = getCurrentWord();
                                                console.log("Current word:", currentWord, "Length:", currentWord.length);
                                                if (currentWord.length >= 1) {
                                                    showCompletions(currentWord);
                                                } else {
                                                    completionPopup.close();
                                                }
                                            }

                                            function showCompletions(filter = "") {
                                                console.log("Showing completions for:", filter);
                                                completionModel.clear();

                                                let suggestions = [
                                                        // Keywords
                                                        {
                                                            text: "val",
                                                            type: "KEY",
                                                            typeColor: "#569cd6",
                                                            description: "Value tracker"
                                                        },
                                                        {
                                                            text: "pval",
                                                            type: "KEY",
                                                            typeColor: "#569cd6",
                                                            description: "Point tracker"
                                                        },
                                                        {
                                                            text: "connect",
                                                            type: "KEY",
                                                            typeColor: "#569cd6",
                                                            description: "Connect tracker"
                                                        },

                                                        // Math functions
                                                        {
                                                            text: "sin",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Sine function"
                                                        },
                                                        {
                                                            text: "cos",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Cosine function"
                                                        },
                                                        {
                                                            text: "tan",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Tangent function"
                                                        },
                                                        {
                                                            text: "sqrt",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Square root"
                                                        },
                                                        {
                                                            text: "abs",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Absolute value"
                                                        },
                                                        {
                                                            text: "pow",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Power function"
                                                        },
                                                        {
                                                            text: "exp",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Exponential"
                                                        },
                                                        {
                                                            text: "log",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Natural logarithm"
                                                        },
                                                        {
                                                            text: "floor",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Floor function"
                                                        },
                                                        {
                                                            text: "ceil",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Ceiling function"
                                                        },
                                                        {
                                                            text: "round",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Round function"
                                                        },
                                                        {
                                                            text: "min",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Minimum value"
                                                        },
                                                        {
                                                            text: "max",
                                                            type: "FUN",
                                                            typeColor: "#dcdcaa",
                                                            description: "Maximum value"
                                                        },

                                                        // Constants
                                                        {
                                                            text: "pi",
                                                            type: "CON",
                                                            typeColor: "#4fc1ff",
                                                            description: "Pi constant"
                                                        },
                                                        {
                                                            text: "e",
                                                            type: "CON",
                                                            typeColor: "#4fc1ff",
                                                            description: "Euler's number"
                                                        },
                                                        {
                                                            text: "t",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Time variable"
                                                        },

                                                        // Common variables
                                                        {
                                                            text: "x",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "X coordinate"
                                                        },
                                                        {
                                                            text: "y",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Y coordinate"
                                                        },
                                                        {
                                                            text: "radius",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Radius value"
                                                        },
                                                        {
                                                            text: "center",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Center point"
                                                        },
                                                        {
                                                            text: "position",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Position point"
                                                        },
                                                        {
                                                            text: "width",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Width value"
                                                        },
                                                        {
                                                            text: "height",
                                                            type: "VAR",
                                                            typeColor: "#9cdcfe",
                                                            description: "Height value"
                                                        }
                                                    ];

                                                // Filter suggestions based on current input
                                                for (let suggestion of suggestions) {
                                                    if (filter === "" || suggestion.text.toLowerCase().startsWith(filter.toLowerCase())) {
                                                        completionModel.append(suggestion);
                                                    }
                                                }

                                                if (completionModel.count > 0) {
                                                    completionListView.currentIndex = 0;
                                                    completionPopup.open();
                                                } else {
                                                    completionPopup.close();
                                                }
                                            }

                                            function insertCompletion() {
                                                if (completionListView.currentIndex < 0 || completionListView.currentIndex >= completionModel.count) {
                                                    completionPopup.close();
                                                    return;
                                                }

                                                let completion = completionModel.get(completionListView.currentIndex);
                                                let currentWord = getCurrentWord();

                                                // Replace current word with completion
                                                let pos = cursorPosition;
                                                let start = pos - currentWord.length;

                                                remove(start, pos);
                                                insert(start, completion.text);

                                                completionPopup.close();
                                            }
                                        }
                    }
                }

            }
            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                StyledButton {
                    text: "▶️ Evaluate"
                    enabled: root.scene !== null && codeInput.text.trim() !== ""
                    backgroundColor: "#4a9eff"
                    onClicked: evaluateExpression()
                }

                StyledButton {
                    text: "🗑️ Clear Input"
                    backgroundColor: "#666666"
                    onClicked: codeInput.text = ""
                }

                Item {
                    Layout.fillWidth: true
                } // Spacer

                StyledButton {
                    text: "📋 Clear Logs"
                    backgroundColor: "#d4651a"
                    isDestructive: true
                    onClicked: {
                        logModel.clear();
                        addLog("📋", "Logs cleared", "#5ce1e6");
                    }
                }
            }
        }
        // Quick Examples Section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: "#2c2c2c"
            border.color: "#444"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Text {
                    text: "🚀 Quick Examples:"
                    color: "#ccc"
                    font.pixelSize: 12
                }

                Flow {
                    Layout.fillWidth: true
                    spacing: 4

                    Repeater {
                        model: [
                            {
                                text: "val x",
                                expr: "val x = 5"
                            },
                            {
                                text: "val r",
                                expr: "val radius = 10"
                            },
                            {
                                text: "pval",
                                expr: "pval center = (100, 200)"
                            },
                            {
                                text: "expr",
                                expr: "area = pi * radius^2"
                            },
                            {
                                text: "func",
                                expr: "val wave = 50 * sin(t)"
                            },
                            {
                                text: "point",
                                expr: "pval pos = (x, wave)"
                            },
                            {
                                text: "connect",
                                expr: "connect x myCircle radius"
                            }
                        ]

                        StyledButton {
                            required property var modelData
                            text: modelData.text
                            width: 60
                            height: 24
                            backgroundColor: "#4a9eff"
                            font.pixelSize: 9
                            onClicked: {
                                codeInput.text = modelData.expr;
                                evaluateExpression();
                            }
                        }
                    }
                }
            }
        }

        // Logs Section
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 150
            color: "#2c2c2c"
            border.color: "#444"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Text {
                    text: "📊 Evaluation Logs:"
                    color: "#ccc"
                    font.pixelSize: 12
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    ListView {
                        id: logListView
                        model: ListModel {
                            id: logModel
                        }
                        spacing: 2

                        delegate: Rectangle {
                            width: logListView.width
                            height: logText.implicitHeight + 8
                            color: index % 2 === 0 ? "#1a1a1a" : "#222"
                            radius: 2

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 4
                                spacing: 8

                                Text {
                                    text: model.timestamp
                                    color: "#666"
                                    font.pixelSize: 9
                                    font.family: "Consolas, Monaco, monospace"
                                    Layout.preferredWidth: 60
                                }

                                Text {
                                    text: model.icon
                                    font.pixelSize: 12
                                    Layout.preferredWidth: 20
                                }

                                Text {
                                    id: logText
                                    text: model.message
                                    color: model.color
                                    font.pixelSize: 10
                                    font.family: "Consolas, Monaco, monospace"
                                    wrapMode: Text.Wrap
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        // Auto-scroll to bottom when new items are added
                        onCountChanged: {
                            Qt.callLater(function () {
                                if (count > 0) {
                                    positionViewAtEnd();
                                }
                            });
                        }
                    }
                }
            }
        }


        // JavaScript functions
        function evaluateExpression() {
            if (!root.scene) {
                addLog("❌", "Error: Scene not available", "#ff4444");
                return;
            }

            let expression = codeInput.text.trim();
            if (expression === "") {
                addLog("⚠️", "Warning: Empty expression", "#ffaa44");
                return;
            }

            try {
                // Check if it's a tracker command (val, pval, connect, etc.)
                if (expression.startsWith("val ") || expression.startsWith("pval ") || expression.startsWith("connect ") || expression.includes("=")) {
                    // Execute as tracker script
                    root.scene.executeTrackerScript(expression);
                    addLog("🎯", `Executed: ${expression}`, "#44ff44");

                    // Show current tracker values
                    let trackerNames = root.scene.getTrackerNames();
                    if (trackerNames.length > 0) {
                        addLog("📊", `Active trackers: ${trackerNames.join(", ")}`, "#5ce1e6");
                    }
                } else {
                    // Evaluate as mathematical expression
                    let result = root.scene.evaluate(expression);
                    addLog("✅", `${expression} = ${result}`, "#44ff44");
                }
            } catch (error) {
                addLog("❌", `Error: ${error}`, "#ff4444");
            }
        }

        function addLog(icon, message, color) {
            let now = new Date();
            let timestamp = now.getHours().toString().padStart(2, '0') + ":" + now.getMinutes().toString().padStart(2, '0') + ":" + now.getSeconds().toString().padStart(2, '0');

            logModel.append({
                                timestamp: timestamp,
                                icon: icon,
                                message: message,
                                color: color || "#ccc"
                            });

            // Keep only last 100 entries
            if (logModel.count > 100) {
                logModel.remove(0, logModel.count - 100);
            }
        }}

    // Component initialization
    Component.onCompleted: {
        addLog("🚀", "Tracker Management initialized", "#5ce1e6");
        if (root.scene) {
            addLog("🔗", "Scene and TrackerManager connected", "#44ff44");
            addLog("💡", "Ready for 'val', 'pval', 'connect' commands", "#5ce1e6");
        } else {
            addLog("⚠️", "Scene not connected", "#ffaa44");
        }

    }

    onSceneChanged: {
        if (root.scene) {
            addLog("🔗", "Scene and TrackerManager connected", "#44ff44");
            addLog("💡", "Ready for tracker commands", "#5ce1e6");
        } else {
            addLog("💔", "Scene disconnected", "#ff4444");
        }
    }
}
