#include <glad/glad.h>
#include <stdio.h>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#define get_current_dir _getcwd

#define WIDTH 800
#define HEIGHT 600


float vertices[] = {
	-0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
};
unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
};

const char* vectexShaderSource = "shaders/vertx.vert";
const char* fragmentShaderSource = "shaders/fragment.frag";

enum ShaderType {
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	PROGRAM
};

std::string readShaderSource(const char* filePath) {
	std::ifstream shaderFile(filePath);
	if (!shaderFile.is_open()) {
		std::cerr << "Failed to open shader file: " << filePath << std::endl;
		return "";
	}
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	return shaderStream.str();
}

void GetShaderCompileErrors(unsigned int shader, ShaderType type) {
	int success;
	char infoLog[512];
	switch (type) {
	case VERTEX_SHADER:
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
		}
		break;
	case FRAGMENT_SHADER:
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
		}
		break;
	case PROGRAM:
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "Shader program linking failed: " << infoLog << std::endl;
		}
		break;

	}
}

void SetShader(unsigned int& shader, std::string shaderSourceFile, ShaderType type) {
	std::string shaderSource = readShaderSource(shaderSourceFile.c_str());
	const char* string = shaderSource.c_str();
	
	switch (type) {
		case VERTEX_SHADER:
			shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(shader, 1, &string, NULL);
			glCompileShader(shader);
			break;
		case FRAGMENT_SHADER:
			shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(shader, 1, &string, NULL);
			glCompileShader(shader);
			break;
	}
	GetShaderCompileErrors(shader, type);
}




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
	SDL_Renderer* renderer;

	SDL_CreateWindowAndRenderer(
		"[glad] GL with SDL3",
		WIDTH, HEIGHT,
		SDL_WINDOW_OPENGL,
		&window, &renderer
	);

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


	unsigned int vertexShader;
	SetShader(vertexShader, vectexShaderSource, VERTEX_SHADER);
	unsigned int fragmentShader;
	SetShader(fragmentShader, fragmentShaderSource, FRAGMENT_SHADER);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GetShaderCompileErrors(shaderProgram, PROGRAM);

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

	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),static_cast<void *>(0));
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram);

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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6 ,GL_UNSIGNED_INT,0);

		SDL_GL_SwapWindow(window);

	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}