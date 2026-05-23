# Arthur

A custom-built, pseudo-3D first-person shooter and dungeon crawler written in modern C++ using [Raylib](https://www.raylib.com/). Inspired by classic raycasting engines like *Wolfenstein 3D* and *DOOM*, this project features a fully functional rendering pipeline, finite state machine (FSM) enemy AI, and multi-level progression.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Raylib](https://img.shields.io/badge/Raylib-5.0-black.svg)
![CMake](https://img.shields.io/badge/CMake-Build-green.svg)

## Key Features

* **Custom Raycasting Engine:** Renders a 2D grid map into a 3D perspective using custom raycasting mathematics, Z-buffering for sprite occlusion, and distance-based depth shading.
* **Finite State Machine (FSM) AI:** Enemies operate autonomously using a state machine (`Idle`, `Wander`, `Chase`, `Attack`, `Flinch`, `Die`). Enemies roam randomly until the player enters their line of sight.
* **Diverse Bestiary:** 5 distinct enemy types with unique stats (Speed, Damage, Health, Cooldowns):
  * 🧙‍♂️ **Wizards:** Standard ranged casters.
  * 🧌 **Goblins:** Fast, low-health swarmers.
  * 💀 **Skeletons:** Slow, high-health tanks that deal massive damage.
  * 🍄 **Mushrooms:** Toxic, high-health ambushers.
  * 👁️ **Flying Eyes:** Extremely fast, agile swarmers.
* **Multi-Level Progression:** 5 distinct levels, escalating from tight corridors to a massive 60x60 open-arena horde survival finale.
* **Dynamic HUD & Radar:** Real-time minimap with color-coded enemy tracking and player field-of-view indicators.
* **Audio Streaming:** Continuous background music streaming using Raylib's hardware audio device integration.

## Tech Stack & Dependencies

* **Language:** C++17
* **Graphics & Audio API:** Raylib 
* **Build System:** CMake
* **Platform:** Windows / Linux / macOS

## Build Instructions

This project uses CMake for cross-platform building. Raylib is automatically fetched and linked via CMake's `FetchContent` module, meaning you do not need to install Raylib manually.

### Prerequisites
* A C++17 compatible compiler (MSVC, GCC, or Clang)
* [CMake](https://cmake.org/download/) (v3.15 or higher)

### Compiling from Source

1. **Clone the repository:**
   ```bash
   git clone https://github.com/IsaMaharramov/Arthur.git
   cd Arthur
   ```
2. **Generate build files:**
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
3. **Build the executable:**
   ```bash
   cmake --build .
   ```
4. **Run the game:**
   ```powerShell
   # On Windows
   .\Debug\Arthur.exe

   # On Linux/macOS
   ./Arthur
   ```
>Note: Ensure the `assets/` folder remains in the project root. The engine uses relative pathing (`../assets/`) to locate textures and audio from the build directory.

## Project Structure

```plaintext
ArthurEngine/
├── assets/                 # Textures, sprite sheets, and audio files
│   ├── goblin/
│   ├── skeleton/
│   ├── mushroom/
│   ├── flying_eye/
│   └── ...
├── src/                    # C++ Source Code
│   ├── main.cpp            # Game loop and level transition logic
│   ├── Engine.h            # Rendering pipeline, HUD, and Audio manager
│   ├── Map.h               # Level generation, collision, and AI routing
│   ├── Player.h            # Input handling, movement, and raycast shooting
│   └── Enemy.h             # Data structures and Enums for the FSM
├── CMakeLists.txt          # CMake configuration and dependency fetching
└── README.md               # Project documentation
```

## Controls
* **W / S:** Move Forward / Backward

* **A / D:** Rotate Camera Left / Right

* **Left Mouse Button:** Fire Weapon

* **ESC:** Exit Game

## Contributing

Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement". 

See [`CONTRIBUTING.md`](/CONTRIBUTING.md) for more details.

## Credits & Assets

[Audio and Art Assets](/assets/README.md)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.