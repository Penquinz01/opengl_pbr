#pragma once

#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "sstream"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "Shader.h"
#include "File.h"
#include "assimp/config.h"

inline SDL_Window* window;
inline float deltaTime = 0.0f;
inline float lastFrame = 0.0f;

// lighting
inline glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
inline glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
inline float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

inline glm::vec3 positions[] = {
  glm::vec3(0.0f,0.0f,0.0f),
  glm::vec3(1.7f,2.5f,3.0f),
  glm::vec3(5.0f,3.0f,3.0f),
  glm::vec3(-1.0f,0.3f,-4.0f),
  glm::vec3(-2.3f,-2.3f,-1.0f)
};

inline glm::vec3 pointLightPositions[] = {
  glm::vec3(0.7f, 0.2f, 2.0f),
  glm::vec3(2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f, 2.0f, -12.0f),
  glm::vec3(0.0f, 0.0f, -3.0f)
};
inline glm::vec3 ambients[] = {
    glm::vec3(0.2f,0.2f,0.2f),
    glm::vec3(0.0f,0.2f,0.0f),
    glm::vec3(0.2f,0.0f,0.0f),
    glm::vec3(0.0f,0.0f,0.2f)
};

inline glm::vec3 diffuse[] = {
    glm::vec3(0.8f),
    glm::vec3(0.0f,0.8f,0.0f),
    glm::vec3(0.8f,0.0f,0.0f),
    glm::vec3(0.0f,0.0f,0.8f)
};
inline glm::vec3 specular[] = {
    glm::vec3(1.0f),
    glm::vec3(0.0f,1.0f,0.0f),
    glm::vec3(1.0f,0.0f,0.0f),
    glm::vec3(0.0f,0.0f,1.0f)
};

inline float GetTime() {
    return (float)SDL_GetTicks() / 1000.0f;
}