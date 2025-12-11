# Scripting & Networking Guide

## Scripting API
The engine exposes the following C++ classes to Python:

### `Entity`
Represents a game object.
- **Properties**:
  - `x`, `y`, `z`: Position (float)
  - `vx`, `vy`: Velocity (float)
  - `ry`: Y-rotation (float)
- **Methods**:
  - `move(dx, dy)`: Move manually.

### `Input`
- `is_key_down(scancode)`: Returns true if key is pressed.

## Networking Model
The engine uses a **Authoritative Server** model.

### Server
- Runs logic (scripts).
- Receives Input packets from clients.
- Updates Entity positions.
- Broadcasts State packets (Position snapshots) to clients.

### Client
- Samples Input and sends to Server.
- Receives State packets and updates (snaps) local Entity positions.
- Visuals (Rendering) happen locally.

## Creating a New Script
1. Create a file in `scripts/` (e.g. `my_script.py`).
2. Define a class (e.g. `MyBehavior`).
3. Add an `update(self, dt)` method.
4. Add it to `config.json` scene:
   ```json
   {
       "type": "MyEntity",
       "script_module": "my_script",
       "script_class": "MyBehavior",
       ...
   }
   ```
