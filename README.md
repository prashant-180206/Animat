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
│   │   ├── animation.h          # Base Animation class & all animation types
│   │   ├── animation.cpp        # Animation implementations
│   │   ├── animpacket.h         # Animation packet container
│   │   ├── animpacket.cpp       # Packet management
│   │   ├── animationmanager.h   # Timeline management
│   │   └── animationmanager.cpp # Manager implementation
│   ├── ValueTracker/
│   │   ├── playbackslider.h     # Dynamic playback control
│   │   └── playbackslider.cpp   # Playback implementation
│   └── Scene.cpp                # Main scene coordination
├── QML/Components/Elements/Input/
│   ├── AnimationTypeSelector.qml    # Animation type dropdown
│   ├── AnimationFieldsPanel.qml     # Parameter input fields
│   ├── AnimInput.qml                # Main animation input UI
│   └── PlaybackInput.qml            # Playback controls UI
└── README.md                        # This documentation
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
