@echo off
echo Building Game Engine...

REM Clean previous build (helps with cache errors)
if exist build (
    rmdir /s /q build
)
mkdir build
cd build

REM Try to generate for Visual Studio 2022
echo Generating Project Files...
cmake ..
if %errorlevel% neq 0 (
    echo.
    echo ----------------------------------------------------------------
    echo  ERROR: CMake Generation Failed!
    echo ----------------------------------------------------------------
    echo  Possible Causes:
    echo  1. C++ Compiler not found.
    echo     SOLUTION: Install "Visual Studio 2022 Community" with "Desktop development with C++".
    echo  2. CMake not in PATH.
    echo ----------------------------------------------------------------
    pause
    exit /b
)

cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build Failed!
    pause
    exit /b
)

echo Build Success!
pause
