#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL3/SDL.h>
#include <glad/glad.h>
enum Direction {
  FORWARD = 0,
  BACKWARD = 1,
  LEFT = 2,
  RIGHT = 3,
  UP = 4,
  DOWN = 5
};

class Camera {
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  float fov = 45.0f;

  Camera(glm::vec3 position,glm::vec3 front,glm::vec3 up);
  void ProcessInput(SDL_Event event);
  glm::mat4 GetViewMatrix();
  void ProcessCameraMovement(float deltaTime);

private:
  float yaw = -90.0f;
  float pitch = 0.0f;
  float sensitivity = 0.1f;

  bool moveArray[6] = { false,false,false,false,false,false };
  void ProcessMouseMotion(SDL_Event event);
  void ProcessMouseWheel(SDL_Event event);
  void ProcessKeyboardInput(SDL_Event event);
  void ProcessMouseInput(SDL_Event event);
};