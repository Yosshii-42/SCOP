*This project has been created as part of the 42 curriculum by yotsurud.*

# SCOP

## Description

SCOP is a 42 project that introduces the fundamentals of 3D rendering using OpenGL.

The goal of the project is to load a three-dimensional object from a Wavefront OBJ
file and render it in a window using a modern graphics pipeline.

The program parses the geometry of the OBJ file, converts polygon faces into
triangles, transfers vertex data to the GPU, and renders the model using GLSL shaders.

The rendered object can be rotated, translated, and scaled. Its appearance can be
switched between grayscale face coloring and texture mapping.

The project implements its own OBJ parser, BMP image loader, vector and matrix
operations, model transformations, and UV coordinate generation.

### Main features

- Wavefront OBJ file parsing
- Custom 24-bit BMP image loader
- OpenGL 3.3 Core Profile rendering
- Model, view, and projection transformations
- Perspective projection
- Depth testing
- Grayscale face coloring
- Texture mapping
- Automatic planar UV projection based on face orientation
- Adjustable texture scale
- Object rotation, translation, and scaling
- Window resizing with aspect-ratio correction
- GLSL vertex and fragment shaders

## Instructions

### Requirements

The project requires:

- A C++17 compiler
- OpenGL
- GLFW

GLAD is included in the project.

#### macOS

GLFW can be installed with Homebrew:

```bash
brew install glfw