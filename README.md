# BLZ Graphics & Audio Library

BLZ is a lightweight, high-performance C library for 2D graphics and audio. It provides a simple, modern API for window management, rendering, and sound.

## About

Releasing this now as i jut ... forgot about it lol

This library will hopefully be used for the Luna language in the future.....

## Features

- **2D Graphics**: High-performance **OpenGL 3.3** rendering (Rectangles, circles, lines, textures, fonts).
- **Audio**: Music streaming and sound effects via **miniaudio**.
- **Input**: Easy-to-use keyboard and mouse handling via **GLFW**.
- **Single Header**: Include `blz.h` and you're ready to go.

## Credits & Sources

BLZ is built upon these incredible open-source projects:

- **[GLFW](https://www.glfw.org/)**: Window management and input.
- **[miniaudio](https://miniaud.io/)**: Single-header audio playback and management.
- **[stb_image](https://github.com/nothings/stb)**: Image loading, Screenshot functionality and Font rendering.

### Acknowledgments

BLZ is heavily inspired by **[raylib](https://www.raylib.com/)**. Many of the API patterns and core implementation ideas (like the batch renderer) are based on the incredible work done by the raylib community. Thank you for making gamedev in C so accessible!


## Directory Structure

- `blz.h`: The main library header.
- `src/`: Core implementation and backend headers.
- `lib/`: Precompiled static libraries.
- `main.c`: Demo application.

## How to Build and Run

### Prerequisites (Linux)
```bash
# Install Clang and dependencies
sudo apt install gcc build-essential libx11-dev libasound2-dev mesa-common-dev libgl1-mesa-dev 
```

### Build & Run
The project uses `clang` by default (see `Makefile`).
```bash
make       # Uses all CPU cores for fast compilation
make run   # Executes the demo 
make run-car # Executes the car demo 
```

## General Example

```c
#include "blz.h"

int main() {
    // 1. Initialize window
    init_window(1280, 720, "BLZ App");
    
    // 2. Initialize audio system
    init_audio();

    // 3. Application Loop
    while (!window_should_close()) {
        begin_drawing();
        clear_background(COLOR_BLACK);
        
        // Draw a red rectangle
        draw_rect((Rect){100, 100, 200, 150}, COLOR_RED);
        
        // Draw text
        draw_text("Hello BLZ!", 400, 300, 40, COLOR_WHITE);
        
        // Handle input
        if (is_key_pressed(KEY_ESCAPE)) break;

        end_drawing();
    }

    // 4. Cleanup
    close_audio();
    close_window();
    return 0;
}
```

### Music Credits 

The following tracks are included in the demo playlist. All rights belong to the respective artists.

| Track Name     | Artist(s)  |   Source                 |
| :----------------- | :------------------------ | :---------------------- |
| **Jackpot**                           | TheFatRat                 | [YouTube](https://www.youtube.com/watch?v=kL8CyVqzmkc) |
| **Rise Up**                           | TheFatRat                 | [YouTube](https://www.youtube.com/watch?v=j-2DGYNXRx0) |
| **Close To The Sun**                  | TheFatRat & Anjulie       | [YouTube](https://www.youtube.com/watch?v=oJuGlqO85YI) |
| **Escaping Gravity**                  | TheFatRat & Cecilia Gault | [YouTube](https://www.youtube.com/watch?v=xfGrN3ZsPLA) |
| **Hot Together**                      | The Pointer Sisters       | [YouTube](https://www.youtube.com/watch?v=H3Aay-47ZT0) |
| **Too Sweet**                         | Hozier                    | [YouTube](https://www.youtube.com/watch?v=NTpbbQUBbuo) |
| **Forever Young**                     | Alphaville                | [YouTube](https://www.youtube.com/watch?v=YHRvDo8rUoQ) |
| **We Will Rock You**                  | Max Raabe                 | [YouTube](https://www.youtube.com/watch?v=a3O-PLopk5g) |
| **Sunflower**                         | Post Malone & Swae Lee    | [YouTube](https://www.youtube.com/watch?v=ApXoWvfEYVU) |
| **Everybody Wants To Rule The World** | Tears For Fears           | [YouTube](https://www.youtube.com/watch?v=aGCdLKXNF3w) |
| **Unstoppable**                       | The Score                 | [YouTube](https://www.youtube.com/watch?v=_PBlykN4KIY) |
| **Tip Toe**                           | HYBS                      | [YouTube](https://www.youtube.com/watch?v=dHUq9xJcaZs) |
| **Blue**                              | yung kai                  | [YouTube](https://www.youtube.com/watch?v=GGJOC1FNqn8) |
| **Nightcall**                         | Kavinsky                  | [YouTube](https://www.youtube.com/watch?v=MV_3Dpw-BRY) |

> [!NOTE]
> I do not own any of the songs or assets used in this demonstration. They are included for demonstration purposes only.

## License

The core BLZ source code and demo applications are licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.

Third-party libraries used in this project are licensed under their own respective terms (Public Domain, MIT, or zlib/libpng).
