# Animat - Animation Framework

A powerful Qt6 QML-based animation framework inspired by Manim, designed for creating mathematical and geometric animations with an intuitive user interface.

## 🎯 Overview

Animat provides a comprehensive animation system that allows you to create, sequence, and playback various types of animations on graphical objects (Mobjects). The system features dynamic timing, interactive controls, modular architecture, and a sophisticated value tracking system for data-driven animations.

## 🏗️ Architecture Overview

### Core Components

- **Animation System** (`Math/Animations/`) - Timeline management and animation types
- **Scene Management** (`Math/Scene.cpp`) - Canvas coordination and object management  
- **UI Components** (`QML/Components/`) - Interactive user interface
- **Value Tracking** (`Math/ValueTracker/`) - Real-time parameter control

### 1. Mobject Hierarchy (`Math/Mobjects/`)

The foundation of the animation system is built around **Mobjects** (Mathematical Objects) with a clear inheritance hierarchy:

```text
Mobject (Base QQuickItem)
└── ClickableMobject (Adds interactivity, properties, scene integration)
    ├── Base/
    │   └── Mobject                   # Core mobject foundation
    ├── Basic/
    │   ├── Circle                    # Circular shapes with radius control
    │   ├── Line                      # Basic line segments with arrow tips
    │   ├── Vector                    # Line with arrow tip for directional representation
    │   ├── Dot                       # Small circular markers
    │   ├── MRectangle                # Rectangular shapes
    │   └── Polygon                   # Multi-sided shapes with customizable vertices
    ├── Simple/
    │   ├── SimpleLine                # Optimized line with circular caps
    │   ├── SimpleDot                 # Lightweight dot for performance
    │   ├── SimpleVector              # Lightweight vector for groups
    │   └── SimpleText                # Lightweight text rendering
    ├── Complex/
    │   ├── Curve                     # Mathematical curves with parametric equations
    │   ├── MPolygon                  # Dynamic polygon with runtime editing
    │   └── Plane                     # Coordinate system and grid
    ├── Groups/
    │   ├── Group                     # Standard object grouping
    │   └── VGroup                    # Virtual grouping with shared properties
    ├── MText/
    │   └── MText                     # Full-featured text with fonts and formatting
    └── Geometric/
        └── Angle                     # Geometric angle representation with arc
```

**Key Features:**

- Each object has a unique ID and maintains its state through JSON serialization
- Property-based type identification system
- Hierarchical organization for different complexity levels

### 2. Properties System (`Math/Helper/Properties/`)

**Main Properties Container:**

- `MProperties` - Central property container with specialized sub-properties

**Property Types:**

- `BaseProperties` - Common to all mobjects (position, color, opacity, scale, rotation, z-index)
- `LineProperties` - Line-specific parameters (start/end points, thickness)
- `CircleProperties` - Circle-specific parameters (radius, center)
- `TextProperties` - Font, size, weight, family for text objects
- `PolygonProperties` - Polygon-specific parameters (border color, thickness)
- `CurveProperties` - Curve-specific parameters
- `GeometricProperties` - Geometric objects (contains AngleProperties for angles)

Objects are identified by their **property existence** - each Mobject constructor initializes only the properties it needs, making object type identification possible by checking which properties exist.

### 3. Scene Management (`Math/Scene.h/.cpp`)

The `Scene` class serves as the main canvas and coordinator:

- **Mobject Registry:** Maintains all objects with unique IDs
- **Selection System:** Tracks currently selected object for property editing
- **Animation Manager:** Coordinates all animations and playback
- **Parser Integration:** Supports command-based object creation and manipulation
- **JSON Serialization:** Save/load complete scenes with all object states

### 4. Object Registration System (`Utils/`)

**MobjectMap (`mobjectmap.h/.cpp`):**

- Factory pattern for creating objects by string name
- Maps object type names to constructor functions
- Initialized once with scene reference for all objects

**MobjectRegistry (`mobjectregistry.h/.cpp`):**

- QML-accessible list model for UI display
- Contains object names and icon paths for toolbar/sidebar
- Used in QML components for object selection menus

