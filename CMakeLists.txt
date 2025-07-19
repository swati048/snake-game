cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Enable static linking
set(SFML_STATIC_LIBRARIES TRUE)
add_definitions(-DSFML_STATIC)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include headers
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

add_executable(snake ${SOURCES})

# Copy resources folder into the build directory
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

# Link SFML static libraries and required Windows libs
target_link_libraries(snake
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    winmm
    gdi32
    opengl32
    freetype
    ws2_32
)
