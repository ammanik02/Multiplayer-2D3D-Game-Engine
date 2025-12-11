# Multiplayer 2D/3D Game Engine

A custom "Mini-Unity" engine written in C++17 with Python scripting and ENet multiplayer.

## Features
- **Core**: SDL2 windowing and input, Logger.
- **Rendering**: Modern OpenGL 3.3+ (3D Cubes).
- **Scripting**: Embedded Python 3 via `pybind11`.
- **Networking**: Client-Server Reliable UDP via `ENet`.
- **Data-Driven**: JSON Scenes and Configs.

## Quick Start (Windows)
1. **Build**: Run `build_project.bat` (Requires CMake).
2. **Run Multiplayer**: Double-click `run_multiplayer.bat`.
3. **Run Example Game**: Double-click `run_example_game.bat`.

## Prerequisites (Detailed)
If the build fails, ensure you have:
1. **Visual Studio 2022 Community**:
   - Open **Visual Studio Installer**.
   - Click **Modify**.
   - Select the **Workloads** tab.
   - check **"Desktop development with C++"** (under Desktop & Mobile).
   - Click **Modify/Install** in the bottom right.
2. **CMake**: Installed and added to PATH.

## Manual Build Instructions
Requirements: CMake 3.14+, Python 3.8+ (Dev), C++ Compiler.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Engine

### Main Demo
```bash
./GameEngine
```
Controls: WASD to move.

### Example Game (AI Demo)
```bash
./GameEngine --config examples/demo_game/config.json
```
shows a "Player vs Enemy" setup with Python-driven AI.

### Multiplayer
1. **Server**: `./GameEngine server`
2. **Client 1**: `./GameEngine`
3. **Client 2**: `./GameEngine`

## Documentation
- [Architecture](docs/ARCHITECTURE.md)
- [Networking & Multiplayer](docs/NETWORKING.md)
- [Scripting API](docs/SCRIPTING.md)

## Structure
- `src/`: C++ Source Code.
- `scripts/`: Python scripts for the main demo.
- `scenes/`: JSON scene definitions.
- `examples/`: Configs and scripts for other demos.
- `docs/`: Detailed documentation.