## 📦 Available Mobjects

Animat supports several types of graphical objects (Mobjects) that can be animated:

### **Circle** ⭕

Circular shapes with radius and positioning control.

- **Properties**: center position, radius, color, opacity
- **Interactions**: Draggable, selectable, animatable
- **Usage**: Basic geometric shapes, markers, orbital objects

### **Line** 📏

Basic line segments with customizable appearance.

- **Properties**: start/end points, thickness, color
- **Interactions**: Full drag-and-drop support, selection
- **Usage**: Connectors, basic geometric constructions

### **Vector** ➡️

Line objects with arrow tips for directional representation.

- **Properties**: start/end points, thickness, color, arrow tip
- **Interactions**: Full drag-and-drop support, selection
- **Usage**: Force vectors, directional indicators, mathematical vectors

### **MText** 📝

Full-featured text objects with comprehensive formatting.

- **Properties**: text content, font size, weight, family, color
- **Interactions**: Click to select, drag to move, animated positioning
- **Usage**: Labels, titles, annotations, mathematical expressions

### **Rectangle** ▭

Rectangular shapes with size and position control.

- **Properties**: size, position, color, corner radius
- **Interactions**: Full transformation support
- **Usage**: Frames, backgrounds, geometric constructions

### **Polygon** ▲

Multi-sided shapes with customizable vertices.

- **Properties**: vertex positions, fill color, stroke, border thickness
- **Interactions**: Vertex manipulation, drag-and-drop
- **Usage**: Custom shapes, geometric constructions

### **Dot** ⚫

Small circular markers for precise positioning.

- **Properties**: position, size, color
- **Interactions**: Draggable, selectable
- **Usage**: Point markers, graph plotting, particle systems

### **Curve** 〰️

Mathematical curves with parametric equations.

- **Properties**: curve function, parameter range, thickness
- **Usage**: Mathematical visualizations, smooth paths

### **Plane** 🏁

Coordinate system and grid representation.

- **Properties**: grid spacing, axis visibility, color
- **Usage**: Mathematical coordinate systems, reference grids

### **Angle** 📐

Geometric angle representation with two vectors and arc.

- **Properties**: angle value, vector lengths, arc radius, colors
- **Interactions**: Vector manipulation, angle adjustment
- **Usage**: Geometric angle visualization, educational content

### **Group** 🔗

Standard container for managing multiple objects together.

- **Purpose**: Group objects for collective transformations
- **Properties**: Shared transformations applied to all members
- **Usage**: Complex object hierarchies

### **VGroup** 🎯

Virtual grouping container for SimpleLine and SimpleDot objects with shared properties.

- **Purpose**: Efficiently manage multiple similar objects with shared visual properties
- **Members**: Works specifically with SimpleLine, SimpleDot, SimpleVector for optimal performance
- **Shared Properties**:
  - `sharedColor` - Applied to all group members
  - `sharedThickness` - Applied to all SimpleLine/SimpleVector members
  - `sharedSize` - Applied to all SimpleDot members
- **Individual Properties**: Each member maintains its own position
- **Automatic Sync**: Adding members automatically applies shared properties

**VGroup Usage Examples:**

```cpp
// Create particle system with SimpleDot
VGroup* particleGroup = new VGroup(parent);
particleGroup->setSharedColor(Qt::red);
particleGroup->setSharedSize(8.0);

for (int i = 0; i < 20; ++i) {
    SimpleDot* particle = new SimpleDot(parent);
    particle->setPos(QPointF(i * 15, sin(i) * 20));
    particleGroup->addMember(particle);  // Auto-applies color & size
}

// Create vector field with SimpleVector
VGroup* vectorField = new VGroup(parent);
vectorField->setSharedColor(Qt::blue);
vectorField->setSharedThickness(2.0);

for (int i = 0; i < 10; ++i) {
    SimpleVector* vector = new SimpleVector(parent);
    vector->setP1(QPointF(i * 30, 0));
    vector->setP2(QPointF(i * 30 + 20, 15));
    vectorField->addMember(vector);  // Auto-applies color & thickness
}

// Change all members at once
particleGroup->setSharedColor(Qt::green);  // All particles turn green
vectorField->setSharedThickness(5.0);     // All lines become thicker
```

