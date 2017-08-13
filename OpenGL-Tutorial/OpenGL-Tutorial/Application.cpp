#include "Application.h"
#include <iostream>

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

	// create vertex buffer
	float vertices[] = {
		 0.0f,  0.5f, // Vertex 1 (X, Y)
		 0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (x, Y)
	};

	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	// create the vertex shader
	const GLchar* vertexSource = R"glsl(
		#version 150 core

		in vec2 position;

		void main() {
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)glsl";

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexSource, NULL);
	glCompileShader(vertexShader);

	// make sure the shader compiled successfully
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		cout << "\n***ERROR: \n" << buffer << "\n"; 
	}

	// create the fragment shader
	const GLchar* fragmentSource = R"glsl(
		#version 150 core

		out vec4 outColor;

		void main() {
			outColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)glsl";
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// make sure the shader compiled successfully
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		cout << "\n***ERROR: \n" << buffer << "\n";
	}

	//combine the vertex and fragment shaders into a program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);	

	// make link between vertex data and attributes
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);
	
}


Application::~Application()
{
	// destroy OpenGL
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	// destroy SDL
	SDL_GL_DeleteContext(context);	
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::Run() {
	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT || (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)) {
				break;
			}
		}

		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}
}
