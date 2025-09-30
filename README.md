# Animat - Animation Framework

A Qt-based animation framework for creating and managing complex animation sequences with an intuitive QML interface.

## Overview

Animat provides a comprehensive animation system that allows you to create, sequence, and playback various types of animations on graphical objects (Mobjects). The system features dynamic timing, interactive controls, and a modular architecture.

## Architecture

### Core Components

- **Animation System** (`Math/Animations/`)
- **Scene Management** (`Math/Scene.cpp`)
- **UI Components** (`QML/Components/`)
- **Value Tracking** (`Math/ValueTracker/`)

## Available Mobjects

Animat supports several types of graphical objects (Mobjects) that can be animated:

### **Text** 📝

Interactive text objects that can be moved, selected, and dragged.

- **Properties**: text content, font size, color
- **Interactions**: Click to select, drag to move, animated positioning
- **Usage**: Labels, titles, annotations

### **Line** 📏

Drawable line segments with customizable appearance.

- **Properties**: start/end points, thickness, color
- **Interactions**: Full drag-and-drop support, selection
- **Usage**: Connectors, arrows, geometric shapes

### **Circle** ⭕

Circular shapes with radius and positioning control.

- **Properties**: center position, radius, color, opacity
- **Interactions**: Draggable, selectable, animatable

### **Rectangle** ▭

Rectangular shapes with size and position control.

- **Properties**: size, position, color, corner radius
- **Interactions**: Full transformation support

### **Curve** 〰️

Mathematical curves with parametric equations.

- **Properties**: curve function, parameter range, thickness
- **Usage**: Mathematical visualizations, smooth paths

### **Polygon** ▲

Multi-sided shapes with customizable vertices.

- **Properties**: vertex positions, fill color, stroke
- **Usage**: Custom shapes, geometric constructions

### **VGroup** 🎯

Virtual grouping container for SimpleLine and SimpleDot objects with shared properties.

- **Purpose**: Efficiently manage multiple similar objects with shared visual properties
- **Members**: Works specifically with SimpleLine and SimpleDot for optimal performance
- **Shared Properties**: 
  - `sharedColor` - Applied to all group members
  - `sharedThickness` - Applied to all SimpleLine members
  - `sharedSize` - Applied to all SimpleDot members
- **Individual Properties**: Each member maintains its own position
- **Automatic Sync**: Adding members automatically applies shared properties

**Usage Examples:**

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

// Create line segments with SimpleLine
VGroup* gridGroup = new VGroup(parent);
gridGroup->setSharedColor(Qt::blue);
gridGroup->setSharedThickness(2.0);

for (int i = 0; i < 10; ++i) {
    SimpleLine* line = new SimpleLine(parent);
    line->setP1(QPointF(i * 30, 0));
    line->setP2(QPointF(i * 30, 300));
    gridGroup->addMember(line);  // Auto-applies color & thickness
}

// Change all members at once
particleGroup->setSharedColor(Qt::green);  // All particles turn green
gridGroup->setSharedThickness(5.0);       // All lines become thicker
```

**Key Benefits:**
- **Performance**: Lightweight property management for groups of similar objects
- **Consistency**: Ensures all group members maintain visual coherence
- **Convenience**: Single property change affects all members instantly
- **Flexibility**: Individual positioning with shared visual properties

All mobjects support:

- **Selection and highlighting** when clicked
- **Drag-and-drop movement** with mouse
- **Property animation** through the animation system
- **Coordinate transformation** between canvas and scene space
- **Memory management** through Qt's parent-child system

## Object and Properties System

### Architecture Overview

Animat uses a sophisticated property management system that separates visual objects (Mobjects) from their animatable properties. This allows for clean separation of concerns, easy UI binding, and flexible animation control.

### Property Management Structure

```
ClickableMobject
    └── MProperties (central property manager)
        ├── BaseProperties (common to all mobjects)
        ├── LineProperties (line-specific properties)
        ├── CircleProperties (circle-specific properties)
        ├── TextProperties (text-specific properties)
        ├── PolygonProperties (polygon-specific properties)
        └── CurveProperties (curve-specific properties)