**Key Benefits:**

- **Performance**: Lightweight property management for groups of similar objects
- **Consistency**: Ensures all group members maintain visual coherence
- **Convenience**: Single property change affects all members instantly
- **Flexibility**: Individual positioning with shared visual properties

### Simple Objects (Performance Optimized)

- **SimpleLine**: Lightweight line with circular caps
- **SimpleDot**: Lightweight dot for large quantities
- **SimpleVector**: Lightweight vector for group usage
- **SimpleText**: Lightweight text rendering

All mobjects support:

- **Selection and highlighting** when clicked
- **Drag-and-drop movement** with mouse
- **Property animation** through the animation system
- **Coordinate transformation** between canvas and scene space
- **Memory management** through Qt's parent-child system

## 🏛️ Property System Architecture

### Base Properties (Extended)

Every mobject inherits these fundamental properties from `BaseProperties`:

```cpp
Q_PROPERTY(QString name)          // Mobject identifier
Q_PROPERTY(QPointF pos)          // Position in scene coordinates
Q_PROPERTY(QPointF size)         // Size dimensions
Q_PROPERTY(QColor color)         // Primary color
Q_PROPERTY(qreal opacity)        // Transparency (0.0-1.0)
Q_PROPERTY(QString type)         // Mobject type identifier
Q_PROPERTY(qreal scale)          // Scale factor (1.0 = normal size)
Q_PROPERTY(qreal rotation)       // Rotation angle in degrees
Q_PROPERTY(qreal zindex)         // Z-order for layering
```

### Specialized Properties

#### Text Properties (`TextProperties`)

Text-specific properties for font and content control:

```cpp
Q_PROPERTY(QString textValue)    // Text content
Q_PROPERTY(int fontSize)         // Font size in pixels
Q_PROPERTY(int fontWeight)       // Font weight (Normal=50, Bold=75)
Q_PROPERTY(QString fontFamily)   // Font family name
Q_PROPERTY(bool bold)           // Bold state (convenience property)
Q_PROPERTY(bool italic)         // Italic state
```

*Note: Text color is managed through the base properties `color` field for consistency across all mobject types.*

#### Line Properties (`LineProperties`)

Line-specific properties for geometric control:

```cpp
Q_PROPERTY(QPointF startPoint)   // Line start position
Q_PROPERTY(QPointF endPoint)     // Line end position
Q_PROPERTY(qreal thickness)      // Line thickness
```

#### Circle Properties (`CircleProperties`)

Circle-specific properties for radius and appearance:

```cpp
Q_PROPERTY(qreal radius)         // Circle radius
Q_PROPERTY(QPointF center)       // Circle center (mirrors pos)
```

#### Polygon Properties (`PolygonProperties`)

Polygon-specific properties for shape control:

```cpp
Q_PROPERTY(QColor borderColor)   // Border/stroke color
Q_PROPERTY(qreal thickness)      // Border thickness
```

#### Geometric Properties (`GeometricProperties`)

Container for geometric objects with specialized properties:

- **AngleProperties**: Angle value, start/end vector lengths, arc properties

### Extended Property System

- **JSON Serialization**: Complete property state persistence
- **Signal-Slot Synchronization**: Bidirectional property updates
- **QML Integration**: Direct property binding in UI components
- **Animation Targeting**: Properties can be directly animated

### Property System Workflow

#### 1. Property Initialization

When a mobject is created, it initializes its properties:

```cpp
Text::Text(Scene *canvas, QQuickItem *parent) {
    // Initialize base properties
    properties->base()->setName("Text");
    properties->base()->setColor(m_color);
    properties->base()->setType("Text");

    // Initialize text-specific properties
    properties->setText(new TextProperties(this));
    properties->text()->setTextValue(m_text);
    properties->text()->setFontSize(m_fontSize);
}
```

#### 2. Signal-Slot Connections

