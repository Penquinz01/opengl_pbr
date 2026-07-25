#include "Stencil.h"
#include <map>


#include "../Model/Model.h"


glm::vec3 newPos(5.0f,5.0f,5.0f);

unsigned int VBO, VAO, LightVAO;
int width, height;
unsigned int diffuseMap, specularMap, emissionMap;
std::vector<glm::vec3> windows;

float transparentVertices[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};


void StartStencilTest() {
    std::string modelPath = "res/models/backpack/backpack.obj";
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL Cant be initialized" << std::endl;
        return;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window = SDL_CreateWindow("LightMap", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Failed to Create Window.Error : %s\n", SDL_GetError());
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
    Shader objectShader("shaders/LightMaps/lightMap.vert", "shaders/LightMaps/multipleLight.frag");
    Shader modelShader("shaders/BackPack/backpack.vert", "shaders/BackPack/backpack.frag");
    Shader outLineShader("shaders/LightMaps/lightMap.vert","shaders/Outline/Outline.frag");
    Shader grassShader("shaders/Texture/grass.vert", "shaders/Texture/grass.frag");

    windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    windows.push_back(glm::vec3(1.5f, 0.0f, 1.5f));
    windows.push_back(glm::vec3(0.0f, 0.0f,
        0.7f));
    windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));


    const char* diffusePath = "res/textures/container2.png";
    const char* specularPath = "res/textures/container2_specular.png";
    const char* emissionPath = "res/textures/matrix.jpg";

    const char* windowPath = "res/textures/window.png";

    diffuseMap = loadTexture(diffusePath);
    specularMap = loadTexture(specularPath);
    emissionMap = loadTexture(emissionPath);


    unsigned int windowTexture = loadTexture(windowPath);

    Model backPack(modelPath.c_str());

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

  


    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);



    //Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //TexCoords

    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vegetationVAO,vegetationVBO;
    glGenVertexArrays(1, &vegetationVAO);
    glGenBuffers(1, &vegetationVBO);
    glBindVertexArray(vegetationVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

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

        width, height;
        SDL_GetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        camera.ProcessCameraMovement(deltaTime);
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)width / height, 0.1f, 100.0f);

        glStencilMask(0x00);
        lightSourceShader.use();
        for (int i = 0; i < 4; i++) {
            lightSourceShader.setVec3("lightColor", ambients[i]);
            lightSourceShader.setMat4("projection", projection);
            lightSourceShader.setMat4("view", view);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightSourceShader.setMat4("model", model);
            glBindVertexArray(LightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        objectShader.use();
        objectShader.setVec3("viewPos", camera.Position);
        objectShader.setVec3("dirLight.direction", lightDir);
        objectShader.setVec3("dirLight.ambient", glm::vec3(0.2f));
        objectShader.setVec3("dirLight.diffuse", glm::vec3(0.8f));
        objectShader.setVec3("dirLight.specular", glm::vec3(1.0f));
        for (int i = 0; i < 4; i++) {
            std::string number = std::to_string(i);
            objectShader.setVec3("pointLights[" + number + "].position", pointLightPositions[i]);
            objectShader.setVec3("pointLights[" + number + "].ambient", ambients[i]);
            objectShader.setVec3("pointLights[" + number + "].diffuse", diffuse[i]);
            objectShader.setVec3("pointLights[" + number + "].specular", specular[i]);
            objectShader.setFloat("pointLights[" + number + "].constant", 1.0f);
            objectShader.setFloat("pointLights[" + number + "].linear", 0.07f);
            objectShader.setFloat("pointLights[" + number + "].quadratic", 0.017f);
        }
        objectShader.setVec3("spotLight.position", camera.Position);
        objectShader.setVec3("spotLight.direction", camera.Front);
        objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(20.0f)));
        objectShader.setVec3("spotLight.ambient", glm::vec3(0.2f, 0.0f, 0.0f));
        objectShader.setVec3("spotLight.diffuse", glm::vec3(0.8f, 0.0f, 0.0f));
        objectShader.setVec3("spotLight.specular", glm::vec3(1.0f, 0.0f, 0.0f));
        objectShader.setFloat("spotLight.constant", 1.0f);
        objectShader.setFloat("spotLight.linear", 0.22f);
        objectShader.setFloat("spotLight.quadratic", 0.20f);


        objectShader.setInt("material.specular", 1);
        objectShader.setInt("material.emmission", 2);
        objectShader.setFloat("material.shininess", 32.0f);
        objectShader.setInt("material.diffuse", 0);
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);
        size_t count = sizeof(positions) / sizeof(positions[0]);
        for (int i = 0; i < count; i++) {
            float angle = GetTime() * 20.0f * i;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objectShader.setMat4("model", model);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, emissionMap);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        modelShader.use();
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        modelShader.setMat4("model", model);
        modelShader.setVec3("light.position", lightPos);
        modelShader.setVec3("light.ambient", glm::vec3(0.2f));
        modelShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.4f, 0.6f));
        modelShader.setVec3("light.specular", glm::vec3(1.0f));
        modelShader.setFloat("light.contant", 1.0f);
        modelShader.setFloat("light.linear", 0.22f);
        modelShader.setFloat("light.quadratic", 0.20f);
        modelShader.setVec3("spotlight.position", camera.Position);
        modelShader.setVec3("spotlight.direction", camera.Front);
        modelShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
        modelShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(20.0f)));
        modelShader.setFloat("spotlight.constant", 1.0f);
        modelShader.setFloat("spotlight.linear", 0.22f);
        modelShader.setFloat("spotLight.quadratic", 0.20f);
        modelShader.setVec3("spotlight.ambient", glm::vec3(0.2f));
        modelShader.setVec3("spotlight.diffuse", glm::vec3(0.8f));
        modelShader.setVec3("spotlight.specular", glm::vec3(1.0f));
        backPack.Draw(modelShader);


        glStencilFunc(GL_ALWAYS,1,0xFF);
        glStencilMask(0xFF);
        objectShader.use();
        DrawCube(camera,objectShader);

        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);
        outLineShader.use();
        DrawScaledUpCube(camera,outLineShader);
        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS,0,0xFF);
        glEnable(GL_DEPTH_TEST);

        grassShader.use();
        glBindVertexArray(vegetationVAO);
        grassShader.setMat4("view", view);
        grassShader.setMat4("projection",projection);
        grassShader.setInt("texture1",0);
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++) {
            float distance = glm::length(camera.Position - windows[i]);
            sorted[distance] = windows[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            grassShader.setMat4("model",model);
            glBindVertexArray(vegetationVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, windowTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }


        SDL_GL_SwapWindow(window);

    }


    SDL_DestroyWindow(window);
    SDL_Quit();
}

