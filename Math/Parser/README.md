# Parser System with CommandManager

This enhanced parser system provides a regex-based command interface for scene operations and ValueTracker management, now with improved organization through the CommandManager.

## Architecture Overview

The system consists of:
- **Parser**: Main interface that coordinates parsing and execution
- **CommandManager**: Manages command registration, categorization, and execution
- **Command**: Represents individual commands with regex patterns and handlers
- **TrackerManager**: Manages ValueTracker instances in a hashmap

## Quick Start

```cpp
#include "parser.h"

// Create parser with scene (automatically creates CommandManager and TrackerManager)
Parser* parser = new Parser(scene);

// Parse commands
parser->parseCommand("create_tracker position 0.0");
parser->parseCommand("set_tracker position 10.5");
parser->parseCommand("help");
```

## CommandManager Features

### Automatic Command Categories
Commands are organized into categories:
- **Basic**: create_tracker, set_tracker, list_trackers, remove_tracker
- **Math**: add_to_tracker, multiply_tracker
- **Animation**: animate_tracker
- **Scene**: reset_scene, help

### Built-in Help System
- `help` - List all available commands
- `help <command_name>` - Get detailed help for a specific command

### Command Information
Each command includes:
- Name and regex pattern
- Category classification
- Description
- Usage example

## Available Commands

### Basic Operations
```
create_tracker <name> [initial_value]  - Create a new ValueTracker
set_tracker <name> <value>             - Set tracker value
remove_tracker <name>                  - Remove tracker
list_trackers                          - List all tracker names
```

### Mathematical Operations
```
add_to_tracker <name> <value>          - Add value to tracker
multiply_tracker <name> <factor>       - Multiply tracker by factor
```

### Animation Commands
```
animate_tracker <name> <target> <duration_ms> - Animate tracker (placeholder)
```

### Scene Commands
```
reset_scene                            - Clear all trackers
help [command_name]                    - Show help information
```

## Adding New Commands

### Method 1: Through CommandManager
```cpp
// Access CommandManager through Parser
CommandManager* cmdManager = parser->commandManager();

// Register new command
cmdManager->registerCommand("my_command",
    QRegularExpression(R"(^my_command\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)"),
    [](const QString& matches, TrackerManager* tm, Scene* scene) {
        // Extract parameters from matches string using regex
        QRegularExpression regex(R"(^my_command\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)");
        QRegularExpressionMatch match = regex.match(matches);
        if (match.hasMatch()) {
            QString name = match.captured(1);
            qreal value = match.captured(2).toDouble();
            // Your command logic here
        }
    });
```

### Method 2: Extend CommandManager Class
Add new methods like `registerCustomCommands()` to `CommandManager` class:

```cpp
void CommandManager::registerCustomCommands()
{
    registerCommandWithInfo("custom_cmd",
        QRegularExpression(R"(^custom_cmd\s+(\w+)\s*$)"),
        [](const QString& matches, TrackerManager* tm, Scene* scene) {
            // Command logic
        },
        "Custom", "My custom command", "custom_cmd <name>");
}
```

## Signal System

### Parser Signals
```cpp
// Command execution feedback
connect(parser, &Parser::commandExecuted, [](const QString& name, const QString& input) {
    qDebug() << "Executed:" << name << "with input:" << input;
});

connect(parser, &Parser::commandFailed, [](const QString& input, const QString& error) {
    qDebug() << "Failed:" << input << "Error:" << error;
});

// Tracker management
connect(parser, &Parser::trackerCreated, [](const QString& name) {
    qDebug() << "Tracker created:" << name;
});

connect(parser, &Parser::trackerRemoved, [](const QString& name) {
    qDebug() << "Tracker removed:" << name;
});
```

### CommandManager Signals
```cpp
// Command registration feedback
connect(cmdManager, &CommandManager::commandRegistered, ...);
connect(cmdManager, &CommandManager::commandUnregistered, ...);
```

## Command Parameter Extraction

Since the Command system passes the entire matched string, you need to re-parse it in your handler:

```cpp
[](const QString& matches, TrackerManager* tm, Scene* scene) {
    // Define the same regex used for registration
    QRegularExpression regex(R"(^command_name\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)");
    QRegularExpressionMatch match = regex.match(matches);
    
    if (match.hasMatch()) {
        QString param1 = match.captured(1);      // First capture group
        qreal param2 = match.captured(2).toDouble(); // Second capture group
        
        // Use parameters
    }
}
```

## Regex Pattern Guidelines

- `^` - Start of string
- `$` - End of string
- `\s+` - One or more whitespaces
- `\s*` - Zero or more whitespaces
- `(\w+)` - Capture word characters (names)
- `([+-]?\d*\.?\d+)` - Capture decimal numbers
- `(\d+)` - Capture positive integers
- `(?:pattern)?` - Optional group

## Files Structure

```
Math/Parser/
├── parser.h/.cpp              - Main parser interface
├── commandmanager.h/.cpp      - Command management and execution
├── command.h/.cpp             - Individual command representation
├── trackermanager.h/.cpp      - ValueTracker management
├── CommandManagerExample.cpp  - Usage examples
└── README.md                  - This documentation
```

## Integration with QML

The Parser is QML_ELEMENT enabled:

```qml
// In QML
Parser {
    id: scriptParser
    
    Component.onCompleted: {
        scriptParser.parseCommand("create_tracker myValue 42.0")
        console.log("Available commands:", scriptParser.getAvailableCommands())
    }
    
    onCommandExecuted: function(name, input) {
        console.log("Command executed:", name)
    }
    
    onCommandFailed: function(input, error) {
        console.log("Command failed:", input, "Error:", error)
    }
}
```

## Performance and Best Practices

1. **Command Registration**: Register commands once during initialization
2. **Regex Efficiency**: Use specific patterns to avoid conflicts
3. **Error Handling**: Always check if trackers exist before operating on them
4. **Memory Management**: CommandManager automatically manages Command lifecycle
5. **Threading**: Use from main thread or add synchronization

## Example Usage

See `CommandManagerExample.cpp` for comprehensive usage examples including:
- Basic command execution
- Custom command registration
- Error handling
- Advanced command patterns