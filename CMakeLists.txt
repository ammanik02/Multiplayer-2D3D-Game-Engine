cmake_minimum_required(VERSION 3.14)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
include(FetchContent)

# --- Dependencies ---
include(FetchContent)

# 1. SDL2
FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-2.28.5.zip
)
FetchContent_MakeAvailable(SDL2)

# 2. Python
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)

# 3. pybind11
FetchContent_Declare(
    pybind11
    URL https://github.com/pybind/pybind11/archive/refs/tags/v2.11.1.zip
)
FetchContent_MakeAvailable(pybind11)

# 4. ENet
FetchContent_Declare(
    enet
    URL https://github.com/zpl-c/enet/archive/refs/tags/v1.3.17.zip
)
FetchContent_MakeAvailable(enet)

# 5. GLM
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
)
FetchContent_MakeAvailable(glm)

# 6. GLAD
FetchContent_Declare(
    glad
    URL https://github.com/Dav1dde/glad/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(glad)

# 7. nlohmann_json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# --- Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# --- Executable ---
add_executable(GameEngine ${SOURCES})

# --- Include Directories ---
target_include_directories(GameEngine PRIVATE 
    ${glm_SOURCE_DIR}
    ${glad_SOURCE_DIR}/include
    ${json_SOURCE_DIR}/include
)

# --- Link Libraries ---
target_link_libraries(GameEngine PRIVATE 
    SDL2::SDL2main 
    SDL2::SDL2-static
    pybind11::embed
    Python::Python
    enet
    glad
    nlohmann_json::nlohmann_json
)

# --- Post-Build Steps ---
# Copy SDL2.dll
if(WIN32)
    add_custom_command(TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:GameEngine>
    )
endif()

# Copy Scripts folder
add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/scripts
    $<TARGET_FILE_DIR:GameEngine>/scripts
)

# Copy Scenes folder
add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/scenes
    $<TARGET_FILE_DIR:GameEngine>/scenes
)

# Copy Assets folder (if exists, though currently empty)
add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:GameEngine>/assets
)

# Copy Examples folder
add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/examples
    $<TARGET_FILE_DIR:GameEngine>/examples
)

# Copy default config
add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config.json
    $<TARGET_FILE_DIR:GameEngine>/config.json
)
