# Animat - Animation Framework# Animat - Animation Framework

A powerful Qt6 QML-based animation framework inspired by Manim, designed for creating mathematical and geometric animations with an intuitive user interface.A Qt-based animation framework for creating and managing complex animation sequences with an intuitive QML interface.

## Overview## Overview

Animat provides a comprehensive animation system that allows you to create, sequence, and playback various types of animations on graphical objects (Mobjects). The system features dynamic timing, interactive controls, modular architecture, and a sophisticated value tracking system for data-driven animations.Animat provides a comprehensive animation system that allows you to create, sequence, and playback various types of animations on graphical objects (Mobjects). The system features dynamic timing, interactive controls, and a modular architecture.

## Architecture Overview## Architecture

### 1. Mobject Hierarchy (`Math/Mobjects/`)### Core Components

The foundation of the animation system is built around **Mobjects** (Mathematical Objects) with a clear inheritance hierarchy:- **Animation System** (`Math/Animations/`)

- **Scene Management** (`Math/Scene.cpp`)

```- **UI Components** (`QML/Components/`)

Mobject (Base QQuickItem)- **Value Tracking** (`Math/ValueTracker/`)

└── ClickableMobject (Adds interactivity, properties, scene integration)

    ├── Base/## Available Mobjects

    │   └── Mobject                   # Core mobject foundation

    ├── Basic/Animat supports several types of graphical objects (Mobjects) that can be animated:

    │   ├── Circle                    # Circular shapes with radius control

    │   ├── Line                      # Basic line segments with arrow tips### **Text** 📝

    │   ├── Vector                    # Line with arrow tip for directional representation

    │   ├── Dot                       # Small circular markersInteractive text objects that can be moved, selected, and dragged.

    │   ├── MRectangle                # Rectangular shapes

    │   └── Polygon                   # Multi-sided shapes with customizable vertices- **Properties**: text content, font size, color

    ├── Simple/- **Interactions**: Click to select, drag to move, animated positioning

    │   ├── SimpleLine                # Optimized line with circular caps- **Usage**: Labels, titles, annotations

    │   ├── SimpleDot                 # Lightweight dot for performance

    │   ├── SimpleVector              # Lightweight vector for groups### **Line** 📏

    │   └── SimpleText                # Lightweight text rendering

    ├── Complex/Drawable line segments with customizable appearance.

    │   ├── Curve                     # Mathematical curves with parametric equations

    │   ├── MPolygon                  # Dynamic polygon with runtime editing- **Properties**: start/end points, thickness, color

    │   └── Plane                     # Coordinate system and grid- **Interactions**: Full drag-and-drop support, selection

    ├── Groups/- **Usage**: Connectors, arrows, geometric shapes

    │   ├── Group                     # Standard object grouping

    │   └── VGroup                    # Virtual grouping with shared properties### **Circle** ⭕

    ├── MText/

    │   └── MText                     # Full-featured text with fonts and formattingCircular shapes with radius and positioning control.

    └── Geometric/

        └── Angle                     # Geometric angle representation with arc- **Properties**: center position, radius, color, opacity

**Key Features:**### **Rectangle** ▭

- Each object has a unique ID and maintains its state through JSON serialization

- Property-based type identification systemRectangular shapes with size and position control.

- Hierarchical organization for different complexity levels

- **Properties**: size, position, color, corner radius

### 2. Properties System (`Math/Helper/Properties/`)- **Interactions**: Full transformation support

**Main Properties Container:**### **Curve** 〰️

- `MProperties` - Central property container with specialized sub-properties

Mathematical curves with parametric equations.

**Property Types:**

- `BaseProperties` - Common to all mobjects (position, color, opacity, scale, rotation, z-index)- **Properties**: curve function, parameter range, thickness

- `LineProperties` - Line-specific parameters (start/end points, thickness)- **Usage**: Mathematical visualizations, smooth paths

- `CircleProperties` - Circle-specific parameters (radius, center)

- `TextProperties` - Font, size, weight, family for text objects### **Polygon** ▲

- `PolygonProperties` - Polygon-specific parameters (border color, thickness)

- `CurveProperties` - Curve-specific parametersMulti-sided shapes with customizable vertices.

- `GeometricProperties` - Geometric objects (contains AngleProperties for angles)

- **Properties**: vertex positions, fill color, stroke

Objects are identified by their **property existence** - each Mobject constructor initializes only the properties it needs, making object type identification possible by checking which properties exist.- **Usage**: Custom shapes, geometric constructions

### 3. Scene Management (`Math/Scene.h/.cpp`)### **VGroup** 🎯

The `Scene` class serves as the main canvas and coordinator:Virtual grouping container for SimpleLine and SimpleDot objects with shared properties.

- **Mobject Registry:** Maintains all objects with unique IDs

- **Selection System:** Tracks currently selected object for property editing- **Purpose**: Efficiently manage multiple similar objects with shared visual properties

- **Animation Manager:** Coordinates all animations and playback- **Members**: Works specifically with SimpleLine and SimpleDot for optimal performance

- **Parser Integration:** Supports command-based object creation and manipulation

- **JSON Serialization:** Save/load complete scenes with all object states- **Shared Properties**

### 4. Object Registration System (`Utils/`)  - `sharedColor` - Applied to all group members

- `sharedThickness` - Applied to all SimpleLine members

**MobjectMap (`mobjectmap.h/.cpp`):**  - `sharedSize` - Applied to all SimpleDot members

- Factory pattern for creating objects by string name- **Individual Properties**: Each member maintains its own position

- Maps object type names to constructor functions- **Automatic Sync**: Adding members automatically applies shared properties

- Initialized once with scene reference for all objects

**Usage Examples:**

**MobjectRegistry (`mobjectregistry.h/.cpp`):**

- QML-accessible list model for UI display```cpp

