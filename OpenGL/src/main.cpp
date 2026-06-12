#include <glad/glad.h>
#include <stdio.h>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage.h"
#define get_current_dir _getcwd

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 800
#define HEIGHT 600


float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
unsigned int indices[] = {
	0, 1, 2,
	0, 2, 3,
};

float texCoords[] = {
	0.5f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
};

const char* vectexShaderSource = "shaders/vertx.vert";
const char* fragmentShaderSource = "shaders/fragment.frag";



int main() {
	std::cout << "Current working directory: "
		<< std::filesystem::current_path() << std::endl;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window;

	window = SDL_CreateWindow("[glad] GL with SDL3",WIDTH, HEIGHT,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE );

	SDL_GLContext glContext =  SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cerr << "OpenGL Context Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	int version = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	int major = 0;
	int minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL :%d.%d\n", major, minor);

	bool running = true;
	SDL_Event event;

	glViewport(0, 0, WIDTH, HEIGHT);


	Shader shaderProgram(vectexShaderSource, fragmentShaderSource);

	unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
	unsigned char* data = stbi_load("image.png",&width,&height,&nrChannels,0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
    std::cerr << "Failed to load texture" << std::endl;
	}
  stbi_image_free(data);



	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x <<" ," << vec.y << " ," << vec.z << std::endl;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f),
		glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	// note that we’re translating the scene in the reverse direction
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
		100.0f);
	glm::vec3 cubePositions[] = {
glm::vec3(0.0f,
0.0f,
0.0f),
glm::vec3(2.0f,
5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f,
-2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f,
-3.5f),
glm::vec3(-1.7f,
3.0f,
-7.5f),
glm::vec3(1.3f, -2.0f,
-2.5f),
glm::vec3(1.5f,
2.0f,
-2.5f),
glm::vec3(1.5f,
0.2f,
-1.5f),
glm::vec3(-1.3f,
1.0f,
-1.5f)
	};

	


	shaderProgram.use();
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
				break;
			}
			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
					running = false;
				}
			}
		}
				
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT );

    float time = (float)SDL_GetTicks() / 1000.0f;
    float angle = time * glm::radians(50.0f);

		shaderProgram.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
		shaderProgram.setTexture("ourTexture", 0);
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
      float time = (float)SDL_GetTicks() / 1000.0f;
			float angle = time * 20.0f * i;
			model = glm::rotate(model, glm::radians(angle),
				glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		SDL_GL_SwapWindow(window);

	}
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}