# Arthur

A modern, hardware-accelerated 2.5D raycaster game engine built from scratch in C++ using **Raylib**. This project takes the core mathematical principles of early 90s shooters like *Wolfenstein 3D* and upgrades them from a text-based console into a true standalone desktop application.

---

## Features
* **True Pixel Rendering:** Uses GPU-accelerated drawing instead of a locked Windows console grid.
* **Fisheye Correction:** Implements cosine angle correction to eliminate the "curved wall" lens distortion common in basic raycasters.
* **Dynamic Lighting:** Real-time distance-based depth shading (walls get darker the further away they are).
* **Smooth Delta-Time Movement:** Framerate-independent movement logic so the game runs at the exact same speed whether you are hitting 60 FPS or 500 FPS.
* **Sliding Wall Collisions:** Smooth AABB-style sliding collision handling preventing the player from getting stuck on geometry.

---

## Project Structure

```text
Arthur/
├── src/
│   ├── main.cpp       # Game loop initialization & frame dispatching
│   ├── Engine.h       # Raycasting algorithm & core rendering pipeline
│   ├── Player.h       # Input handling, movement, and collision math
│   └── Map.h          # Grid layout definitions & collision checking
├── assets/            # Future textures, audio, and font files
└── CMakeLists.txt     # Modern CMake configuration build file
```
---
## Controls
| Key | Action |
| :--- | :--- |
| **W** | Walk Forward |
| **S** | Walk Backward |
| **A** | Strafe / Turn Left |
| **D** | Strafe / Turn Right |
| **Esc** | Exit Game |
---
## How to Build and Run
This project uses CMake to completely automate the setup process. It will automatically download, compile, and link the required `raylib` binaries behind the scenes.

### Prerequisites
Make sure you have a C++ compiler installed (GCC, Clang, or MSVC via Visual Studio) and CMake (version 3.11 or higher).

### Compilation Steps

1. Open your terminal/command prompt and navigate to the project directory:

```bash
   cd path/to/Arthur
```

2. Create and enter a build directory:

```bash
   mkdir build && cd build
```

3. Generate the build files:

```bash
   cmake ..
```

4. Build the executable:

```bash
   cmake --build .
```

5. Run the game:

Windows: `.\Debug\Arthur.exe`

Mac/Linux: `./Arthur`

---

## Roadmap / Future Implementations
Wall Textures: Replace solid color rendering with vertical texture mapping.

Mini-Map Overlay: Add a toggleable HUD component showing player position in real-time.

Sprites: Implement a Z-buffer depth system to draw enemies, items, and decorations.

Audio: Integrate background music and directional sound effects for footsteps and impacts.