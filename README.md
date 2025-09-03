# Animat Animation Tool

MathVisual Designer is a desktop application built with **C++** and **Qt** that enables users to visually compose and animate mathematical objects ("mobjects") with a drag-and-drop interface—no coding required. Think of it as PowerPoint, but for math animations!

## Features

- **Drag-and-Drop Math Components**: Easily add equations, graphs, shapes, and more to your canvas.
- **Rich Animation Controls**: Animate each component independently with a timeline editor.
- **Intuitive UI**: All features accessible via graphical controls—no programming needed!
- **Export Options**: Save your work as JSON, or shareable project files.
- **Math Input Support**: Enter equations a visual editor, rendered as math objects.
- **Project Management**: Save, open, and organize your animation projects.
- **Snap & Align Tools**: Precisely position mobjects with grid and alignment helpers.
- **Layer & Property Panels**: Manage object stacking, styles, and attributes.


## Getting Started

### Prerequisites

- [Qt 5 or Qt 6](https://www.qt.io/download) (Qt Creator recommended)
- C++17 or higher
- CMake (or qmake if preferred)

### Building

```bash
git clone https://github.com/yourusername/mathvisual-designer.git
cd mathvisual-designer
mkdir build && cd build
cmake ..
make
./MathVisualDesigner
```

Or open the project in **Qt Creator** and build/run.

### Running

On successful build, launch the executable. You’ll see the main canvas and a palette of math components to start creating your animations.

## Project Structure

- `/` — Application source code (C++ & Qt)
- `/assets` — Icons, images, and UI assets

## Authors

- [Prashant Suthar ](https://github.com/prashant-180206)

---

**Animat** — Making math visual, interactive, and fun!