Properties automatically sync with mobject state using Qt's signal-slot system:

```cpp
// Property changes update mobject
connect(properties->text(), &TextProperties::textValueChanged, this, [this]() {
    setText(properties->text()->textValue());
});

// Mobject changes update properties
void Text::setText(const QString &text) {
    m_text = text;
    properties->text()->setTextValue(text);  // Keep properties in sync
    emit textChanged();
}
```

#### 3. QML Integration

Properties are exposed to QML through Q_PROPERTY declarations, enabling direct UI binding:

```qml
// In MPropertiesEditor.qml
StyledTextField {
    text: mprop && mprop.text ? mprop.text.textValue : ""
    onEditingFinished: {
        if (mprop && mprop.text)
            mprop.text.textValue = text;  // Direct property binding
    }
}
```

### Property Access Patterns

#### From C++ Code

```cpp
// Get mobject properties
MProperties* props = mobject->getProperties();

// Access base properties
props->base()->setPos(QPointF(100, 50));
QColor currentColor = props->base()->color();

// Access specialized properties
if (props->text()) {
    props->text()->setFontSize(24);
    props->text()->setTextColor(Qt::blue);
}
```

#### From QML UI

```qml
property MProperties mprop: canvas.SelectedMobject ?
    canvas.SelectedMobject.getProperties() : null

// Direct property bindings
NumberInput {
    value: mprop && mprop.text ? mprop.text.fontSize : 24
    onValueChanged: {
        if (mprop && mprop.text)
            mprop.text.fontSize = newValue;
    }
}
```

### Property Synchronization

The system maintains **bidirectional synchronization**:

1. **UI → Properties → Mobject**: User changes in the properties panel update the visual object
2. **Mobject → Properties → UI**: Programmatic changes in the mobject update the UI
3. **Animation → Properties**: Animations can target specific properties for smooth transitions

### Animation Integration

Properties can be directly animated using the animation system:

```cpp
// Animate text color
CustomScalar* colorAnim = new CustomScalar(
    "textColor",     // Property name
    Qt::white,       // Start color
    Qt::red,         // End color
    2.0             // Duration
);

// Animate font size
CustomScalar* sizeAnim = new CustomScalar(
    "fontSize",      // Property name
    12,             // Start size
    48,             // End size
    1.5             // Duration
);
```

### Property Editor UI

Each property type has dedicated UI controls in `MPropertiesEditor.qml`:

- **Text Fields**: For strings (name, textValue, fontFamily)
- **Number Inputs**: For numeric values (fontSize, fontWeight, opacity)
- **Color Pickers**: For colors (color, textColor, borderColor)
- **Point Inputs**: For 2D coordinates (pos, size, startPoint, endPoint)
- **Checkboxes**: For boolean values (bold, italic)

### Memory Management

The property system follows Qt's parent-child memory management:

```cpp
// Properties are owned by the mobject
MProperties* properties = new MProperties(this);  // 'this' = parent mobject

// Specialized properties are owned by MProperties
properties->setText(new TextProperties(this));    // 'this' = parent mobject

// Automatic cleanup when mobject is destroyed
```

### Thread Safety

All property operations occur on the main UI thread, ensuring:

- Safe QML bindings
- Consistent signal emission
- No race conditions during UI updates

### Performance Considerations

- **Lazy Loading**: Specialized properties are only created when needed
- **Change Detection**: Properties only emit signals when values actually change
- **Batch Updates**: Multiple property changes can be batched to avoid excessive redraws
- **Efficient Binding**: QML property bindings are optimized by Qt's binding system

## 🎬 Animation System

### Core Animation Architecture

#### Animation Base Class

The `Animation` class provides the foundation for all animations:

- **Abstract Design**: Pure virtual `apply()` method implemented by subclasses
- **Time Management**: `setLtime()` manages timing with validation and interpolation
- **Lifecycle Events**: `onStart()` for initialization, `onEnd()` for cleanup
- **Property System**: JSON serialization support for animation state persistence
- **Scene Integration**: Direct access to `Scene` object for coordinate systems

