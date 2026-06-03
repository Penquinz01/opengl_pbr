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

#define WIDTH 800
#define HEIGHT 600


float vertices[] = {
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
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

	glVertexAttribPointer(0,4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);


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
				
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT );

		shaderProgram.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
		shaderProgram.setTexture("ourTexture", 0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6 ,GL_UNSIGNED_INT,0);

		SDL_GL_SwapWindow(window);

	}
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}