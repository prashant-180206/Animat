# Animat - Animation Framework

A powerful, code-less animation framework inspired by Manim, designed for creating mathematical and geometric animations with an intuitive user interface.

## Architecture Overview

### 1. Mobject Hierarchy (`Math/Mobjects/`)

The foundation of the animation system is built around **Mobjects** (Mathematical Objects) with a clear inheritance hierarchy:

```pgsql
Mobject (Base QQuickItem)

└── ClickableMobject (Adds interactivity, properties, scene integration)
    ├── Basic/
    │   ├── Circle
    │   ├── Line  
    │   ├── Dot
    │   ├── MRectangle
    │   └── Polygon
    ├── Complex/
    │   ├── Curve

    │   ├── MPolygon  
    │   └── Plane (Coordinate system)

    ├── MText/
    │   └── MText (Text rendering)

    ├── Groups/

        └── SimpleText
```

**Key Features:**

- Each object has a unique ID and maintains its state through JSON serialization

#### 2. Properties System (`Math/Helper/Properties/`)

**Main Properties Container:**

- `MProperties` - Central property container with specialized sub-properties

**Property Types:**

- `PolygonProperties` - Thickness, border color, fill properties
- `CurveProperties` - Curve-specific parameters

- `TextProperties` - Font, size, alignment for text objects

Objects are identified by their **property existence** - each Mobject constructor initializes only the properties it needs, making object type identification possible by checking which properties exist.

Example:

```cpp

// Circle constructor
    properties->setPolygon(new PolygonProperties(this->properties)); // Inherited from Polygon
    // Base properties are always initialized in ClickableMobject

```

#### 3. Scene Management (`Math/Scene.h/.cpp`)

The `Scene` class serves as the main canvas and coordinator:

- **Mobject Registry:** Maintains all objects with unique IDs
- **Selection System:** Tracks currently selected object for property editing
- **Animation Manager:** Coordinates all animations and playback
- **Parser Integration:** Supports command-based object creation and manipulation
- **JSON Serialization:** Save/load complete scenes with all object states

#### 4. Object Registration System (`Utils/`)

**MobjectMap (`mobjectmap.h/.cpp`):**

- Factory pattern for creating objects by string name
- Maps object type names to constructor functions
- Initialized once with scene reference for all objects

**MobjectRegistry (`mobjectregistry.h/.cpp`):**

- QML-accessible list model for UI display
- Contains object names and icon paths for toolbar/sidebar
- Used in QML components for object selection menus

#### 5. Animation System (`Math/Animations/`)

**Base Animation Framework:**

The animation system provides a comprehensive framework for creating mathematical object animations:

**Core Animation Architecture:**

- **Animation Base Class:** Abstract foundation with pure virtual `apply()` method
- **Time Management:** `setLtime()` handles timing with validation and interpolation
- **Lifecycle Events:** `onStart()` for initialization, `onEnd()` for cleanup
- **Property System:** JSON serialization for animation state persistence
- **Scene Integration:** Direct access to Scene object for coordinate systems

**AnimPacket System:**

- **Grouped Execution:** Multiple animations run simultaneously within same time frame
- **JSON Serialization:** Complete packet state persistence for project files
- **Update Coordination:** Single `update()` call propagates to all contained animations
- **Memory Management:** Automatic cleanup of animation resources

**AnimationManager:**

- **Timeline Orchestration:** Doubly-linked list for efficient packet management
- **Time-Based Activation:** Automatic packet activation when start time reached
- **Playback Controls:** Play, pause, reset, and seek functionality
- **Dynamic Updates:** Real-time packet addition/removal during playback

**Animation Types (`animTypes/`):**

- `CreateAnimation` - Fade in objects (opacity 0→1)
- `DestroyAnimation` - Fade out objects (opacity 1→0)
- `MoveAnimation` - Smooth position transitions
- Custom animations can be easily added following the same pattern

#### 6. User Interface (`QML/`)

**Main UI Structure:**

- `Main.qml` - Root application window
- `Components/` - Reusable UI components
  - `SceneManager.qml` - Canvas area where animations are displayed
  - `PropertiesToolbar.qml` - Dynamic toolbar that changes based on selected object
  - `Sidebar.qml` - Object palette for adding new Mobjects
  - `Menubar.qml` - Main application menu
  - `Taskbar.qml` - Bottom status/control bar

**Toolbar Sections (`ToolbarSections/`):**
Dynamic property panels that appear based on selected object type:

- `AppearanceToolbarSection.qml` - Universal appearance controls (color, opacity, scale, rotation)
- `PolygonToolBar.qml` - Polygon-specific controls (points, thickness, borders)
- `TextToolbarSection.qml` - Text formatting controls
- `SpecialSection.qml` - Advanced features and animations

The toolbar system automatically detects object type through property existence and shows relevant controls.

#### 7. Utility Systems

**File Management (`Utils/filehandler.h/.cpp`):**