- Contains object names and icon paths for toolbar/sidebar// Create particle system with SimpleDot

- Used in QML components for object selection menusVGroup* particleGroup = new VGroup(parent);

particleGroup->setSharedColor(Qt::red);

## Available MobjectsparticleGroup->setSharedSize(8.0)

Animat supports several types of graphical objects (Mobjects) that can be animated:for (int i = 0; i < 20; ++i) {

    SimpleDot* particle = new SimpleDot(parent);

### **Circle** ⭕    particle->setPos(QPointF(i *15, sin(i)* 20))

Circular shapes with radius and positioning control.    particleGroup->addMember(particle);  // Auto-applies color & size

- **Properties**: center position, radius, color, opacity}

- **Interactions**: Draggable, selectable, animatable

- **Usage**: Basic geometric shapes, markers, orbital objects// Create line segments with SimpleLine

VGroup* gridGroup = new VGroup(parent);

### **Line** 📏gridGroup->setSharedColor(Qt::blue)

Basic line segments with customizable appearance.gridGroup->setSharedThickness(2.0);

- **Properties**: start/end points, thickness, color

- **Interactions**: Full drag-and-drop support, selectionfor (int i = 0; i < 10; ++i) {

- **Usage**: Connectors, basic geometric constructions    SimpleLine* line = new SimpleLine(parent);

    line->setP1(QPointF(i * 30, 0));

### **Vector** ➡️    line->setP2(QPointF(i * 30, 300))

Line objects with arrow tips for directional representation.    gridGroup->addMember(line);  // Auto-applies color & thickness

- **Properties**: start/end points, thickness, color, arrow tip}

- **Interactions**: Full drag-and-drop support, selection  

- **Usage**: Force vectors, directional indicators, mathematical vectors// Change all members at once

particleGroup->setSharedColor(Qt::green);  // All particles turn green

### **MText** 📝gridGroup->setSharedThickness(5.0);       // All lines become thicker

Full-featured text objects with comprehensive formatting.```

- **Properties**: text content, font size, weight, family, color

- **Interactions**: Click to select, drag to move, animated positioning**Key Benefits:**

- **Usage**: Labels, titles, annotations, mathematical expressions

- **Performance**: Lightweight property management for groups of similar objects

### **Rectangle** ▭- **Consistency**: Ensures all group members maintain visual coherence

Rectangular shapes with size and position control.- **Convenience**: Single property change affects all members instantly

- **Properties**: size, position, color, corner radius- **Flexibility**: Individual positioning with shared visual properties

- **Interactions**: Full transformation support

- **Usage**: Frames, backgrounds, geometric constructionsAll mobjects support:

### **Polygon** ▲- **Selection and highlighting** when clicked

Multi-sided shapes with customizable vertices.- **Drag-and-drop movement** with mouse

- **Properties**: vertex positions, fill color, stroke, border thickness- **Property animation** through the animation system

- **Interactions**: Vertex manipulation, drag-and-drop- **Coordinate transformation** between canvas and scene space

- **Usage**: Custom shapes, geometric constructions- **Memory management** through Qt's parent-child system

### **Dot** ⚫## Object and Properties System

Small circular markers for precise positioning.

- **Properties**: position, size, color### Architecture Overview

- **Interactions**: Draggable, selectable

- **Usage**: Point markers, graph plotting, particle systemsAnimat uses a sophisticated property management system that separates visual objects (Mobjects) from their animatable properties. This allows for clean separation of concerns, easy UI binding, and flexible animation control.

### **Curve** 〰️### Property Management Structure

Mathematical curves with parametric equations.

- **Properties**: curve function, parameter range, thickness```cpp

- **Usage**: Mathematical visualizations, smooth pathsClickableMobject

    └── MProperties (central property manager)

### **Plane** 🏁        ├── BaseProperties (common to all mobjects)

Coordinate system and grid representation.        ├── LineProperties (line-specific properties)

- **Properties**: grid spacing, axis visibility, color        ├── CircleProperties (circle-specific properties)

- **Usage**: Mathematical coordinate systems, reference grids        ├── TextProperties (text-specific properties)

        ├── PolygonProperties (polygon-specific properties)

### **Angle** 📐 🆕        └── CurveProperties (curve-specific properties)

Geometric angle representation with two vectors and arc.```

- **Properties**: angle value, vector lengths, arc radius, colors

- **Interactions**: Vector manipulation, angle adjustment### Base Properties

- **Usage**: Geometric angle visualization, educational content

Every mobject inherits these fundamental properties from `BaseProperties`:

### **Group** 🔗

Standard container for managing multiple objects together.```cpp

- **Purpose**: Group objects for collective transformationsQ_PROPERTY(QString name)          // Mobject identifier

- **Properties**: Shared transformations applied to all membersQ_PROPERTY(QPointF pos)          // Position in scene coordinates

- **Usage**: Complex object hierarchiesQ_PROPERTY(QPointF size)         // Size dimensions

