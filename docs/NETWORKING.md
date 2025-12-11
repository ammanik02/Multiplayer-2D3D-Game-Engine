# Networking Guide

## How to Run Multiplayer
1. Open Terminal A (Server):
   ```bash
   ./GameEngine server --config config.json
   ```
2. Open Terminal B (Client 1):
   ```bash
   ./GameEngine --config config.json
   ```
3. Open Terminal C (Client 2):
   ```bash
   ./GameEngine --config config.json
   ```

## Entity Replication
- **Authority**: The Server is the source of truth.
- **Replication**:
    1. Server loops through all entities.
    2. Packages `id, x, y` into a `StatePacket`.
    3. Broadcasts to all connected peers.
    4. Clients receive packet, find entity by `id`, and snap `x, y` to new values.

## Input Handling
- **Prediction**: Currently Client-side prediction is minimal (visual only via script, but overridden by network state).
- **Flow**:
    1. Client polls `Input` (WASD).
    2. Sends `InputPacket` to Server.
    3. Server receives packet, applies velocity to the controlled `Entity`.
    4. Entity moves on Server.
    5. Server sends new position back to Client.
