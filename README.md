
# OpenGL PBR Learning Project

A work-in-progress graphics programming project built in C++ using OpenGL and SDL3 while following the LearnOpenGL tutorial series. The goal of this project is to understand modern rendering techniques and eventually implement a Physically Based Rendering (PBR) pipeline.

## Overview

This repository documents my journey through modern OpenGL and real-time rendering concepts. Instead of GLFW, the project uses SDL3 for window creation, input handling, and OpenGL context management.

Current Progress: **Lighting Maps**

## Technologies

* C++
* OpenGL
* SDL3
* GLAD
* GLM
* stb_image
* Visual Studio 2022

## Implemented Features

### Core Systems

* SDL3 window creation
* OpenGL context initialization
* Event handling
* Render loop
* Keyboard input processing
* Delta time updates

### Rendering Fundamentals

* Vertex Buffer Objects (VBO)
* Vertex Array Objects (VAO)
* Element Buffer Objects (EBO)
* Shader compilation and management

### Transformations

* Model matrices
* View matrices
* Projection matrices
* Camera movement

### Lighting

* Basic lighting
* Phong lighting model
* Material properties
* Diffuse textures
* Lighting maps

## Planned Features

* Specular maps
* Directional lights
* Point lights
* Spotlights
* Multiple light sources
* Model loading
* Framebuffers
* Cubemaps
* Shadow mapping
* HDR rendering
* Bloom
* Physically Based Rendering (PBR)

## Progress

* [x] Window creation
* [x] Shader system
* [x] Camera system
* [x] Basic lighting
* [x] Diffuse maps
* [x] Lighting maps
* [ ] Multiple lights
* [ ] Model loading
* [ ] Shadow mapping
* [ ] HDR
* [ ] PBR

## Current Implementation
<img width="395" height="295" alt="Screenshot 2026-06-25 173140" src="https://github.com/user-attachments/assets/5b9c54a1-a8f7-437b-9c8f-fc9a434351bf" />

## Building the Project

### Requirements

* Visual Studio 2022
* SDL3
* OpenGL
* GLAD
* GLM

### Steps

1. Clone the repository.

```bash
git clone https://github.com/Penquinz01/opengl_pbr.git
```

2. Open the Visual Studio solution file.

3. Ensure all required libraries and include paths are configured.

4. Build and run the project.

## Learning Resources

* LearnOpenGL
* OpenGL documentation
* Graphics programming references

## Goals

This project aims to develop a deeper understanding of:

* Modern OpenGL
* Real-time rendering
* Shader programming
* Graphics pipelines
* Physically Based Rendering
* Engine programming concepts

## Status

🚧 Work in Progress

The project is actively being developed while progressing through LearnOpenGL tutorials, with the long-term objective of implementing a complete PBR renderer.
