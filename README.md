# OpenGL_Test

A 3D graphics rendering application built with OpenGL 3.3, featuring an interactive scene viewer with real-time lighting and a Dear ImGui-based control interface.

## Overview

This project is an OpenGL-based 3D rendering application that loads and displays 3D models from OBJ files with configurable lighting. It features a flexible level loading system that reads scene configurations from JSON files, allowing users to define multiple objects and lights in a scene. The application includes an interactive camera system and an ImGui interface for real-time manipulation of object properties and lighting parameters.

## Features

- **3D Model Rendering**: Loads and renders 3D models in OBJ format
- **Dynamic Lighting System**: Support for multiple lights with configurable position, color, and intensity
- **Level Loading**: JSON-based level configuration system for defining scenes
- **Interactive Camera**: First-person camera with keyboard controls for navigation
- **Real-time GUI**: Dear ImGui interface for adjusting:
  - Object positions, rotations, and colors
  - Light positions, colors, and intensities
  - Performance statistics (FPS, frame time)
- **Phong Lighting Model**: Fragment shader implementing diffuse and ambient lighting
- **Multi-object Support**: Render multiple objects simultaneously with independent transformations

## Dependencies

The project requires the following libraries:

- **OpenGL**: 3.3 or higher (Core Profile)
- **GLFW3**: Window creation and input handling
- **GLEW**: OpenGL Extension Wrangler (version 2.0 or higher)
- **GLM**: OpenGL Mathematics library for transformations
- **Dear ImGui**: Immediate mode GUI library (included in repository)
- **RapidJSON**: JSON parsing library (included in repository)

## Building the Project

### Requirements

- CMake 3.5 or higher
- C++11 compatible compiler
- OpenGL 3.3+ compatible graphics driver

### Build Instructions

1. Clone the repository:
```bash
git clone <repository-url>
cd OpenGL_Test
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure and build with CMake:
```bash
cmake ..
cmake --build .
```

4. Run the executable:
```bash
./OpenGL_Test
```

## Project Structure

```
OpenGL_Test/
├── main.cpp                      # Main application entry point
├── CMakeLists.txt               # CMake build configuration
├── Level_01.json                # Example level configuration file
├── SimpleVertexShader.glsl      # Vertex shader for 3D transformations
├── SimpleFragmentShader.glsl    # Fragment shader for lighting calculations
├── teapot.obj                   # Example 3D model
├── cube-test.obj                # Example cube model
├── cube.obj                     # Another cube model
├── common/                      # Core application classes
│   ├── camera.hpp/cpp          # Camera system implementation
│   ├── light.hpp/cpp           # Light data structure
│   ├── object.hpp/cpp          # 3D object wrapper
│   ├── objloader.hpp/cpp       # OBJ file parser
│   ├── lvlloader.hpp/cpp       # JSON level loader
│   ├── shader.hpp/cpp          # Shader compilation utilities
│   ├── vertex.hpp              # Vertex data structure
│   ├── normal.hpp              # Normal vector data structure
│   └── face.hpp                # Face/triangle data structure
├── imgui/                       # Dear ImGui library files
└── rapidjson/                   # RapidJSON library files
```

## Usage

### Controls

**Camera Movement:**
- `W` - Move forward
- `S` - Move backward
- `A` - Move left
- `D` - Move right
- `Space` - Move up
- `Left Ctrl` - Move down
- `Q` - Rotate camera left
- `E` - Rotate camera right

### ImGui Interface

The application displays several GUI windows:

1. **Stats Window**: Shows performance metrics
   - Frame time in milliseconds
   - Frames per second (FPS)
   - Number of objects in the scene
   - Number of lights in the scene

2. **Object Windows**: One window per object showing:
   - Current position (X, Y, Z)
   - Rotation angle in radians
   - Object color (RGB)
   - Interactive sliders to modify these properties in real-time

3. **Light Windows**: One window per light showing:
   - Light position (X, Y, Z)
   - Light color (RGB)
   - Light intensity
   - Interactive controls to adjust these properties

### Level Configuration

Levels are defined in JSON format. Example structure from `Level_01.json`:

```json
{
  "Name": "Test_Level_01",
  "Description": "",
  "Lights": [
    {
      "Position": [0.0, 3.0, 2.0],
      "Color": [1.0, 1.0, 0.0],
      "Intensity": 0.1
    }
  ],
  "Models": [
    {
      "FileName": "../teapot.obj",
      "FragmentShader": "../SimpleFragmentShader.glsl",
      "VertexShader": "../SimpleVertexShader.glsl",
      "LocationX": 0.0,
      "LocationY": -1.0,
      "LocationZ": -1.0,
      "Angle": -45.0,
      "Color": [1.0, 0.5, 0.5],
      "LoadObject": true
    }
  ]
}
```

## Technical Details

### Rendering Pipeline

1. **Model Loading**: OBJ files are parsed to extract vertices, normals, and face indices
2. **Shader Compilation**: Vertex and fragment shaders are loaded and compiled
3. **Buffer Creation**: Vertex and element buffers are created on the GPU
4. **Transformation**: Model-View-Projection matrices are computed for each object
5. **Lighting Calculation**: Multiple lights are processed in the fragment shader
6. **Rasterization**: Final pixel colors are computed using Phong lighting model

### Shaders

- **Vertex Shader** (`SimpleVertexShader.glsl`): Transforms vertices from model space to clip space and passes world-space positions and normals to the fragment shader
- **Fragment Shader** (`SimpleFragmentShader.glsl`): Implements multi-light Phong lighting with support for up to 4 lights, calculating ambient and diffuse components

### Camera System

The camera uses a look-at matrix for view transformations and supports:
- Position-based movement in 6 directions
- Rotation around the Y-axis
- Configurable movement speed

## Window Configuration

- **Resolution**: 1280x720 pixels
- **Anti-aliasing**: 4x MSAA
- **Depth Testing**: Enabled
- **Field of View**: 60 degrees
- **Near/Far Planes**: 0.1 to 100.0 units

## License

[Add your license information here]

## Credits

- **Dear ImGui**: Omar Cornut and contributors
- **RapidJSON**: Tencent and contributors
