*This project has been created as part of the 42 curriculum by yotsurud.*

# SCOP

## Overview
- [Description](#description)
- [Instructions](#instructions)
- [Project Structure](#project-structure)
- [Main features](#main-features)
- [Controls](#controls)
- [References](#references)
- [AI Usage](#ai-usage)

## Description

This project is an exercise designed to learn the basics of 3D rendering using OpenGL.
It loads an `.obj` file and displays the 3D object in a window.
The object can be rotated, moved, and scaled using keyboard inputs.
Additionally, a texture can be loaded and applied to the 3D object.

## Instructions

### Requirements
- **OS:** Linux / macOS
- **Compiler:** C++17 compatible compiler
- **Graphics API:** OpenGL 3.3 Core Profile
- **Window / Input:** GLFW
- **OpenGL Loader:** GLAD (included in the repository)
- **Build Tool:** Make

GLFW is already installed in the 42 school environment.
GLAD is included in this repository, so no additional installation is required.
```bash
includes/
└── glad/
    ├── include/
    │   ├── glad/
    │   │   └── glad.h
    │   └── KHR/
    │       └── khrplatform.h
    └── src/
        └── glad.c
```
### Installation
```bash
    git clone <repository of this project>
    cd project-name
```

### Compilation
Compile the project with:
```bash
    make
```
This creates the `SCOP` executable.
Other available commands:
- `make clean` - removes object and dependency files
- `make fclean` - removes object files, dependency files, and the executable
- `make re` - rebuilds the project
    
### Usage
```bash
    ./SCOP <obj_file> <bmp_file>
```

## Project Structure

The project is organized into modules based on their responsibilities.

- `gl/`: OpenGL rendering and window management
- `math/`: Vector and matrix calculations
- `parser/`: OBJ and BMP file parsing
- `validate/`: Mesh geometry diagnostics
- `utils/`: Utility functions
```bash
    .
    ├── img
    ├── includes
    │   ├── gl
    │   ├── glad
    │   │   ├── include
    │   │   └── src
    │   ├── math
    │   ├── parser
    │   ├── utils
    │   └── validate
    ├── resources
    ├── shaders
    └── srcs
        ├── gl
        ├── math
        ├── parser
        ├── utils
        └── validate
```

## Main features

### Mandatory
- Custom OBJ file parser
- Custom 24-bit BMP image loader
- OpenGL 3.3 Core Profile rendering
- Model, view, and projection transformations
- Perspective projection with aspect-ratio correction
- Depth testing
- Grayscale face coloring
- Texture mapping
- Toggle between textured and non-textured rendering
- Load and apply image textures using OpenGL
- Object rotation, translation, and scaling using keyboard controls
- GLSL vertex and fragment shaders

### Bonus: Complex Polygon Handling
This project supports triangulation of concave polygons using the Ear Clipping algorithm.
For non-coplanar polygons, Newell's Method is used to estimate a representative normal vector and select an appropriate 2D projection plane. Ear Clipping is then performed on the projected vertices, while the original 3D coordinates are preserved for rendering.
- Ear Clipping triangulation for concave polygons
- Non-coplanar polygon handling using Newell's Method for projection-plane selection
- Triangle Fan triangulation for comparison and demonstration

### Bonus: Texture Mapping
This project supports texture mapping using automatically generated UV coordinates.
- Automatic planar UV projection based on face orientation
- UV mapping using XY, XZ, or YZ projection planes
- Adjust texture repetition interactively using keyboard controls

Controls:
- T: Toggle texture mapping on / off
- C: Increase texture repetition
- V: Decrease texture repetition

### Bonus: Magic Pocket
Press P to add a new instance of the loaded object with a randomized position, scale, and rotation axis.

- P: Add a new object instance
- K: Remove all added instances, keeping the first instance
- SPACE: Rotate each instance around its own randomly assigned axis

All instances share the same mesh data (VAO/VBO), avoiding duplicate GPU mesh uploads.

### Additional Feature: Mesh Diagnostics
- Degenerate Triangle Detection
- Face Winding Validation
- Geometric Edge Analysis
- Zero-Length Edge Detection
- Triangle Shape Analysis
- Non-Coplanar Face Detection

### Demonstration

- Planar star: Demonstrates the difference between Triangle Fan and Ear Clipping triangulation.
- Non-coplanar pentagon: Demonstrates triangulation and rendering of a non-coplanar polygon.
- Non-coplanar star: Demonstrates the combined handling of concave and non-coplanar geometry.

Triangle Fan triangulation is retained for comparison during evaluation.
To compare Triangle Fan and Ear Clipping triangulation, switch the function call in Object::setVertexData() and rebuild the project.

By default, Ear Clipping triangulation is enabled.

Limitation: The triangulation assumes that the polygon has a valid, non-self-intersecting 2D projection. Arbitrary non-coplanar polygons may require additional handling.

## Controls

The object can be manipulated using the following keyboard controls:

| Key | Action |
|---|---|
| `←` / `→` | Rotate around the Y-axis |
| `↑` / `↓` | Rotate around the X-axis |
| `Q` / `E` | Rotate around the Z-axis |
| `A` / `D` | Move along the X-axis |
| `R` / `F` | Move along the Y-axis |
| `W` / `S` | Move along the Z-axis |
| `Z` / `X` | Scale the object up / down |
| `T` | Toggle texture on / off |
| `C` / `V` | Increase / decrease texture repetition |
| `P` | Add a new object instance |
| `K` | Remove all added instances, keeping the first instance |
| `SPACE` | Rotate each instance around its own randomly assignd axis |
| `ESC` | Close the window |

## References

- [LearnOpenGL](https://learnopengl.com/)
- [GLFW Documentation](https://www.glfw.org/docs/latest/quick.html)
- [OpenGL Reference](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf)
- [GLAD](https://glad.dav1d.de)
- [BMP file format](https://www.setsuki.com/hsp/ext/bmp.htm)
- [Area of Polygon](https://gihyo.jp/dev/serial/01/geometry/0008)
- [Ear Clipping Triangulation](https://qiita.com/fujii-kotaro/items/a411f2a45627ed2f156e)
- [Checking Whether a Point Lies on a Triangle's Vertices](http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html)
- [Normal vector of a polygon](https://web.ist.utl.pt/~antonio.menezes.leitao/Docs/Books/BookJulia/Parametric_Representation/Surface_Normals/index.html)

## AI Usage

AI was used as a learning and development support tool for the following tasks:

- Understanding OpenGL concepts and the rendering pipeline
- Understanding model, view, and projection transformations
- Reviewing class responsibilities and code structure
- Understanding the OBJ and BMP file formats
- Understanding UV mapping and texture stretching
- Debugging and reviewing code
- Improving documentation and README structure