Q_PROPERTY(QColor color)         // Primary color

### **VGroup** 🎯Q_PROPERTY(qreal opacity)        // Transparency (0.0-1.0)

Virtual grouping container for SimpleLine and SimpleDot objects with shared properties.Q_PROPERTY(QString type)         // Mobject type identifier

- **Purpose**: Efficiently manage multiple similar objects with shared visual properties```

- **Members**: Works specifically with SimpleLine, SimpleDot, SimpleVector for optimal performance

- **Shared Properties**: ### Specialized Properties

  - `sharedColor` - Applied to all group members

  - `sharedThickness` - Applied to all SimpleLine/SimpleVector members#### Text Properties (`TextProperties`)

  - `sharedSize` - Applied to all SimpleDot members

- **Individual Properties**: Each member maintains its own positionText-specific properties for font and content control:

- **Automatic Sync**: Adding members automatically applies shared properties

```cpp

**VGroup Usage Examples:**Q_PROPERTY(QString textValue)    // Text content

```cppQ_PROPERTY(int fontSize)         // Font size in pixels

// Create particle system with SimpleDotQ_PROPERTY(int fontWeight)       // Font weight (Normal=50, Bold=75)

VGroup* particleGroup = new VGroup(parent);Q_PROPERTY(QString fontFamily)   // Font family name

particleGroup->setSharedColor(Qt::red);Q_PROPERTY(bool bold)           // Bold state (convenience property)

particleGroup->setSharedSize(8.0);Q_PROPERTY(bool italic)         // Italic state

```

for (int i = 0; i < 20; ++i) {

    SimpleDot* particle = new SimpleDot(parent);_Note: Text color is managed through the base properties `color` field for consistency across all mobject types._

    particle->setPos(QPointF(i * 15, sin(i) * 20));

    particleGroup->addMember(particle);  // Auto-applies color & size#### Line Properties (`LineProperties`)

}

Line-specific properties for geometric control:

// Create vector field with SimpleVector

VGroup* vectorField = new VGroup(parent);```cpp

vectorField->setSharedColor(Qt::blue);Q_PROPERTY(QPointF startPoint)   // Line start position

vectorField->setSharedThickness(2.0);Q_PROPERTY(QPointF endPoint)     // Line end position

Q_PROPERTY(qreal thickness)      // Line thickness

for (int i = 0; i < 10; ++i) {```

    SimpleVector* vector = new SimpleVector(parent);

    vector->setP1(QPointF(i * 30, 0));#### Circle Properties (`CircleProperties`)

    vector->setP2(QPointF(i * 30 + 20, 15));

    vectorField->addMember(vector);  // Auto-applies color & thicknessCircle-specific properties for radius and appearance:

}

Q_PROPERTY(qreal radius)         // Circle radius

### Simple Objects (Performance Optimized)Q_PROPERTY(QPointF center)       // Circle center (mirrors pos)

- **SimpleLine**: Lightweight line with circular caps```

- **SimpleDot**: Lightweight dot for large quantities

- **SimpleVector**: Lightweight vector for group usage#### Polygon Properties (`PolygonProperties`)

- **SimpleText**: Lightweight text rendering

Polygon-specific properties for shape control:

All mobjects support:

- **Selection and highlighting** when clicked```cpp

- **Drag-and-drop movement** with mouseQ_PROPERTY(QColor borderColor)   // Border/stroke color

- **Property animation** through the animation systemQ_PROPERTY(qreal thickness)      // Border thickness

- **Coordinate transformation** between canvas and scene space```

- **Memory management** through Qt's parent-child system

### Property System Workflow

## Property System Architecture

#### 1. **Property Initialization**

### Base Properties (Extended)

When a mobject is created, it initializes its properties:

Every mobject inherits these fundamental properties from `BaseProperties`:

```cpp

```cppText::Text(Scene *canvas, QQuickItem *parent) {

Q_PROPERTY(QString name)          // Mobject identifier    // Initialize base properties

Q_PROPERTY(QPointF pos)          // Position in scene coordinates      properties->base()->setName("Text");

Q_PROPERTY(QPointF size)         // Size dimensions    properties->base()->setColor(m_color);

Q_PROPERTY(QColor color)         // Primary color    properties->base()->setType("Text");

Q_PROPERTY(qreal opacity)        // Transparency (0.0-1.0)

Q_PROPERTY(QString type)         // Mobject type identifier    // Initialize text-specific properties

Q_PROPERTY(qreal scale)          // Scale factor (1.0 = normal size)    properties->setText(new TextProperties(this));

Q_PROPERTY(qreal rotation)       // Rotation angle in degrees    properties->text()->setTextValue(m_text);

Q_PROPERTY(qreal zindex)         // Z-order for layering    properties->text()->setFontSize(m_fontSize);

```}

```

### Specialized Properties

#### 2. **Signal-Slot Connections**

#### Geometric Properties (`GeometricProperties`)

Container for geometric objects with specialized properties:Properties automatically sync with mobject state using Qt's signal-slot system:

- **AngleProperties**: Angle value, start/end vector lengths, arc properties



#### Extended Property System// Property changes update mobject

- **JSON Serialization**: Complete property state persistenceconnect(properties->text(), &TextProperties::textValueChanged, this, [this]() {

- **Signal-Slot Synchronization**: Bidirectional property updates    setText(properties->text()->textValue());

- **QML Integration**: Direct property binding in UI components});

- **Animation Targeting**: Properties can be directly animated

// Mobject changes update properties

## Animation Systemvoid Text::setText(const QString &text) {

    m_text = text;

### Core Animation Architecture    properties->text()->setTextValue(text);  // Keep properties in sync

    emit textChanged();

#### Animation Base Class}

The `Animation` class provides the foundation for all animations:```