#### Animation Types (`Math/Animations/animTypes/`)

1. **MoveAnimation** - Smoothly moves objects between positions
2. **CreateAnimation** - Fades in objects (opacity 0→1)
3. **DestroyAnimation** - Fades out objects (opacity 1→0)
4. **CustomScalarAnimation** - Animates any numeric property
5. **CustomPointAnimation** - Animates any 2D point property
6. **ValueAnimation** - Animates abstract values linked to properties
7. **WaitAnimation** - Creates controllable delays in sequences

#### AnimPacket System

The `AnimPacket` class groups multiple animations for coordinated execution:

- **Simultaneous Execution**: Multiple animations run in parallel within same time frame
- **JSON Serialization**: Complete packet state persistence
- **Update Coordination**: Single `update()` call propagates to all contained animations
- **Memory Management**: Automatic cleanup of animation resources

#### AnimationManager

Timeline orchestration system:

- **Doubly-Linked List**: Efficient packet insertion and removal during playback
- **Time-Based Activation**: Packets activate automatically when start time reached
- **Playback Controls**: Play, pause, reset, and seek functionality
- **Dynamic Updates**: Real-time packet addition/removal during playback

## 📊 Value Tracker System

### ValueTracker Hierarchy

#### Base ValueTracker Class

Provides interactive control over animation parameters:

- **QML Integration**: Exposed as QML component for UI binding
- **Signal System**: `valueChanged()` emitted on updates for reactive programming
- **Range Support**: Optional min/max constraints for bounded values
- **Real-time Updates**: Immediate propagation to connected animations

#### Specialized Tracker Types

##### Standard ValueTracker

- **Numeric Values**: Single `qreal` value with range validation
- **Slider Integration**: Direct binding to QML slider components
- **Animation Linking**: Values can drive animation parameters in real-time

##### PtValueTracker (Point Tracker)

- **2D Coordinates**: `QPointF` values for position tracking
- **Dual-Axis Control**: Separate X/Y sliders or combined 2D input
- **Geometric Operations**: Direct integration with coordinate-based animations

##### PlaybackSlider

Special tracker for timeline control:

- **Animation Manager Binding**: Direct connection to animation playback
- **Duration Management**: Automatic duration calculation from animation packets
- **Play/Pause Controls**: User interaction with timeline progression
- **Progress Visualization**: Real-time display of animation progress

### TrackerManager

#### Management System

Centralized control for all value trackers in the scene:

- **Hash-Based Storage**: Fast O(1) lookup by tracker name
- **Type Safety**: Separate storage for value and point trackers
- **Dynamic Creation**: Runtime tracker creation via parser commands
- **JSON Persistence**: Complete tracker state serialization

#### Interactive Controls

##### Time-Triggered Sliders

- **Appear Time**: Sliders appear at specific timeline moments
- **Context-Sensitive**: Show only relevant controls during playback
- **Dual-Type Support**: Both value and point tracker controls
- **Parser Integration**: Values feed directly into animation system

## 🔧 Parser System

### Command-Based Scripting Architecture

The Parser system provides flexible command interface for animation scripting:

- **Regex-Based Matching**: Each command defined by specific regex patterns
- **CommandFactory Pattern**: Centralized command creation and validation
- **Scene Integration**: Direct access to scene objects and tracker management
- **QML Exposure**: Complete parser functionality available in QML interface

### Command Categories

#### Tracker Creation Commands

- `val name = value;` - Creates numeric value tracker
- `pval name = (x, y);` - Creates point value tracker
- `dval name = expression;` - Creates calculated value tracker
- `dpval name = expression;` - Creates calculated point tracker

#### Connection Commands

- `connect(tracker, object.property);` - Links tracker to object property
- Enables real-time property updates based on tracker values

#### Command Execution Flow

1. **Input Parsing**: Regex pattern matching against registered commands
2. **Parameter Extraction**: Capture groups extract command parameters
3. **Validation**: Type checking and parameter validation
4. **Execution**: Command execution with scene and tracker manager access
5. **Signal Emission**: Success/failure signals for UI feedback

