# ABS Graphics Library

A simple and minimal graphics library for creating 2D applications in C. Built as an improved abstraction layer over OpenGL/GLFW, inspired by raylib's clean API design.This library is Designed specifically for integration with the Luna programming language.

## About

ABS provides a clean API for window management and 2D drawing primitives. The library uses OpenGL for rendering and GLFW for window management, wrapped in a simple interface that minimizes boilerplate code.

## Dependencies

**OpenGL**: Cross-platform graphics API for rendering 2D/3D graphics.

**GLFW**: Library for creating windows and handling input events.

### Installation

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install libglfw3-dev
```

**Fedora:**
```bash
sudo dnf install glfw-devel
```

**Arch Linux:**
```bash
sudo pacman -S glfw-wayland
```

**macOS:**
```bash
brew install glfw
```

## Building

```bash
make           # Compile the project
make clean     # Remove compiled files
make run       # Compile and run
```

## Project Structure

| File | Description |
|------|-------------|
| `colors.h` | Color definitions and RGB/RGBA helper functions |
| `abs.h` | Main library header with function declarations |
| `abs.c` | Implementation of drawing and window functions |
| `main.c` | Example application demonstrating library features |

## Available Functions

### Window Management

| Function | Description |
|----------|-------------|
| `create_window(width, height, title)` | Initialize window with specified dimensions and title |
| `window_is_open()` | Returns 1 if window is open, 0 if closed |
| `update_window()` | Handle events and refresh display |
| `close_window()` | Clean up and terminate window |

### Drawing Functions

| Function | Parameters | Description |
|----------|------------|-------------|
| `DrawRectangle()` | `x, y, width, height, thickness, color` | Draw rectangle (negative thickness = filled) |
| `DrawCircle()` | `x, y, radius, thickness, color` | Draw circle (negative thickness = filled) |
| `DrawLine()` | `x1, y1, x2, y2, thickness, color` | Draw line with specified thickness |
| `DrawText()` | `text, x, y, color` | Draw text (placeholder, not yet implemented) |

### Color System

**Predefined Colors:**
`RED`, `GREEN`, `BLUE`, `YELLOW`, `ORANGE`, `PURPLE`, `CYAN`, `MAGENTA`, `WHITE`, `BLACK`, `GRAY`, `PINK`, `BROWN`, `GOLD`, `NAVY`, `TEAL`, and more.

**Custom Colors:**
```c
rgb(r, g, b)        // Create RGB color (0-255)
rgba(r, g, b, a)    // Create RGBA color with transparency
```

## Examples

### Quick Start: Hello Window

Minimal code to get started with the library:

```c
#include "abs.h"

int main() {
    create_window(800, 600, "Hello ABS");
    
    while (window_is_open()) {
        update_window();
    }
    
    close_window();
    return 0;
}
```

This creates a blank window that stays open until closed.

### Example 1: Filled vs Outlined Shapes

```c
#include "abs.h"

int main() {
    create_window(800, 600, "Filled vs Outlined Demo");
    
    while (window_is_open()) {
        
        // LEFT SIDE: FILLED SHAPES (thickness = -1)
        
        // Filled red rectangle
        DrawRectangle(50, 50, 150, 100, -1, RED);
        
        // Filled green circle
        DrawCircle(125, 250, 60, -1, GREEN);
        
        // Filled yellow square
        DrawRectangle(50, 350, 100, 100, -1, YELLOW);
        
        
        // RIGHT SIDE: OUTLINED SHAPES (thickness = positive)
        
        // Outlined blue rectangle (2px thick)
        DrawRectangle(450, 50, 150, 100, 2, BLUE);
        
        // Outlined orange circle (3px thick)
        DrawCircle(525, 250, 60, 3, ORANGE);
        
        // Outlined purple square (4px thick)
        DrawRectangle(475, 350, 100, 100, 4, PURPLE);
        
        
        // CENTER: DIVIDER LINE
        DrawLine(400, 0, 400, 600, 1, WHITE);
        
        // BOTTOM: VARIOUS LINE THICKNESSES
        DrawLine(50, 520, 350, 520, 1, CYAN);    // 1px thin
        DrawLine(50, 540, 350, 540, 3, CYAN);    // 3px medium
        DrawLine(50, 560, 350, 560, 8, CYAN);    // 8px thick
        
        // Custom RGB colors
        DrawCircle(700, 100, 40, -1, rgb(255, 100, 200));      // Filled pink
        DrawRectangle(650, 200, 80, 60, 5, rgb(50, 150, 255)); // Outlined light blue
        
        // Semi-transparent
        DrawRectangle(250, 150, 100, 100, -1, rgba(255, 0, 255, 128)); // Transparent magenta
        
        update_window();
    }
    
    close_window();
    return 0;
}
```

### Example 2: Simple Animation

```c
#include "abs.h"

int main() {
    create_window(800, 600, "Animation Demo");
    
    float x = 0;
    
    while (window_is_open()) {
        
        // Moving circle
        DrawCircle(x, 300, 50, -1, RED);
        
        // Trail effect
        DrawCircle(x - 60, 300, 40, -1, rgba(255, 0, 0, 100));
        DrawCircle(x - 120, 300, 30, -1, rgba(255, 0, 0, 50));
        
        // Increment position
        x += 2;
        if (x > 850) x = -50;
        
        update_window();
    }
    
    close_window();
    return 0;
}
```

### Example 3: Grid Pattern

```c
#include "abs.h"

int main() {
    create_window(800, 600, "Grid Pattern");
    
    while (window_is_open()) {
        
        // Draw grid lines
        for (int i = 0; i < 800; i += 50) {
            DrawLine(i, 0, i, 600, 1, GRAY);
        }
        
        for (int i = 0; i < 600; i += 50) {
            DrawLine(0, i, 800, i, 1, GRAY);
        }
        
        // Draw colorful circles at intersections
        for (int x = 50; x < 800; x += 100) {
            for (int y = 50; y < 600; y += 100) {
                DrawCircle(x, y, 20, -1, rgb(x % 255, y % 255, (x + y) % 255));
            }
        }
        
        update_window();
    }
    
    close_window();
    return 0;
}
```

## Key Concepts

**Coordinate System:** Origin (0, 0) is at the top-left corner. X increases right, Y increases downward.

**Thickness Convention:**
- Negative value (e.g., -1): Filled shape
- Positive value (e.g., 1, 2, 3): Outlined shape with specified pixel width

**Color Values:** RGB values range from 0 to 255. Alpha (transparency) ranges from 0 (fully transparent) to 255 (fully opaque).

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3). See the LICENSE file for details.
