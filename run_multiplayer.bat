@echo off
REM Run Multiplayer Demo (Server + 2 Clients)

echo Searching for GameEngine.exe...

REM Check common build output locations
if exist "build\Release\GameEngine.exe" (
    cd build\Release
) else if exist "build\Debug\GameEngine.exe" (
    cd build\Debug
) else if exist "build\GameEngine.exe" (
    cd build
) else (
    echo Error: Could not find GameEngine.exe.
    echo Please build the project first using CMake.
    echo Expected path: build/Release/GameEngine.exe or build/GameEngine.exe
    pause
    exit /b
)

echo Found GameEngine.exe! Starting processes...

REM Start Server
echo Starting Server...
start "Game Server (Headless)" GameEngine.exe server

REM Wait for server to initialize
timeout /t 2 /nobreak >nul

REM Start Client 1
echo Starting Client 1...
start "Player 1" GameEngine.exe

REM Wait a bit
timeout /t 1 /nobreak >nul

REM Start Client 2
echo Starting Client 2...
start "Player 2" GameEngine.exe

echo Multiplayer setup complete.