- **Abstract Design**: Pure virtual `apply()` method implemented by subclasses

- **Time Management**: `setLtime()` manages timing with validation and interpolation#### 3. **QML Integration**

- **Lifecycle Events**: `onStart()` for initialization, `onEnd()` for cleanup

- **Property System**: JSON serialization support for animation state persistenceProperties are exposed to QML through Q_PROPERTY declarations, enabling direct UI binding:

- **Scene Integration**: Direct access to `Scene` object for coordinate systems



#### Animation Types (`Math/Animations/animTypes/`)// In MPropertiesEditor.qml

StyledTextField {

1. **MoveAnimation** - Smoothly moves objects between positions    text: mprop && mprop.text ? mprop.text.textValue : ""

2. **CreateAnimation** - Fades in objects (opacity 0→1)    onEditingFinished: {

3. **DestroyAnimation** - Fades out objects (opacity 1→0)        if (mprop && mprop.text)

4. **CustomScalarAnimation** - Animates any numeric property            mprop.text.textValue = text;  // Direct property binding

5. **CustomPointAnimation** - Animates any 2D point property    }

6. **ValueAnimation** - Animates abstract values linked to properties}

7. **WaitAnimation** - Creates controllable delays in sequences```



#### AnimPacket System### Property Access Patterns

The `AnimPacket` class groups multiple animations for coordinated execution:

- **Simultaneous Execution**: Multiple animations run in parallel within same time frame#### From C++ Code

- **JSON Serialization**: Complete packet state persistence

- **Update Coordination**: Single `update()` call propagates to all contained animations```cpp

- **Memory Management**: Automatic cleanup of animation resources// Get mobject properties

MProperties* props = mobject->getProperties();

#### AnimationManager

Timeline orchestration system:// Access base properties

- **Doubly-Linked List**: Efficient packet insertion and removal during playbackprops->base()->setPos(QPointF(100, 50));

- **Time-Based Activation**: Packets activate automatically when start time reachedQColor currentColor = props->base()->color();

- **Playback Controls**: Play, pause, reset, and seek functionality  

- **Dynamic Updates**: Real-time packet addition/removal during playback// Access specialized properties