```

### Base Properties

Every mobject inherits these fundamental properties from `BaseProperties`:

```cpp
Q_PROPERTY(QString name)          // Mobject identifier
Q_PROPERTY(QPointF pos)          // Position in scene coordinates
Q_PROPERTY(QPointF size)         // Size dimensions
Q_PROPERTY(QColor color)         // Primary color
Q_PROPERTY(qreal opacity)        // Transparency (0.0-1.0)
Q_PROPERTY(QString type)         // Mobject type identifier
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

_Note: Text color is managed through the base properties `color` field for consistency across all mobject types._

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

### Property System Workflow

#### 1. **Property Initialization**

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

#### 2. **Signal-Slot Connections**

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

#### 3. **QML Integration**

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

## Animation Types

### 1. **Move Animation**

Smoothly moves a mobject from a start position to a target position.

**Parameters:**

- `startPos`: Starting position (x,y)
- `targetPos`: Target position (x,y)
- `duration`: Animation duration in seconds

**Example:** Move a circle from (0,0) to (100,100) over 2 seconds.

### 2. **Create Animation**

Fades in a mobject by animating its opacity from 0 to 1.

**Parameters:**

- `duration`: Fade-in duration in seconds

**Use case:** Smoothly introduce new objects into the scene.

### 3. **Destroy Animation**

Fades out a mobject by animating its opacity from 1 to 0.

**Parameters:**

- `duration`: Fade-out duration in seconds

**Use case:** Smoothly remove objects from the scene.

### 4. **CustomScalar Animation**

Animates any scalar (numeric) property of a mobject.

**Parameters:**

- `property`: Property name (e.g., "radius", "thickness")
- `startVal`: Starting value
- `targetVal`: Target value
- `duration`: Animation duration

**Example:** Animate a circle's radius from 10 to 50.

### 5. **CustomPoint Animation**

Animates any point (x,y) property of a mobject.

**Parameters:**

- `property`: Property name (e.g., "center", "size")
- `startVal`: Starting point (x,y)
- `targetVal`: Target point (x,y)
- `duration`: Animation duration

**Example:** Animate a rectangle's size from (50,30) to (100,80).

### 6. **Value Animation**

Animates abstract values that can be connected to multiple mobject properties.

**Parameters:**

- `startVal`: Starting value
- `endVal`: Ending value
- `duration`: Animation duration

**Use case:** Drive multiple properties with a single animated value.

### 7. **Wait Animation** 🆕

Creates a controllable delay/gap between animations.

**Parameters:**

- `duration`: Wait duration in seconds

**Use case:** Add precise timing gaps in animation sequences.

## Animation Script Parser 📝

Animat includes a powerful script parser that allows you to create complex animations and value tracking using a custom scripting language. The parser supports various command types for creating dynamic, data-driven animations.

### Parser Command Types

#### 1. **Value Tracker (`val`)**

Creates a scalar value tracker with either static values or mathematical expressions.

**Syntax:**
```
val name = value;
val name = expression;
```

**Examples:**
```
val radius = 25;
val angle = sin(t * 2);
val speed = cos(t) * 10 + 15;
val growth = t * 0.5;
```

**Use Cases:**
- Static configuration values
- Time-dependent calculations
- Mathematical function evaluation
- Animation parameter control

#### 2. **Point Value Tracker (`pval`)**

Creates a 2D point tracker with x,y coordinates, supporting both static points and parametric expressions.

**Syntax:**
```
pval name = (x, y);
pval name = (x_expression, y_expression);
```

**Examples:**
```
pval center = (100, 50);
pval orbit = (cos(t * 2) * 50, sin(t * 2) * 50);
pval wave = (t * 10, sin(t * 0.5) * 30);
pval spiral = (t * cos(t), t * sin(t));
```