### Script Processing

#### Multi-Command Scripts

- **Semicolon Separation**: Commands separated by semicolons
- **Comment Support**: `//` line comments ignored during parsing
- **Batch Execution**: Multiple commands executed in sequence
- **Error Handling**: Individual command failures don't stop script execution

#### Mathematical Expression Support

- **muParser Integration**: Advanced mathematical expression evaluation
- **Function Support**: Trigonometric, logarithmic, and custom functions
- **Variable Support**: Time-based variables and tracker references
- **Real-time Evaluation**: Expressions evaluated continuously during animation

## 🖥️ User Interface (`QML/`)

### Main UI Structure

- `Main.qml` - Root application window with layout management
- `Components/` - Modular, reusable UI components

#### Core Components

- **SceneManager.qml** - Main canvas area where animations are displayed
- **PropertiesToolbar.qml** - Dynamic toolbar that changes based on selected object
- **Sidebar.qml** - Object palette for adding new Mobjects
- **Menubar.qml** - Application menu and file operations
- **Taskbar.qml** - Bottom status and control bar

#### Toolbar Sections (`ToolbarSections/`)

Dynamic property panels that appear based on selected object type:

- **AppearanceToolbarSection.qml** - Universal appearance controls (color, opacity, scale, rotation)
- **PolygonToolBar.qml** - Polygon-specific controls (points, thickness, borders)
- **TextToolbarSection.qml** - Text formatting controls
- **GeometricToolbarSection.qml** - Geometric object controls (angles, measurements)
- **SpecialSection.qml** - Advanced features and animations

#### Input Components (`Elements/Input/`)

- **StyledTextField.qml** - Consistent text input with validation
- **StyledTextArea.qml** - Multi-line text input for scripts
- **NumberInput.qml** - Numeric input with range validation
- **PointInput.qml** - 2D coordinate input
- **ColorPicker.qml** - Color selection interface
- **MSlider.qml** - Custom slider with precise control

#### Tracker Components (`Elements/TrackerComponents/`)

- **ValSlider.qml** - Value tracker slider controls
- **TimeTriggeredSlider.qml** - Time-based slider activation
- **SlidersPanel.qml** - Main slider management interface
- **ValueTrackersList.qml** - List of active value trackers
- **PointTrackersList.qml** - List of active point trackers

## 🎯 Key Design Principles

### 1. Property-Based Object Identification

Instead of traditional type checking, objects are identified by which properties they possess. This makes the system highly extensible - new object types can be added by simply defining their unique property combinations.

### 2. Signal-Slot Property Synchronization

All property changes automatically update the visual representation through Qt's signal-slot system. When a property changes, connected slots update the object's appearance immediately.

### 3. Factory Pattern Object Creation

The `MobjectMap` uses factories to create objects by name, making it easy to add new object types without modifying core scene management code.

### 4. Hierarchical Properties Tree

Properties are organized in a tree structure rather than inheritance, allowing flexible composition of features without complex inheritance hierarchies.

### 5. Animation-First Design

Every object property is designed to be animatable. The animation system can smoothly interpolate between any property values over time.

### 6. Performance Optimization

Simple variants of objects (SimpleLine, SimpleDot, SimpleVector) provide lightweight alternatives for scenarios requiring many objects.

## 📁 File Structure