- Scene serialization/deserialization
- Project file management
- Import/export functionality

**Value Tracking (`ValueTracker/`):**

**ValueTracker Hierarchy:**

- **Base ValueTracker:** Interactive control over animation parameters with QML integration
- **Signal System:** `valueChanged()` emitted on updates for reactive programming
- **Range Support:** Optional min/max constraints for bounded values
- **Real-time Updates:** Immediate propagation to connected animations

**Specialized Tracker Types:**

- **Standard ValueTracker:** Single `qreal` value with range validation and slider integration
- **PtValueTracker:** `QPointF` values for 2D coordinate tracking with dual-axis control
- **PlaybackSlider:** Special timeline control tracker with animation manager binding

**TrackerManager:**

- **Centralized Control:** Hash-based storage for fast O(1) lookup by tracker name
- **Type Safety:** Separate storage for value and point trackers
- **Dynamic Creation:** Runtime tracker creation via parser commands
- **JSON Persistence:** Complete tracker state serialization

**Interactive Controls:**

- **Time-Triggered Sliders:** Appear at specific timeline moments with context sensitivity
- **Dual-Type Support:** Both value and point tracker controls
- **Parser Integration:** Values feed directly into animation system
- **Progress Visualization:** Real-time display of animation progress

**Parser System (`Math/Parser/`):**

**Command-Based Scripting Architecture:**

- **Regex-Based Matching:** Each command defined by specific regex patterns
- **CommandFactory Pattern:** Centralized command creation and validation  
- **Scene Integration:** Direct access to scene objects and tracker management
- **QML Exposure:** Complete parser functionality available in QML interface

**Command Categories:**

- **Tracker Creation:** `val name = value;` for numeric trackers, `pval name = (x,y);` for point trackers
- **Calculated Values:** `dval name = expression;` and `dpval name = expression;` for computed trackers
- **Property Linking:** `connect(tracker, object.property);` for real-time property updates

**Script Processing:**

- **Multi-Command Support:** Semicolon-separated commands with comment support (`//`)
- **Batch Execution:** Multiple commands executed in sequence with individual error handling
- **Real-time Integration:** Live script editing with immediate execution and debug feedback
- **Mathematical Expressions:** Integrated muParser library for complex calculations

## Key Design Principles

### 1. **Property-Based Object Identification**

Instead of traditional type checking, objects are identified by which properties they possess. This makes the system highly extensible - new object types can be added by simply defining their unique property combinations.

### 2. **Signal-Slot Property Synchronization**  

All property changes automatically update the visual representation through Qt's signal-slot system. When a property changes, connected slots update the object's appearance immediately.

### 3. **Factory Pattern Object Creation**

The `MobjectMap` uses factories to create objects by name, making it easy to add new object types without modifying core scene management code.

### 4. **Hierarchical Properties Tree**

Properties are organized in a tree structure rather than inheritance, allowing flexible composition of features without complex inheritance hierarchies.

### 5. **Animation-First Design**

Every object property is designed to be animatable. The animation system can smoothly interpolate between any property values over time.

## Adding New Geometric Objects

To extend the framework with new geometric objects in the "Geometric" category:

### 1. Create New Mobject Classes

```cpp
// Math/Mobjects/Geometric/NewShape.h
class NewShape : public ClickableMobject {
    // Constructor initializes required properties
    // Implement shape-specific rendering logic
};
```

### 2. Define Geometric Properties

```cpp
// Math/Helper/Properties/Geometric/newshapeproperties.h
class NewShapeProperties : public QObject {
    // Shape-specific properties (dimensions, angles, etc.)
};
```

### 3. Update GeometricProperties Container

Add the new property type to `GeometricProperties` class and `MProperties`.

### 4. Register in Object Map

Add factory function to `MobjectMap::init()` and entry in `MobjectRegistry`.

### 5. Create Toolbar Section

```qml
// QML/Components/ToolbarSections/GeometricToolbarSection.qml
// Property controls that appear when geometric objects are selected
```

### 6. Update PropertiesToolbar

Add the new toolbar section to the main properties toolbar with appropriate visibility conditions.

This modular architecture makes adding new object types straightforward while maintaining clean separation of concerns and consistent behavior across all objects.

## Build System

The project uses CMake with Qt6:

- **Qt6 Quick** for QML-based UI
- **muParser** library for mathematical expression parsing
- **CMake tasks** configured for building and running with proper DLL deployment

## Development Philosophy

Animat aims to make mathematical animation accessible to non-programmers while maintaining the power and flexibility that developers need. The visual interface removes the coding barrier while the underlying architecture supports complex animations and custom extensions.

The framework prioritizes:

- **Intuitive UI/UX** - Drag-and-drop object creation, visual property editing
- **Real-time feedback** - Immediate visual updates as properties change  
- **Extensibility** - Clean architecture for adding new objects and animations
- **Performance** - Qt6/QML rendering for smooth 60fps animations
- **Flexibility** - Both GUI and code-based workflows supported