**Use Cases:**
- Object positioning
- Parametric motion paths
- Circular/orbital movement
- Wave and spiral patterns

#### 3. **Dynamic Value Tracker (`dval`)**

Creates values that depend on other tracked values using bracket notation for references.

**Syntax:**
```
dval name = expression_with_[references];
```

**Examples:**
```
dval combined = [radius] * 2 + [speed];
dval scaled = [orbit_x] * [scale_factor];
dval distance = sqrt([x1] * [x1] + [y1] * [y1]);
dval interpolated = [start] + ([end] - [start]) * [progress];
```

**Use Cases:**
- Value composition and combination
- Calculated properties based on other values
- Complex mathematical relationships
- Multi-stage value processing

#### 4. **Dynamic Point Tracker (`dpval`)**

Creates 2D points that depend on other tracked values.

**Syntax:**
```
dpval name = (x_expression_with_[refs], y_expression_with_[refs]);
```

**Examples:**
```
dpval adjusted_center = ([base_x] + [offset_x], [base_y] + [offset_y]);
dpval scaled_position = ([pos_x] * [scale], [pos_y] * [scale]);
dpval relative_point = ([anchor_x] + [radius] * cos([angle]), [anchor_y] + [radius] * sin([angle]));
```

**Use Cases:**
- Dependent positioning
- Coordinate transformations
- Relative positioning systems
- Complex motion combinations

#### 5. **Connection Commands (`connect`)**

Links tracked values to mobject properties for automatic updates.

**Syntax:**
```
connect(tracker_name, object.property);
```

**Examples:**
```
connect(radius, circle.radius);
connect(center, rectangle.pos);
connect(rotation_angle, text.rotation);
connect(wave_position, line.startPoint);
```

**Supported Properties:**
- `pos` - Position (requires point tracker)
- `size` - Size dimensions (requires point tracker)
- `color` - Color (requires color tracker)
- `opacity` - Transparency (0.0-1.0)
- `scale` - Scale factor
- `rotation` - Rotation angle
- `radius` - Circle radius
- `thickness` - Line thickness
- Type-specific properties

#### 6. **Loop Commands (`loop`)** 🆕

Creates iterative command blocks for generating multiple similar trackers or connections.

**Syntax:**
```
loop (iterator_name:start_number->end_number) {
    // Commands using [iterator_name] as variable
}
```

**Validation Rules:**
- ✅ `start_number < end_number` (start must be less than end)
- ✅ Maximum 50 iterations `(end - start) <= 50`
- ✅ Iterator name must be valid identifier
- ✅ Loop body cannot be empty

**Examples:**
```
// Create multiple particles
loop (i:0->10) {
    val x_[i] = [i] * 20;
    val y_[i] = sin([i] * 0.1) * 50;
    pval pos_[i] = ([x_[i]], [y_[i]]);
    connect(pos_[i], particle_[i].pos);
}

// Generate wave points
loop (frame:1->30) {
    val amplitude_[frame] = sin([frame] * 0.2) * 100;
    pval wave_[frame] = ([frame] * 5, [amplitude_[frame]]);
    connect(wave_[frame], point_[frame].pos);
}

// Create rotation sequence
loop (step:0->12) {
    val angle_[step] = [step] * 30; // 30 degree increments
    val x_[step] = cos([angle_[step]]) * 50;
    val y_[step] = sin([angle_[step]]) * 50;
    dpval rotated_[step] = ([x_[step]], [y_[step]]);
}
```

**Use Cases:**
- Particle systems generation
- Repetitive pattern creation
- Array-like value management
- Procedural animation sequences

### Expression Syntax

The parser supports mathematical expressions with standard operators and functions:

#### **Operators:**
- Arithmetic: `+`, `-`, `*`, `/`, `^` (power)
- Comparison: `<`, `>`, `<=`, `>=`, `==`, `!=`
- Logical: `&&`, `||`, `!`

