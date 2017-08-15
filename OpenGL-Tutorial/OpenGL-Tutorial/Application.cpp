#include "Application.h"
#include <iostream>
#include <chrono>

#include "Shader.h"
#include "Mesh.h"

using namespace std;

Application::Application(string appName) : 
	appName(appName)
{
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow(appName.c_str(), 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// initialize OpenGL
	context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();

	// create the vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vector<gl_buffer> buffers = Mesh::getInstance("meshes/triangle.mesh");

	// create shaders
	vertexShader = Shader::getInstance("shaders/simple.vert");
	fragmentShader = Shader::getInstance("shaders/simple.frag");	

	//combine the vertex and fragment shaders into a program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// make link between vertex data and attributes
	colAttrib = glGetAttribLocation(shaderProgram, "color");	
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	posAttrib = glGetAttribLocation(shaderProgram, "position");	
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);
}

Application::~Application()
{
	// destroy OpenGL	
	Shader::destroyInstance(vertexShader);
	Shader::destroyInstance(fragmentShader);
	glDeleteProgram(shaderProgram);
	Mesh::destroyInstance("meshes/triangle.mesh");
	glDeleteVertexArrays(1, &vao);

	// destroy SDL
	SDL_GL_DeleteContext(context);	
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::Run() {
	auto t_start = chrono::high_resolution_clock::now();

	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT || (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)) {
				break;
			}
		}

		glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}
}