if (props->text()) {

## Value Tracker System    props->text()->setFontSize(24);

    props->text()->setTextColor(Qt::blue);

### ValueTracker Hierarchy}



#### Base ValueTracker Class

Provides interactive control over animation parameters:#### From QML UI

- **QML Integration**: Exposed as QML component for UI binding

- **Signal System**: `valueChanged()` emitted on updates for reactive programming```qml

- **Range Support**: Optional min/max constraints for bounded valuesproperty MProperties mprop: canvas.SelectedMobject ?

- **Real-time Updates**: Immediate propagation to connected animations    canvas.SelectedMobject.getProperties() : null

#### Specialized Tracker Types// Direct property bindings

NumberInput {

##### Standard ValueTracker    value: mprop && mprop.text ? mprop.text.fontSize : 24

- **Numeric Values**: Single `qreal` value with range validation    onValueChanged: {

- **Slider Integration**: Direct binding to QML slider components        if (mprop && mprop.text)

- **Animation Linking**: Values can drive animation parameters in real-time            mprop.text.fontSize = newValue;

    }

##### PtValueTracker (Point Tracker)}

- **2D Coordinates**: `QPointF` values for position tracking```

- **Dual-Axis Control**: Separate X/Y sliders or combined 2D input

- **Geometric Operations**: Direct integration with coordinate-based animations### Property Synchronization

##### PlaybackSliderThe system maintains **bidirectional synchronization**

Special tracker for timeline control:

- **Animation Manager Binding**: Direct connection to animation playback1. **UI → Properties → Mobject**: User changes in the properties panel update the visual object

- **Duration Management**: Automatic duration calculation from animation packets2. **Mobject → Properties → UI**: Programmatic changes in the mobject update the UI

- **Play/Pause Controls**: User interaction with timeline progression3. **Animation → Properties**: Animations can target specific properties for smooth transitions

- **Progress Visualization**: Real-time display of animation progress

### Animation Integration

### TrackerManager

Properties can be directly animated using the animation system:

#### Management System

Centralized control for all value trackers in the scene:```cpp

- **Hash-Based Storage**: Fast O(1) lookup by tracker name// Animate text color

- **Type Safety**: Separate storage for value and point trackersCustomScalar* colorAnim = new CustomScalar(

- **Dynamic Creation**: Runtime tracker creation via parser commands    "textColor",     // Property name

- **JSON Persistence**: Complete tracker state serialization    Qt::white,       // Start color

    Qt::red,         // End color

#### Interactive Controls    2.0             // Duration

);

##### Time-Triggered Sliders

- **Appear Time**: Sliders appear at specific timeline moments// Animate font size

- **Context-Sensitive**: Show only relevant controls during playbackCustomScalar* sizeAnim = new CustomScalar(

- **Dual-Type Support**: Both value and point tracker controls    "fontSize",      // Property name

- **Parser Integration**: Values feed directly into animation system    12,             // Start size

    48,             // End size

## Parser System    1.5             // Duration

);

### Command-Based Scripting Architecture```

The Parser system provides flexible command interface for animation scripting:### Property Editor UI

- **Regex-Based Matching**: Each command defined by specific regex patterns

- **CommandFactory Pattern**: Centralized command creation and validationEach property type has dedicated UI controls in `MPropertiesEditor.qml`:

- **Scene Integration**: Direct access to scene objects and tracker management

- **QML Exposure**: Complete parser functionality available in QML interface- **Text Fields**: For strings (name, textValue, fontFamily)

- **Number Inputs**: For numeric values (fontSize, fontWeight, opacity)

### Command Categories- **Color Pickers**: For colors (color, textColor, borderColor)

- **Point Inputs**: For 2D coordinates (pos, size, startPoint, endPoint)

#### Tracker Creation Commands- **Checkboxes**: For boolean values (bold, italic)

- `val name = value;` - Creates numeric value tracker

- `pval name = (x, y);` - Creates point value tracker  ### Memory Management

- `dval name = expression;` - Creates calculated value tracker

- `dpval name = expression;` - Creates calculated point trackerThe property system follows Qt's parent-child memory management:

#### Connection Commands```cpp

- `connect(tracker, object.property);` - Links tracker to object property// Properties are owned by the mobject

- Enables real-time property updates based on tracker valuesMProperties* properties = new MProperties(this);  // 'this' = parent mobject

#### Command Execution Flow// Specialized properties are owned by MProperties

1. **Input Parsing**: Regex pattern matching against registered commandsproperties->setText(new TextProperties(this));    // 'this' = parent mobject

2. **Parameter Extraction**: Capture groups extract command parameters

3. **Validation**: Type checking and parameter validation// Automatic cleanup when mobject is destroyed

4. **Execution**: Command execution with scene and tracker manager access```

5. **Signal Emission**: Success/failure signals for UI feedback

### Thread Safety

### Script Processing

All property operations occur on the main UI thread, ensuring:

#### Multi-Command Scripts

- **Semicolon Separation**: Commands separated by semicolons- Safe QML bindings

- **Comment Support**: `//` line comments ignored during parsing- Consistent signal emission

- **Batch Execution**: Multiple commands executed in sequence- No race conditions during UI updates

- **Error Handling**: Individual command failures don't stop script execution

### Performance Considerations

#### Mathematical Expression Support

- **muParser Integration**: Advanced mathematical expression evaluation- **Lazy Loading**: Specialized properties are only created when needed

- **Function Support**: Trigonometric, logarithmic, and custom functions- **Change Detection**: Properties only emit signals when values actually change

- **Variable Support**: Time-based variables and tracker references- **Batch Updates**: Multiple property changes can be batched to avoid excessive redraws

- **Real-time Evaluation**: Expressions evaluated continuously during animation- **Efficient Binding**: QML property bindings are optimized by Qt's binding system

## User Interface (`QML/`)## Animation Types

### Main UI Structure### 1. **Move Animation**

- `Main.qml` - Root application window with layout management

- `Components/` - Modular, reusable UI componentsSmoothly moves a mobject from a start position to a target position.

#### Core Components**Parameters:**

- **SceneManager.qml** - Main canvas area where animations are displayed

- **PropertiesToolbar.qml** - Dynamic toolbar that changes based on selected object- `startPos`: Starting position (x,y)

- **Sidebar.qml** - Object palette for adding new Mobjects- `targetPos`: Target position (x,y)

- **Menubar.qml** - Application menu and file operations- `duration`: Animation duration in seconds

- **Taskbar.qml** - Bottom status and control bar

**Example:** Move a circle from (0,0) to (100,100) over 2 seconds.

#### Toolbar Sections (`ToolbarSections/`)

Dynamic property panels that appear based on selected object type:### 2. **Create Animation**

- **AppearanceToolbarSection.qml** - Universal appearance controls (color, opacity, scale, rotation)

- **PolygonToolBar.qml** - Polygon-specific controls (points, thickness, borders)Fades in a mobject by animating its opacity from 0 to 1.

- **TextToolbarSection.qml** - Text formatting controls

- **GeometricToolbarSection.qml** - Geometric object controls (angles, measurements)**Parameters:**

- **SpecialSection.qml** - Advanced features and animations

- `duration`: Fade-in duration in seconds

#### Input Components (`Elements/Input/`)

- **StyledTextField.qml** - Consistent text input with validation**Use case:** Smoothly introduce new objects into the scene.

- **StyledTextArea.qml** - Multi-line text input for scripts

- **NumberInput.qml** - Numeric input with range validation### 3. **Destroy Animation**

- **PointInput.qml** - 2D coordinate input

- **ColorPicker.qml** - Color selection interfaceFades out a mobject by animating its opacity from 1 to 0.

- **MSlider.qml** - Custom slider with precise control

**Parameters:**

#### Tracker Components (`Elements/TrackerComponents/`)

- **ValSlider.qml** - Value tracker slider controls- `duration`: Fade-out duration in seconds

- **TimeTriggeredSlider.qml** - Time-based slider activation

- **SlidersPanel.qml** - Main slider management interface**Use case:** Smoothly remove objects from the scene.

- **ValueTrackersList.qml** - List of active value trackers

- **PointTrackersList.qml** - List of active point trackers### 4. **CustomScalar Animation**

## Key Design PrinciplesAnimates any scalar (numeric) property of a mobject

### 1. **Property-Based Object Identification****Parameters:**

Instead of traditional type checking, objects are identified by which properties they possess. This makes the system highly extensible - new object types can be added by simply defining their unique property combinations.

- `property`: Property name (e.g., "radius", "thickness")

### 2. **Signal-Slot Property Synchronization**- `startVal`: Starting value

All property changes automatically update the visual representation through Qt's signal-slot system. When a property changes, connected slots update the object's appearance immediately.- `targetVal`: Target value

- `duration`: Animation duration

### 3. **Factory Pattern Object Creation**

The `MobjectMap` uses factories to create objects by name, making it easy to add new object types without modifying core scene management code.**Example:** Animate a circle's radius from 10 to 50.

### 4. **Hierarchical Properties Tree**### 5. **CustomPoint Animation**

Properties are organized in a tree structure rather than inheritance, allowing flexible composition of features without complex inheritance hierarchies.

Animates any point (x,y) property of a mobject.

### 5. **Animation-First Design**

Every object property is designed to be animatable. The animation system can smoothly interpolate between any property values over time.**Parameters:**

### 6. **Performance Optimization**- `property`: Property name (e.g., "center", "size")

Simple variants of objects (SimpleLine, SimpleDot, SimpleVector) provide lightweight alternatives for scenarios requiring many objects.- `startVal`: Starting point (x,y)

- `targetVal`: Target point (x,y)

## File Structure- `duration`: Animation duration

```**Example:** Animate a rectangle's size from (50,30) to (100,80).

Animat/

├── Math/### 6. **Value Animation**

│   ├── Scene.h/.cpp                      # Main scene coordination

│   ├── Animations/Animates abstract values that can be connected to multiple mobject properties.

│   │   ├── AllAnimations.h              # Animation includes header

│   │   ├── animation.h/.cpp             # Base Animation class**Parameters:**

│   │   ├── animpacket.h/.cpp            # Animation packet container

│   │   ├── animationmanager.h/.cpp      # Timeline management- `startVal`: Starting value

│   │   └── animTypes/- `endVal`: Ending value

│   │       ├── MoveAnimation.h/.cpp     # Position movement- `duration`: Animation duration

│   │       ├── CreateAnimation.h/.cpp   # Fade in animation

│   │       ├── DestroyAnimation.h/.cpp  # Fade out animation**Use case:** Drive multiple properties with a single animated value.

│   │       ├── CustomScalarAnimation.h/.cpp  # Numeric property animation

│   │       ├── CustomPointAnimation.h/.cpp   # Point property animation### 7. **Wait Animation** 🆕

│   │       ├── ValueAnimation.h/.cpp    # Abstract value animation

│   │       └── WaitAnimation.h/.cpp     # Delay/wait animationCreates a controllable delay/gap between animations.

│   ├── ValueTracker/

│   │   ├── ValueTracker.h/.cpp          # Base value tracking**Parameters:**

│   │   ├── ptvaluetracker.h/.cpp        # Point value tracking

│   │   ├── playbackslider.h/.cpp        # Timeline control- `duration`: Wait duration in seconds

│   │   ├── trackermanager.h/.cpp        # Tracker management

│   │   ├── trackerdata.h/.cpp           # Value tracker data wrapper**Use case:** Add precise timing gaps in animation sequences.

│   │   ├── pttrackerdata.h/.cpp         # Point tracker data wrapper

│   │   └── animationtimer.h/.cpp        # Animation timing control## Animation Script Parser 📝

│   ├── Parser/

│   │   ├── parser.h/.cpp                # Main parser interfaceAnimat includes a powerful script parser that allows you to create complex animations and value tracking using a custom scripting language. The parser supports various command types for creating dynamic, data-driven animations.

│   │   ├── command.h/.cpp               # Base command class

│   │   ├── CommandFactory.h             # Command creation factory### Parser Command Types

│   │   └── Commands/

│   │       ├── makevalcommand.h/.cpp    # Value tracker creation#### 1. **Value Tracker (`val`)**

│   │       ├── makeptvalcommand.h/.cpp  # Point tracker creation

│   │       ├── dvalcommand.h/.cpp       # Dynamic value trackerCreates a scalar value tracker with either static values or mathematical expressions.

│   │       ├── dpvalcommand.h/.cpp      # Dynamic point tracker

│   │       └── connectcommand.h/.cpp    # Property connection**Syntax:**

│   ├── Mobjects/

│   │   ├── Base/```cpp

│   │   │   ├── Mobject.h/.cpp           # Core mobject foundationval name = value;

│   │   │   └── ClickableMobject.h/.cpp  # Interactive base classval name = expression;

│   │   ├── Basic/```

│   │   │   ├── Circle.h/.cpp            # Circular shapes

│   │   │   ├── line.h/.cpp              # Basic line segments**Examples:**

│   │   │   ├── Vector.h/.cpp            # Lines with arrow tips

│   │   │   ├── Dot.h/.cpp               # Small circular markers```cpp

│   │   │   ├── MRectangle.h/.cpp        # Rectangular shapesval radius = 25;

│   │   │   └── Polygon.h/.cpp           # Multi-sided shapesval angle = sin(t * 2);

│   │   ├── Simple/val speed = cos(t) * 10 + 15;

│   │   │   ├── SimpleLine.h/.cpp        # Optimized lineval growth = t * 0.5;

│   │   │   ├── SimpleDot.h/.cpp         # Optimized dot```

│   │   │   ├── SimpleVector.h/.cpp      # Optimized vector

│   │   │   └── SimpleText.h/.cpp        # Optimized text**Use Cases:**

│   │   ├── Complex/

│   │   │   ├── Curve.h/.cpp             # Mathematical curves- Static configuration values

│   │   │   ├── MPolygon.h/.cpp          # Dynamic polygon- Time-dependent calculations

│   │   │   └── plane.h/.cpp             # Coordinate system- Mathematical function evaluation

│   │   ├── Groups/- Animation parameter control

│   │   │   ├── group.h/.cpp             # Standard grouping

│   │   │   └── VGroup.h/.cpp            # Virtual grouping#### 2. **Point Value Tracker (`pval`)**

│   │   ├── MText/

│   │   │   └── MText.h/.cpp             # Full-featured textCreates a 2D point tracker with x,y coordinates, supporting both static points and parametric expressions.

│   │   └── Geometric/

│   │       └── Angle.h/.cpp             # Geometric angle**Syntax:**

│   ├── Helper/

│   │   ├── mproperties.h/.cpp           # Property management```cpp

│   │   ├── funcs.h/.cpp                 # Utility functionspval name = (x, y);

│   │   └── Properties/pval name = (x_expression, y_expression);

│   │       ├── baseproperties.h/.cpp    # Common properties```

│   │       ├── lineproperties.h/.cpp    # Line-specific

│   │       ├── circleproperties.h/.cpp  # Circle-specific**Examples:**

│   │       ├── textproperties.h/.cpp    # Text-specific

│   │       ├── polygonproperties.h/.cpp # Polygon-specific```cpp

│   │       ├── curveproperties.h/.cpp   # Curve-specificpval center = (100, 50);

│   │       ├── geometricproperties.h/.cpp # Geometric containerpval orbit = (cos(t * 2) * 50, sin(t * 2) * 50);

│   │       └── Geometric/pval wave = (t * 10, sin(t * 0.5) * 30);

│   │           ├── angleproperties.h/.cpp   # Angle propertiespval spiral = (t * cos(t), t * sin(t));

│   │           └── planeproperties.h/.cpp   # Plane properties```

│   └── Managers/

│       └── funcmap.h/.cpp               # Function mapping utilities**Use Cases:**

├── QML/

│   ├── Main.qml                         # Root application window- Object positioning

│   └── Components/- Parametric motion paths

│       ├── SceneManager.qml             # Main canvas- Circular/orbital movement

│       ├── PropertiesToolbar.qml        # Dynamic property toolbar- Wave and spiral patterns

│       ├── Sidebar.qml                  # Object palette

│       ├── Menubar.qml                  # Application menu#### 3. **Dynamic Value Tracker (`dval`)**

│       ├── Taskbar.qml                  # Bottom control bar

│       ├── TitleBar.qml                 # Window title barCreates values that depend on other tracked values using bracket notation for references.

│       ├── ToolbarSections/

│       │   ├── AppearanceToolbarSection.qml  # Appearance controls**Syntax:**

│       │   ├── PolygonToolBar.qml            # Polygon controls

│       │   ├── TextToolbarSection.qml        # Text formatting```cpp

│       │   ├── GeometricToolbarSection.qml   # Geometric controlsdval name = expression_with_[references];

│       │   └── SpecialSection.qml            # Advanced features```

│       └── Elements/

│           ├── MpropertiesEditor.qml    # Modular property editor**Examples:**

│           ├── ValueManagement.qml      # Tracker management

│           ├── ActiveAnimationsList.qml # Animation list```cpp

│           ├── Input/dval combined = [radius] * 2 + [speed];

│           │   ├── StyledTextField.qml  # Text inputdval scaled = [orbit_x] * [scale_factor];

│           │   ├── StyledTextArea.qml   # Multi-line inputdval distance = sqrt([x1] * [x1] + [y1] * [y1]);

│           │   ├── NumberInput.qml      # Numeric inputdval interpolated = [start] + ([end] - [start]) * [progress];

│           │   ├── PointInput.qml       # 2D coordinate input```

│           │   ├── ColorPicker.qml      # Color selection

│           │   ├── MSlider.qml          # Custom slider**Use Cases:**

│           │   ├── AnimInput.qml        # Animation input

│           │   └── PlaybackInput.qml    # Playback controls- Value composition and combination

│           └── TrackerComponents/- Calculated properties based on other values

│               ├── ValSlider.qml        # Value slider- Complex mathematical relationships

│               ├── TimeTriggeredSlider.qml   # Time-based slider- Multi-stage value processing

│               ├── SlidersPanel.qml     # Slider management

│               ├── ValueTrackersList.qml     # Value tracker list#### 4. **Dynamic Point Tracker (`dpval`)**

│               └── PointTrackersList.qml     # Point tracker list

├── Utils/Creates 2D points that depend on other tracked values.

│   ├── mobjectmap.h/.cpp                # Object factory mapping

│   ├── mobjectregistry.h/.cpp           # Object registration**Syntax:**

│   ├── filehandler.h/.cpp               # File I/O operations

│   ├── Constants.h                      # Application constants```cpp

│   └── Singleton.h                      # Singleton pattern utilitydpval name = (x_expression_with_[refs], y_expression_with_[refs]);

├── lib/                                 # muParser mathematical library```

├── CMakeLists.txt                       # Build configuration

└── README.md                            # This documentation**Examples:**

```

```cpp

## Building and Runningdpval adjusted_center = ([base_x] + [offset_x], [base_y] + [offset_y]);

dpval scaled_position = ([pos_x] * [scale], [pos_y] * [scale]);

### Requirementsdpval relative_point = ([anchor_x] + [radius] * cos([angle]), [anchor_y] + [radius] * sin([angle]));

- Qt 6.9.1 or later```

- CMake 3.16 or later

- MinGW 64-bit compiler (Windows)**Use Cases:**



### Build Instructions- Dependent positioning

```bash- Coordinate transformations

mkdir build- Relative positioning systems

cd build- Complex motion combinations

cmake ..

cmake --build .#### 5. **Connection Commands (`connect`)**

```

Links tracked values to mobject properties for automatic updates.

### Running

```bash
**Syntax:**

./build/Desktop_Qt_6_9_1_MinGW_64_bit-Debug/appAnimat.exe

```

connect(tracker_name, object.property);

## Development Philosophy```



Animat aims to make mathematical animation accessible to non-programmers while maintaining the power and flexibility that developers need. The visual interface removes the coding barrier while the underlying architecture supports complex animations and custom extensions.**Examples:**



The framework prioritizes:```cpp

- **Intuitive UI/UX** - Drag-and-drop object creation, visual property editingconnect(radius, circle.radius);

- **Real-time feedback** - Immediate visual updates as properties changeconnect(center, rectangle.pos);

- **Extensibility** - Clean architecture for adding new objects and animationsconnect(rotation_angle, text.rotation);

- **Performance** - Qt6/QML rendering for smooth 60fps animationsconnect(wave_position, line.startPoint);

- **Flexibility** - Both GUI and code-based workflows supported```



## Contributing**Supported Properties:**



When adding new features:- `pos` - Position (requires point tracker)

- `size` - Size dimensions (requires point tracker)

### Adding New Mobject Types- `color` - Color (requires color tracker)

1. Create new class in appropriate `Math/Mobjects/` subdirectory- `opacity` - Transparency (0.0-1.0)

2. Define specialized properties if needed in `Math/Helper/Properties/`- `scale` - Scale factor

3. Register in `MobjectMap::init()` and `MobjectRegistry`- `rotation` - Rotation angle

4. Add UI support in toolbar sections- `radius` - Circle radius

5. Update property editor for new property types- `thickness` - Line thickness

- Type-specific properties

### Adding New Animation Types

1. Define class in `Math/Animations/animTypes/`### Expression Syntax

2. Implement in corresponding `.cpp` file

3. Add case in `AnimPacket::addAnimation()`The parser supports mathematical expressions with standard operators and functions:

4. Update UI components for new animation parameters

#### **Operators:**

### Adding New Properties

1. Create property class in `Math/Helper/Properties/`- Arithmetic: `+`, `-`, `*`, `/`

2. Add to `MProperties` container

3. Update JSON serialization methods#### **Functions:**

4. Add UI controls in property editor

- Trigonometric: `sin()`, `cos()`, `tan()`, `asin()`, `acos()`, `atan()`

---- Mathematical: `sqrt()`, `log()`, `exp()`, `abs()`, `floor()`, `ceil()`

- Constants: `pi`, `e`

_Built with Qt 6.9.1 and QML for cross-platform mathematical animation creation._
#### **Variables:**

- `[tracker_name]` - Reference to other tracker values
- Numbers: `123`, `3.14`, `-0.5`

### Script Examples

#### **Basic Animation Setup:**

```cpp
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

```cpp
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

```cpp
User Input → AnimationTypeSelector → AnimationFieldsPanel → Validation
```

### 2. **Animation Processing**

```cpp
Parameters → AnimPacket.addAnimation() → Animation Object Creation
```

### 3. **Timeline Management**

```cpp
AnimationManager → Doubly Linked List → Sorted by Start Time
```

### 4. **Playback Control**

```cpp
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

```cpp
Timeline: 0s────2s────4s────6s────8s
         Move   Wait  Scale  Wait  Destroy
```

## File Structure

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

## Technical Details

### Animation Base Class

All animations inherit from the `Animation` base class, providing:

- **Normalized time** (0.0 to 1.0) calculation
- **Easing curve** application
- **Start offset** and **duration** management
- **Abstract apply()** method for subclass implementation

### Memory Management (Technical Details)

- **Qt parent-child** system for automatic cleanup
- **Smart pointer** usage where appropriate
- **Proper disconnection** of signals when objects are destroyed

### Performance Considerations (Points)

- **Efficient sorting** using doubly linked list
- **Minimal allocations** during playback
- **Optimized rendering** through Qt's scene graph

## Building and Running

### Requirements

- Qt 6.9.1 or later
- CMake 3.16 or later
- MinGW 64-bit compiler (Windows)

### Build Instructions

    mkdir build
    cd build
    cmake ..
    cmake --build .

### Running (Command)

    ./build/Desktop_Qt_6_9_1_MinGW_64_bit-Debug/appAnimat.exe

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