#### **Functions:**
- Trigonometric: `sin()`, `cos()`, `tan()`, `asin()`, `acos()`, `atan()`
- Mathematical: `sqrt()`, `log()`, `exp()`, `abs()`, `floor()`, `ceil()`
- Constants: `pi`, `e`

#### **Variables:**
- `t` - Current time variable
- `[tracker_name]` - Reference to other tracker values
- Numbers: `123`, `3.14`, `-0.5`

### Script Examples

#### **Basic Animation Setup:**
```
// Static configuration
val center_x = 200;
val center_y = 150;
val orbit_radius = 75;

// Time-based movement
val angle = t * 1.5;
pval orbit_pos = ([center_x] + [orbit_radius] * cos([angle]), 
                  [center_y] + [orbit_radius] * sin([angle]));

// Connect to objects
connect(orbit_pos, planet.pos);
connect(angle, planet.rotation);
```

#### **Complex Wave System:**
```
// Wave parameters
val frequency = 2.0;
val amplitude = 50;
val speed = 1.5;

// Generate wave points
loop (i:0->20) {
    val x_[i] = [i] * 10;
    val y_[i] = [amplitude] * sin([frequency] * [i] + t * [speed]);
    pval wave_point_[i] = ([x_[i]], [y_[i]]);
    connect(wave_point_[i], wave_segment_[i].pos);
}
```

#### **Interactive Particle System:**
```
// Particle configuration
val particle_count = 15;
val explosion_force = 100;

loop (p:0->15) {
    // Individual particle parameters
    val angle_[p] = [p] * 24; // Spread evenly in circle
    val speed_[p] = [explosion_force] * (0.5 + [p] * 0.1);
    
    // Calculate positions
    val x_[p] = [speed_[p]] * cos([angle_[p]]) * t;
    val y_[p] = [speed_[p]] * sin([angle_[p]]) * t - 9.8 * t * t / 2; // Gravity
    
    dpval particle_pos_[p] = ([x_[p]], [y_[p]]);
    connect(particle_pos_[p], particle_[p].pos);
    
    // Fade out over time
    val opacity_[p] = 1.0 - t * 0.3;
    connect(opacity_[p], particle_[p].opacity);
}
```

### Parser Features

#### **Dependency Resolution:**
The parser automatically detects dependencies between trackers and ensures proper evaluation order.

#### **Error Handling:**
- Syntax validation with detailed error reporting
- Circular dependency detection
- Type checking for expressions
- Range validation for loops

#### **Performance Optimization:**
- Efficient expression evaluation
- Cached dependency graphs
- Optimized update cycles
- Memory-efficient storage

#### **Integration:**
- Seamless QML property binding
- Real-time expression evaluation
- Dynamic tracker creation/destruction
- Animation timeline synchronization

### Comments and Formatting

The parser supports:
- **Line comments:** `// This is a comment`
- **Flexible whitespace:** Commands can span multiple lines
- **Case sensitivity:** All keywords and names are case-sensitive
- **Statement termination:** Most commands end with semicolon `;`

### Parser Usage in Code

```cpp
// Parse animation script
AnimationScriptParser parser;
QVector<TrackerCommand> commands = parser.parseScript(scriptText);

// Process commands
for (const TrackerCommand& cmd : commands) {
    switch (cmd.getType()) {
        case TrackerCommand::VALUE_TRACKER:
            // Create value tracker
            break;
        case TrackerCommand::LOOP:
            // Process loop commands
            break;
        // ... handle other types
    }
}
```

The animation script parser provides a powerful, flexible way to create complex, data-driven animations with clean, readable syntax.

## Animation System Workflow

### 1. **Animation Creation**

```
User Input → AnimationTypeSelector → AnimationFieldsPanel → Validation
```

### 2. **Animation Processing**

```
Parameters → AnimPacket.addAnimation() → Animation Object Creation
```

### 3. **Timeline Management**

```
AnimationManager → Doubly Linked List → Sorted by Start Time
```

### 4. **Playback Control**

