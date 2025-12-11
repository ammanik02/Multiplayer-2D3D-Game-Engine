@echo off
REM Run Example Game (AI Demo)

echo Searching for GameEngine.exe...

REM Check common build output locations
if exist "build\Release\GameEngine.exe" (
    cd build\Release
    REM We need to go back up to root to find examples if we cd'd into Release
    REM Actually, the engine looks for config relative to CWD.
    REM A safer bet is to call the exe from the root directory if possible, or pass absolute paths.
    REM But let's assume assets are copied next to binary.
) else if exist "build\Debug\GameEngine.exe" (
    cd build\Debug
) else if exist "build\GameEngine.exe" (
    cd build
) else (
    echo Error: Could not find GameEngine.exe.
    echo Please build the project first using CMake.
    pause
    exit /b
)

echo Starting Example Game (Player vs AI)...
REM The config path needs to be relative to the executable OR absolute.
REM If we are in build/Release, the examples are in ../../examples (if not copied)
REM BUT we added a CMake command to copy examples to the output dir!
REM So "examples/demo_game/config.json" should work relative to the EXE.

GameEngine.exe --config examples/demo_game/config.json

pause