```text
Animat/
├── Math/
│   ├── Scene.h/.cpp                      # Main scene coordination
│   ├── Animations/
│   │   ├── AllAnimations.h              # Animation includes header
│   │   ├── animation.h/.cpp             # Base Animation class
│   │   ├── animpacket.h/.cpp            # Animation packet container
│   │   ├── animationmanager.h/.cpp      # Timeline management
│   │   └── animTypes/
│   │       ├── MoveAnimation.h/.cpp     # Position movement
│   │       ├── CreateAnimation.h/.cpp   # Fade in animation
│   │       ├── DestroyAnimation.h/.cpp  # Fade out animation
│   │       ├── CustomScalarAnimation.h/.cpp  # Numeric property animation
│   │       ├── CustomPointAnimation.h/.cpp   # Point property animation
│   │       ├── ValueAnimation.h/.cpp    # Abstract value animation
│   │       └── WaitAnimation.h/.cpp     # Delay/wait animation
│   ├── ValueTracker/
│   │   ├── ValueTracker.h/.cpp          # Base value tracking
│   │   ├── ptvaluetracker.h/.cpp        # Point value tracking
│   │   ├── playbackslider.h/.cpp        # Timeline control
│   │   ├── trackermanager.h/.cpp        # Tracker management
│   │   ├── trackerdata.h/.cpp           # Value tracker data wrapper
│   │   ├── pttrackerdata.h/.cpp         # Point tracker data wrapper
│   │   └── animationtimer.h/.cpp        # Animation timing control
│   ├── Parser/
│   │   ├── parser.h/.cpp                # Main parser interface
│   │   ├── command.h/.cpp               # Base command class
│   │   ├── CommandFactory.h             # Command creation factory
│   │   └── Commands/
│   │       ├── makevalcommand.h/.cpp    # Value tracker creation
│   │       ├── makeptvalcommand.h/.cpp  # Point tracker creation
│   │       ├── dvalcommand.h/.cpp       # Dynamic value tracker
│   │       ├── dpvalcommand.h/.cpp      # Dynamic point tracker
│   │       └── connectcommand.h/.cpp    # Property connection
│   ├── Mobjects/
│   │   ├── Base/
│   │   │   ├── Mobject.h/.cpp           # Core mobject foundation
│   │   │   └── ClickableMobject.h/.cpp  # Interactive base class
│   │   ├── Basic/
│   │   │   ├── Circle.h/.cpp            # Circular shapes
│   │   │   ├── line.h/.cpp              # Basic line segments
│   │   │   ├── Vector.h/.cpp            # Lines with arrow tips
│   │   │   ├── Dot.h/.cpp               # Small circular markers
│   │   │   ├── MRectangle.h/.cpp        # Rectangular shapes
│   │   │   └── Polygon.h/.cpp           # Multi-sided shapes
│   │   ├── Simple/
│   │   │   ├── SimpleLine.h/.cpp        # Optimized line
│   │   │   ├── SimpleDot.h/.cpp         # Optimized dot
│   │   │   ├── SimpleVector.h/.cpp      # Optimized vector
│   │   │   └── SimpleText.h/.cpp        # Optimized text
│   │   ├── Complex/
│   │   │   ├── Curve.h/.cpp             # Mathematical curves
│   │   │   ├── MPolygon.h/.cpp          # Dynamic polygon
│   │   │   └── plane.h/.cpp             # Coordinate system
│   │   ├── Groups/
│   │   │   ├── group.h/.cpp             # Standard grouping
│   │   │   └── VGroup.h/.cpp            # Virtual grouping
│   │   ├── MText/
│   │   │   └── MText.h/.cpp             # Full-featured text
│   │   └── Geometric/
│   │       └── Angle.h/.cpp             # Geometric angle
│   ├── Helper/
│   │   ├── mproperties.h/.cpp           # Property management
│   │   ├── funcs.h/.cpp                 # Utility functions
│   │   └── Properties/
│   │       ├── baseproperties.h/.cpp    # Common properties
│   │       ├── lineproperties.h/.cpp    # Line-specific
│   │       ├── circleproperties.h/.cpp  # Circle-specific
│   │       ├── textproperties.h/.cpp    # Text-specific
│   │       ├── polygonproperties.h/.cpp # Polygon-specific
│   │       ├── curveproperties.h/.cpp   # Curve-specific
│   │       ├── geometricproperties.h/.cpp # Geometric container
│   │       └── Geometric/
│   │           ├── angleproperties.h/.cpp   # Angle properties
│   │           └── planeproperties.h/.cpp   # Plane properties
│   └── Managers/
│       └── funcmap.h/.cpp               # Function mapping utilities
├── QML/
│   ├── Main.qml                         # Root application window
│   └── Components/
│       ├── SceneManager.qml             # Main canvas
│       ├── PropertiesToolbar.qml        # Dynamic property toolbar
│       ├── Sidebar.qml                  # Object palette
│       ├── Menubar.qml                  # Application menu
│       ├── Taskbar.qml                  # Bottom control bar
│       ├── TitleBar.qml                 # Window title bar
│       ├── ToolbarSections/
│       │   ├── AppearanceToolbarSection.qml  # Appearance controls
│       │   ├── PolygonToolBar.qml            # Polygon controls
│       │   ├── TextToolbarSection.qml        # Text formatting
│       │   ├── GeometricToolbarSection.qml   # Geometric controls
│       │   └── SpecialSection.qml            # Advanced features
│       └── Elements/
│           ├── MpropertiesEditor.qml    # Modular property editor
│           ├── ValueManagement.qml      # Tracker management
│           ├── ActiveAnimationsList.qml # Animation list
│           ├── Input/
│           │   ├── StyledTextField.qml  # Text input
│           │   ├── StyledTextArea.qml   # Multi-line input
│           │   ├── NumberInput.qml      # Numeric input
│           │   ├── PointInput.qml       # 2D coordinate input
│           │   ├── ColorPicker.qml      # Color selection
│           │   ├── MSlider.qml          # Custom slider
│           │   ├── AnimInput.qml        # Animation input
│           │   └── PlaybackInput.qml    # Playback controls
│           └── TrackerComponents/
│               ├── ValSlider.qml        # Value slider
│               ├── TimeTriggeredSlider.qml   # Time-based slider
│               ├── SlidersPanel.qml     # Slider management
│               ├── ValueTrackersList.qml     # Value tracker list
│               └── PointTrackersList.qml     # Point tracker list
├── Utils/
│   ├── mobjectmap.h/.cpp                # Object factory mapping
│   ├── mobjectregistry.h/.cpp           # Object registration
│   ├── filehandler.h/.cpp               # File I/O operations
│   ├── Constants.h                      # Application constants
│   └── Singleton.h                      # Singleton pattern utility
├── lib/                                 # muParser mathematical library
├── CMakeLists.txt                       # Build configuration
└── README.md                            # This documentation
```

