#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : Position(position), Front(front), Up(up)
{
}

void Camera::ProcessInput(SDL_Event event)
{
  if ((event.type == SDL_EVENT_KEY_DOWN) || (event.type == SDL_EVENT_KEY_UP)) {
    ProcessKeyboardInput(event);
  }
  if (event.type == SDL_EVENT_MOUSE_MOTION) {
    ProcessMouseMotion(event);
  }
  if (event.type == SDL_EVENT_MOUSE_WHEEL) {
    ProcessMouseWheel(event);
  }
}

void Camera::ProcessMouseMotion(SDL_Event event)
{
  float xoffset = (float)event.motion.xrel;
  float yoffset = (float)event.motion.yrel;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch -= yoffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }if (pitch < -89.0f) {
    pitch = -89.0f;
  }
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  Front = glm::normalize(direction);
}

void Camera::ProcessMouseWheel(SDL_Event event)
{
  fov -= (float)event.wheel.y;
  if (fov < 1.0f) {
    fov = 1.0f;
  }
  if (fov > 45.0f) {
    fov = 45.0f;
  }
}

void Camera::ProcessCameraMovement(float deltaTime)
{
  const float cameraSpeed = 2.50f * deltaTime;
  if (moveArray[FORWARD]) {
    Position += cameraSpeed * Front;
  }
  if (moveArray[BACKWARD]) {
    Position -= cameraSpeed * Front;
  }
  if (moveArray[LEFT]) {
    Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
  }
  if (moveArray[RIGHT]) {
    Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
  }
}

void Camera::ProcessKeyboardInput(SDL_Event event)
{
  if (event.type == SDL_EVENT_KEY_DOWN) {
    if (event.key.scancode == SDL_SCANCODE_W) {
      moveArray[FORWARD] = 1;
    }
    if (event.key.scancode == SDL_SCANCODE_S) {
      moveArray[BACKWARD] = 1;
    }
    if (event.key.scancode == SDL_SCANCODE_A) {
      moveArray[LEFT] = 1;
    }
    if (event.key.scancode == SDL_SCANCODE_D) {
      moveArray[RIGHT] = 1;
    }
  }
  if (event.type == SDL_EVENT_KEY_UP) {
    if (event.key.scancode == SDL_SCANCODE_W) {
      moveArray[FORWARD] = 0;
    }
    if (event.key.scancode == SDL_SCANCODE_S) {
      moveArray[BACKWARD] = 0;
    }
    if (event.key.scancode == SDL_SCANCODE_A) {
      moveArray[LEFT] = 0;
    }
    if (event.key.scancode == SDL_SCANCODE_D) {
      moveArray[RIGHT] = 0;
    }
  }
}

glm::mat4 Camera::GetViewMatrix()
{
  return glm::lookAt(Position, Position + Front, Up);
}