void DrawCube(Camera camera,Shader objectShader) {
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)width / height, 0.1f, 100.0f);
    objectShader.setVec3("viewPos", camera.Position);
    objectShader.setVec3("dirLight.direction", lightDir);
    objectShader.setVec3("dirLight.ambient", glm::vec3(0.2f));
    objectShader.setVec3("dirLight.diffuse", glm::vec3(0.8f));
    objectShader.setVec3("dirLight.specular", glm::vec3(1.0f));
    for (int i = 0; i < 4; i++) {
        std::string number = std::to_string(i);
        objectShader.setVec3("pointLights[" + number + "].position", pointLightPositions[i]);
        objectShader.setVec3("pointLights[" + number + "].ambient", ambients[i]);
        objectShader.setVec3("pointLights[" + number + "].diffuse", diffuse[i]);
        objectShader.setVec3("pointLights[" + number + "].specular", specular[i]);
        objectShader.setFloat("pointLights[" + number + "].constant", 1.0f);
        objectShader.setFloat("pointLights[" + number + "].linear", 0.07f);
        objectShader.setFloat("pointLights[" + number + "].quadratic", 0.017f);
    }
    objectShader.setVec3("spotLight.position", camera.Position);
    objectShader.setVec3("spotLight.direction", camera.Front);
    objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(20.0f)));
    objectShader.setVec3("spotLight.ambient", glm::vec3(0.2f, 0.0f, 0.0f));
    objectShader.setVec3("spotLight.diffuse", glm::vec3(0.8f, 0.0f, 0.0f));
    objectShader.setVec3("spotLight.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    objectShader.setFloat("spotLight.constant", 1.0f);
    objectShader.setFloat("spotLight.linear", 0.22f);
    objectShader.setFloat("spotLight.quadratic", 0.20f);


    objectShader.setInt("material.specular", 1);
    objectShader.setInt("material.emmission", 2);
    objectShader.setFloat("material.shininess", 32.0f);
    objectShader.setInt("material.diffuse", 0);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, newPos);
    objectShader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionMap);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DrawScaledUpCube(Camera camera,Shader shaderSingleColor) {
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)width / height, 0.1f, 100.0f);
    float scale = 1.05f;
    // cubes
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, newPos);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    shaderSingleColor.setMat4("model", model);
    shaderSingleColor.setMat4("view", view);
    shaderSingleColor.setMat4("projection",projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}




