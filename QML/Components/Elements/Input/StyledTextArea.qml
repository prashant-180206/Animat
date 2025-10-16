import QtQuick 2.15
import QtQuick.Controls.Basic
import Animat 1.0

TextEdit {
    id: control
    property color backgroundColor: "#121212"
    property color textColor: "#f8f8f2"
    property color borderColor: "#333333"
    property color placeholderColor: "#666666"
    property bool formatting: false
    property bool intellisenseActive: false
    property var suggestions: []
    property string currentWord: ""
    property int wordStartPos: 0
    property Scene scene

    width: 200
    height: 120
    anchors.margins: 20
    textFormat: TextEdit.RichText
    wrapMode: TextEdit.Wrap
    selectByMouse: true
    color: textColor
    font.family: "Consolas"
    font.pixelSize: 12
    text: "val foo = (bar);"

    // --------------------------------
    // Syntax Highlighting
    // --------------------------------
    Timer {
        id: formatTimer
        interval: 100  // Debounce formatting
        repeat: false
        onTriggered: performFormatting()
    }

    function formatText() {
        if (formatting || !control.visible)
            return;
        formatTimer.restart();
    }

    function performFormatting() {
        if (formatting)
            return;
        formatting = true;

        let cursorPos = control.cursorPosition;
        let selectionStart = control.selectionStart;
        let selectionEnd = control.selectionEnd;
        let input = control.getText(0, control.length);

        if (!input || input.length > 5000) {
            formatting = false;
            return;
        }

        let keywords = ["val", "dval", "pval","dpval", "connect", "to", "from"];
        let operators = ["=", "+", "-", "*", "/"];
        let delimiters = ["(", ")", "{", "}", "[", "]", ";", ","];

        let tokens = [];
        let regex = /(\/\/.*$|[A-Za-z_]\w*|\d+(?:\.\d+)?|[=+\-*\/(){}[\];,]|\s+|.)/gm;
        let match;

        while ((match = regex.exec(input)) !== null) {
            tokens.push({
                            text: match[0],
                            start: match.index,
                            end: match.index + match[0].length
                        });
        }

        let newtext = "";
        for (let token of tokens) {
            let text = token.text;
            if (keywords.includes(text))
                newtext += `<span style="color:#5ce1e6;font-weight:bold;">${text}</span>`;
            else if (text.match(/^\d+(?:\.\d+)?$/))
                newtext += `<span style="color:#ffb86c;">${text}</span>`;
            else if (text.match(/^\/\/.*/))
                newtext += `<span style="color:#888888;">${text}</span>`;
            else if (operators.includes(text))
                newtext += `<span style="color:#ff79c6;">${text}</span>`;
            else if (delimiters.includes(text))
                newtext += `<span style="color:#bd93f9;">${text}</span>`;
            else if (text.match(/^[A-Za-z_]\w*$/))
                newtext += `<span style="color:#50fa7b;">${text}</span>`;
            else
                newtext += text;
        }

        if (newtext !== control.text) {
            control.text = newtext;
            Qt.callLater(function () {
                control.cursorPosition = Math.min(cursorPos, control.length);
                if (selectionStart !== selectionEnd) {
                    control.select(Math.min(selectionStart, control.length), Math.min(selectionEnd, control.length));
                }
            });
        }

        formatting = false;
    }

    onTextChanged: {
        formatText();
        updateIntellisense();
    }

    // --------------------------------
    // Intellisense System
    // --------------------------------
    function updateIntellisense() {
        if (formatting)
            return;

        let pos = control.cursorPosition;
        let text = control.getText(0, control.length);

        let beforeCursor = text.substring(0, pos);
        let wordMatch = beforeCursor.match(/[A-Za-z_]\w*$/);

        if (wordMatch) {
            currentWord = wordMatch[0];
            wordStartPos = pos - currentWord.length;
            suggestions = generateSuggestions(currentWord, beforeCursor);
            intellisenseActive = suggestions.length > 0 && currentWord.length > 0;
        } else {
            intellisenseActive = false;
            currentWord = "";
            suggestions = [];
        }
    }

    function generateSuggestions(word, context) {
        let allSuggestions = [
                {
                    text: "val",
                    detail: "val name = value",
                    type: "keyword"
                },
                {
                    text: "pval",
                    detail: "pval name = (x, y)",
                    type: "keyword"
                },
                {
                    text: "dval",
                    detail: "dval name = expression",
                    type: "keyword"
                },
                {
                    text: "dpval",
                    detail: "dpval name = expression",
                    type: "keyword"
                },
                {
                    text: "connect",
                    detail: "connect tracker to mobject",
                    type: "keyword"
                },
                {
                    text: "to",
                    detail: "connection keyword",
                    type: "keyword"
                },
                {
                    text: "from",
                    detail: "source keyword",
                    type: "keyword"
                },
                {
                    text: "height",
                    detail: "mobject property",
                    type: "keyword"
                },
                {
                    text: "width",
                    detail: "mobject property",
                    type: "keyword"
                },
                {
                    text: "radius",
                    detail: "circle property",
                    type: "keyword"
                },
                {
                    text: "thickness",
                    detail: "line property",
                    type: "keyword"
                },
                {
                    text: "tend",
                    detail: "function property",
                    type: "keyword"
                },
                , {
                    text: "position",
                    detail: "mobject property",
                    type: "keyword"
                },
                {
                    text: "trange",
                    detail: "curve property",
                    type: "keyword"
                },
                {
                    text: "lstart",
                    detail: "line property",
                    type: "keyword"
                },
                {
                    text: "lend",
                    detail: "line property",
                    type: "keyword"
                },
                ...scene.mobjectIds.map(id => {
                                            return {
                                                text: id,
                                                detail: "mobject id",
                                                type: "mobject"
                                            };
                                        })];

        if (context.includes("connect") && !context.includes(" to ")) {
            allSuggestions.push({
                                    text: "to",
                                    detail: "connect ... to ...",
                                    type: "keyword"
                                });
        }

        return allSuggestions.filter(s => s.text.toLowerCase().startsWith(word.toLowerCase())).slice(0, 5);
    }

    function acceptSuggestion(suggestion) {
        if (!intellisenseActive)
            return;

        formatting = true;

        let pos = control.cursorPosition;
        control.remove(wordStartPos, pos);
        control.insert(wordStartPos, suggestion.text);

        intellisenseActive = false;
        formatting = false;
    }

    Keys.onPressed: event => {
                        const ignoredKeys = [Qt.Key_Shift, Qt.Key_Control, Qt.Key_Alt, Qt.Key_Meta, Qt.Key_Left, Qt.Key_Right, Qt.Key_Up, Qt.Key_Down, Qt.Key_Home, Qt.Key_End, Qt.Key_PageUp, Qt.Key_PageDown, Qt.Key_CapsLock];

                        if (ignoredKeys.includes(event.key) || event.text === undefined || event.text === "") {
                            event.accepted = false;
                            return;
                        }

                        let pos = control.cursorPosition;

                        // IntelliSense
                        if (intellisenseActive) {
                            if (event.key === Qt.Key_Tab || event.key === Qt.Key_Return) {
                                if (suggestions.length > 0) {
                                    acceptSuggestion(suggestions[0]);
                                    event.accepted = true;
                                    return;
                                }
                            }
                            if (event.key === Qt.Key_Escape) {
                                intellisenseActive = false;
                                event.accepted = true;
                                return;
                            }
                        }
                        if (event.key === Qt.Key_ParenLeft || event.key === Qt.Key_BraceLeft || event.key === Qt.Key_BracketLeft) {
                            control.cursorPosition += 1;
                            if (event.key === Qt.Key_ParenLeft)
                            control.insert(control.cursorPosition, ")");
                            else if (event.key === Qt.Key_BraceLeft)
                            control.insert(control.cursorPosition, "}");
                            else if (event.key === Qt.Key_BracketLeft)
                            control.insert(control.cursorPosition, "]");
                            control.cursorPosition -= 1;
                        }

                        event.accepted = false;
                    }

    // --------------------------------
    // Background
    // --------------------------------
    Rectangle {
        anchors.fill: parent
        color: control.backgroundColor
        radius: 8
        border.color: control.borderColor
        z: control.z - 1
    }

    // --------------------------------
    // Intellisense Popup
    // --------------------------------
    Rectangle {
        id: intellisensePopup
        visible: control.intellisenseActive
        width: 200
        height: Math.min(suggestions.length * 25, 125)
        color: "#2d2d30"
        border.color: "#5ce1e6"
        border.width: 1
        radius: 4
        z: 1000

        x: Math.min(control.width - width, Math.max(0, cursorRectangle.x))
        y: cursorRectangle.y + cursorRectangle.height + 5

        ListView {
            anchors.fill: parent
            anchors.margins: 2
            model: control.suggestions

            delegate: Rectangle {
                width: parent.width
                height: 25
                color: index === 0 ? "#404040" : "transparent"

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Text {
                        text: modelData.type === "keyword" ? "🔑" : "📝"
                        font.pixelSize: 12
                    }
                    Text {
                        text: modelData.text
                        color: "#ffffff"
                        font.pixelSize: 12
                        font.bold: true
                    }
                    Text {
                        text: modelData.detail
                        color: "#cccccc"
                        font.pixelSize: 10
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: control.acceptSuggestion(modelData)
                }
            }
        }
    }
}
