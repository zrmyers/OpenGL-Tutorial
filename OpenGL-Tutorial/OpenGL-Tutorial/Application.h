#pragma once
//includes for graphics libraries
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

//includes for data types
#include <string>

typedef struct gl_buffer {
	GLuint bObject;
	GLenum bType;
} gl_buffer;

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
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLint  posAttrib;
	GLint colAttrib;
};

