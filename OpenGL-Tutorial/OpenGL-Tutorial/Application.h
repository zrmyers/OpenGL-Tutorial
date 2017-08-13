#pragma once
//includes for graphics libraries
#include <SDL.h>
#define GLEW_STATIC
#include <GL\glew.h>
#include <SDL_opengl.h>

//includes for data types
#include <string>

class Application
{
public:
	Application(std::string appName = "OpenGL App");
	~Application();

	void Run();

private:
	std::string appName;

private:
	//SDL
	SDL_Window* window;
	SDL_GLContext context;

private:
	//OpenGL
	GLuint vao;
	GLuint vbo;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLint  posAttrib;
};