```
PlaybackSlider → Dynamic Duration Calculation → Time Updates → Animation.apply()
```

## Key Features

### Dynamic Duration System

- **Automatic calculation** of total timeline duration based on all animations
- **Real-time updates** when animations are added/removed
- **Minimum duration** fallback (5 seconds) when no animations exist

### Smart Timeline Management

- **Sorted insertion** of animation packets by start time
- **Efficient navigation** through animation sequence
- **Proper cleanup** and memory management

### Interactive Controls

- **Play/Pause/Reset** functionality
- **Scrub through timeline** with interactive slider
- **Jump to specific animations** in the sequence

### Easing Support

- Built-in easing curves for smooth animations
- Default: `QEasingCurve::InOutQuad`
- Customizable per animation type

## Usage Examples

### Creating a Simple Animation Sequence

1. **Create a Move Animation:**

   - Select "Move" from animation type
   - Choose target mobject
   - Set start position: "0,0"
   - Set target position: "100,100"
   - Duration: "2" seconds

2. **Add a Wait Period:**

   - Select "Wait" from animation type
   - Set duration: "1" second
   - Start offset: "2" seconds

3. **Add a Destroy Animation:**
   - Select "Destroy" from animation type
   - Choose same mobject
   - Duration: "1" second
   - Start offset: "3" seconds

**Result:** Object moves (0-2s) → waits (2-3s) → fades out (3-4s)

### Advanced Sequencing

```
Timeline: 0s────2s────4s────6s────8s
         Move   Wait  Scale  Wait  Destroy
```

## File Structure