## 🏗️ Building and Running

### Requirements

- Qt 6.9.1 or later
- CMake 3.16 or later
- MinGW 64-bit compiler (Windows)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running

```bash
./build/Desktop_Qt_6_9_1_MinGW_64_bit-Debug/appAnimat.exe
```

Alternatively, use the provided VS Code tasks:

1. **Build and Copy DLLs** - Compiles project and copies required Qt libraries
2. **Run Application** - Launches the application with proper environment setup

## 🎨 Development Philosophy

Animat aims to make mathematical animation accessible to non-programmers while maintaining the power and flexibility that developers need. The visual interface removes the coding barrier while the underlying architecture supports complex animations and custom extensions.

The framework prioritizes:

- **Intuitive UI/UX** - Drag-and-drop object creation, visual property editing
- **Real-time feedback** - Immediate visual updates as properties change
- **Extensibility** - Clean architecture for adding new objects and animations
- **Performance** - Qt6/QML rendering for smooth 60fps animations
- **Flexibility** - Both GUI and code-based workflows supported

## 🤝 Contributing

When adding new features:

### Adding New Mobject Types

1. Create new class in appropriate `Math/Mobjects/` subdirectory
2. Define specialized properties if needed in `Math/Helper/Properties/`
3. Register in `MobjectMap::init()` and `MobjectRegistry`
4. Add UI support in toolbar sections
5. Update property editor for new property types

### Adding New Animation Types

1. Define class in `Math/Animations/animTypes/`
2. Implement in corresponding `.cpp` file
3. Add case in `AnimPacket::addAnimation()`
4. Update UI components for new animation parameters

### Adding New Properties

1. Create property class in `Math/Helper/Properties/`
2. Add to `MProperties` container
3. Update JSON serialization methods
4. Add UI controls in property editor

---

*Built with Qt 6.9.1 and QML for cross-platform mathematical animation creation.*