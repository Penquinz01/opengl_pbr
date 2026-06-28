#include "Lightmap.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Camera.h"
#include "../Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage.h"
#include "../File.h"

SDL_Window* window;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
float vertices[] = {
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

glm::vec3 positions[] = {
  glm::vec3(0.0f,0.0f,0.0f),
  glm::vec3(1.7f,2.5f,3.0f),
  glm::vec3(5.0f,3.0f,3.0f),
  glm::vec3(-1.0f,0.3f,-4.0f),
  glm::vec3(-2.3f,-2.3f,-1.0f)
};

float GetTime() {
  return (float)SDL_GetTicks() / 1000.0f;
}


void ligthMapStart() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "SDL Cant be initialized" << std::endl;
    return;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window = SDL_CreateWindow("LightMap", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_Log("Failed to Create Window.Error : %s\n",SDL_GetError());
    SDL_Quit();
    return;
  }
  SDL_SetWindowRelativeMouseMode(window, true);

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_Log("Failed to Create OpenGL Context.Error : %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
  }
  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  Shader lightSourceShader("shaders/Materials/3.1.light_cube.vs", "shaders/Materials/3.1.light_cube.fs");
  Shader objectShader("shaders/LightMaps/lightMap.vert", "shaders/LightMaps/spotlight.frag");




  const char* diffusePath = "res/textures/container2.png";
  const char* specularPath = "res/textures/container2_specular.png";
  const char* emissionPath = "res/textures/matrix.jpg";

  unsigned int diffuseMap = loadTexture(diffusePath);
  unsigned int specularMap = loadTexture(specularPath);
  unsigned int emissionMap = loadTexture(emissionPath);




  unsigned int VBO, VAO,LightVAO;

  glGenVertexArrays(1, &VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindVertexArray(VAO);

  //Vertices
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *)0);
  glEnableVertexAttribArray(0);

  //Normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  //TexCoords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenVertexArrays(1, &LightVAO);
  glBindVertexArray(LightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void *)0);
  glEnableVertexAttribArray(0);

  bool isRunning = true;
  SDL_Event event;
  while (isRunning) {
    float currentFrame = GetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
      if (event.type == SDL_EVENT_KEY_UP && event.key.scancode == SDL_SCANCODE_ESCAPE) {
        isRunning = false;
      }
      camera.ProcessInput(event);
    }
    camera.ProcessCameraMovement(deltaTime);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

    lightSourceShader.use();

    lightSourceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    lightSourceShader.setMat4("projection", projection);
    lightSourceShader.setMat4("view",view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightSourceShader.setMat4("model", model);
    glBindVertexArray(LightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    objectShader.use();
    objectShader.setVec3("viewPos", camera.Position);
    objectShader.setVec3("light.position", camera.Position);
    objectShader.setVec3("light.ambient", glm::vec3(0.2f));
    objectShader.setVec3("light.diffuse", glm::vec3(0.8f));
    objectShader.setVec3("light.specular", glm::vec3(1.0f));
    objectShader.setFloat("light.constant", 1.0f);
    objectShader.setFloat("light.linear", 0.09f);
    objectShader.setFloat("light.quadratic", 0.032f);
    objectShader.setVec3("light.direction", camera.Front);
    objectShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    objectShader.setFloat("light.outerCutOff", glm::cos(glm::radians(15.0f)));
    objectShader.setInt("material.specular", 1);
    objectShader.setInt("material.emmission", 2);
    objectShader.setFloat("material.shininess", 32.0f);
    objectShader.setInt("material.diffuse", 0);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);
    size_t count = sizeof(positions) / sizeof(positions[0]);
    for (int i = 0; i < count; i++) {  
        float angle =GetTime()*20.0f * i;
        model = glm::mat4(1.0f);
        model = glm::translate(model, positions[i]);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        objectShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,emissionMap);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    SDL_GL_SwapWindow(window);

  }


  SDL_DestroyWindow(window);
  SDL_Quit();
}


