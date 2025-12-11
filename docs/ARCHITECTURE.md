# Engine Architecture

## 1. Core System (`Engine.cpp`)
The heart of the engine.
- **Initialization**: Sets up SDL2 (OpenGL Context), ENet, and GLAD.
- **Game Loop**: Standard `Input -> Network -> Update -> Render` loop.
- **Time**: Calculates `dt` (Delta Time) via high-performance counters.

## 2. Entity Component System (Lite)
Currently object-oriented `Entity` class.
- **Data**: `id`, `x,y,z` (Position), `vx,vy` (Velocity), `r,g,b` (Color).
- **Logic**: Can have a **Python Script** attached.
- **Render**: Submits itself to the Renderer (as a primitive Cube).

## 3. Scripting (`Scripting.cpp`)
Uses `pybind11` to embed Python.
- **API**: Exposes `Entity` and `Input` to Python.
- **Lifecycle**: `attach_script()` creates a Python instance. `update()` is called every frame from C++.

## 4. Networking (`Network.cpp`)
Uses `ENet` (Reliable UDP).
- **Client-Server**:
  - **Server**: Authoritative. Receives input, runs logic, broadcasts state.
  - **Client**: Dumb terminal. Sends input, visualizes received state.
- **Packets**:
  - `INPUT`: Keys pressed.
  - `STATE`: Snapshot of entity positions.

## 5. Rendering (`Renderer/`)
Modern OpenGL 3.3 Core.
- **Shader**: Handled by `Shader` class.
- **Geometry**: Handled by `Mesh` class (VAO/VBO).
- **Loop**: Clears screen -> Sets Matrices (Perspective/View) -> Draws Meshes.