```
Animat/
├── Math/
│   ├── Animations/
│   │   ├── animation.h               # Base Animation class & all animation types
│   │   ├── animation.cpp             # Animation implementations
│   │   ├── animpacket.h              # Animation packet container
│   │   ├── animpacket.cpp            # Packet management
│   │   ├── animationmanager.h        # Timeline management
│   │   └── animationmanager.cpp      # Manager implementation
│   ├── ValueTracker/
│   │   ├── playbackslider.h          # Dynamic playback control
│   │   ├── playbackslider.cpp        # Playback implementation
│   │   ├── valuetracker.h            # Value tracking system
│   │   ├── valuetracker.cpp          # Value tracker implementation
│   │   └── ptvaluetracker.h          # Point value tracker
│   ├── Parser/                       # 🆕 Animation Script Parser
│   │   ├── animationscriptparser.h   # Main parser class
│   │   ├── animationscriptparser.cpp # Parser implementation
│   │   ├── trackercommand.h          # Command definitions
│   │   ├── trackercommand.cpp        # Command implementations
│   │   ├── trackermanager.h          # Tracker management
│   │   ├── trackermanager.cpp        # Manager implementation
│   │   ├── parser.h                  # Expression parser
│   │   ├── parser.cpp                # Expression evaluation
│   │   └── dependencygraph.h         # Dependency resolution
│   ├── Mobjects/                     # Graphical Objects
│   │   ├── Mobject.h                 # Base mobject class
│   │   ├── Group.h                   # Object grouping
│   │   ├── VGroup.h                  # 🆕 Virtual grouping (shared properties)
│   │   ├── Circle.h                  # Circle objects
│   │   ├── Text.h                    # Text objects
│   │   ├── Line.h                    # Line objects
│   │   ├── SimpleLine.h              # 🆕 Optimized line with circular caps
│   │   ├── Polygon.h                 # Polygon objects
│   │   ├── MPolygon.h                # 🆕 Dynamic polygon with runtime editing
│   │   ├── MRectangle.h              # Rectangle objects
│   │   ├── Curve.h                   # Mathematical curves
│   │   └── Dot.h                     # 🆕 Small circular markers
│   ├── Helper/
│   │   ├── ClickableMobject.h        # Interactive mobject base
│   │   ├── mproperties.h             # Property management
│   │   └── Properties/               # Specialized property types
│   │       ├── baseproperties.h      # Common properties
│   │       ├── textproperties.h      # Text-specific properties
│   │       ├── lineproperties.h      # Line-specific properties
│   │       ├── circleproperties.h    # Circle-specific properties
│   │       ├── polygonproperties.h   # Polygon-specific properties
│   │       └── curveproperties.h     # Curve-specific properties
│   └── Scene.cpp                     # Main scene coordination
├── QML/Components/
│   ├── Elements/
│   │   ├── Input/
│   │   │   ├── AnimationTypeSelector.qml     # Animation type dropdown
│   │   │   ├── AnimationFieldsPanel.qml      # Parameter input fields
│   │   │   ├── AnimInput.qml                 # Main animation input UI
│   │   │   ├── PlaybackInput.qml             # Playback controls UI
│   │   │   ├── StyledTextField.qml           # 🆕 Consistent text input
│   │   │   ├── StyledTextArea.qml            # 🆕 Multi-line text input
│   │   │   ├── ColorPicker.qml               # Color selection
│   │   │   ├── NumberInput.qml               # Numeric input with validation
│   │   │   └── PointInput.qml                # 2D coordinate input
│   │   ├── PropertyPanels/                   # 🆕 Modular property editors
│   │   │   ├── MobjectIdPanel.qml           # ID display and delete
│   │   │   ├── MobjectNamePanel.qml         # Name editing
│   │   │   ├── TransformPanel.qml           # Position and size
│   │   │   ├── AppearancePanel.qml          # Color properties
│   │   │   ├── NumericPanel.qml             # Numeric properties
│   │   │   └── PolygonPanel.qml             # 🆕 Polygon point management
│   │   ├── MpropertiesEditor.qml             # 🆕 Modular property editor
│   │   ├── ValueManagement.qml               # Value tracker management
│   │   └── ActiveAnimationsList.qml          # Animation list display
│   ├── TitleBar.qml                          # Application title bar
│   ├── Taskbar.qml                           # Main toolbar
│   ├── ControlPanel.qml                      # Control panel UI
│   └── SceneManager.qml                      # Scene management UI
├── Utils/                                    # Utility classes
│   ├── mobjectmap.h                          # Object mapping utilities
│   ├── mobjectregistry.h                     # Object registration system
│   ├── filehandler.h                         # File I/O operations
│   ├── Constants.h                           # Application constants
│   └── Singleton.h                           # Singleton pattern utility
├── CMakeLists.txt                            # Build configuration
└── README.md                                 # This documentation
```

## Technical Details

### Animation Base Class

All animations inherit from the `Animation` base class, providing:

- **Normalized time** (0.0 to 1.0) calculation
- **Easing curve** application
- **Start offset** and **duration** management
- **Abstract apply()** method for subclass implementation

### Memory Management

- **Qt parent-child** system for automatic cleanup
- **Smart pointer** usage where appropriate
- **Proper disconnection** of signals when objects are destroyed

### Performance Considerations

- **Efficient sorting** using doubly linked list
- **Minimal allocations** during playback
- **Optimized rendering** through Qt's scene graph

## Building and Running

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

## Contributing

When adding new animation types:

1. **Define class** in `animation.h`
2. **Implement** in `animation.cpp`
3. **Add case** in `AnimPacket::addAnimation()`
4. **Update type detection** in `getAnimationTypeName()`
5. **Add UI support** in `AnimationTypeSelector.qml`
6. **Update field visibility** in `AnimationFieldsPanel.qml`
7. **Test thoroughly** with various parameter combinations

## Future Enhancements

- [ ] **Keyframe animations** with multiple control points
- [ ] **Animation curves** with custom bezier controls
- [ ] **Group animations** for complex object hierarchies
- [ ] **Timeline editor** with visual timeline representation
- [ ] **Animation templates** for common sequences
- [ ] **Export/Import** functionality for animation data

---

_Built with Qt 6.9.1 and QML for cross-platform animation creation._
