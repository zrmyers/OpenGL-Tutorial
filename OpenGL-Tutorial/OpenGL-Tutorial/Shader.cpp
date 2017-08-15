#include "Shader.h"
#include <fstream>
#include <iostream>

unordered_map<string, Shader> Shader::instances;
unordered_map<GLuint, string> Shader::shaderMap;

Shader::Shader() : 
	shaderFileName(""), 
	shaderSource(""), 
	shader(0) {
}

Shader::~Shader()
{
	//destroy
	if(shader != 0) glDeleteShader(shader);
}

GLuint Shader::getInstance(string shaderFileName) {
	Shader* instance;
	try {
		// if the shader file has been loaded
		instance = &instances.at(shaderFileName);
	}
	catch (std::out_of_range e) {
		// otherwise, create a new instance of shader object, and place it in the map
		instance = &instances[shaderFileName];
		instance->load(shaderFileName);
	}
	return instance->shader;
}

void Shader::destroyInstance(string shaderFileName) {
	try {
		// if the shader file has been loaded
		Shader* instance = &instances.at(shaderFileName);
		instances.erase(shaderFileName);		
	}
	catch (std::out_of_range e) {
		cout << "***Error: attempting to delete shader which doesn't exist!\n";
		cout << e.what() << "\n";
	}
}

void Shader::destroyInstance(GLuint shaderID) {
	try
	{
		string shaderfilename = shaderMap.at(shaderID);
		destroyInstance(shaderfilename);
		shaderMap.erase(shaderID);
	}
	catch (std::out_of_range e)
	{
		cout << "***Error: attempting to delete shader which doesn't exist!\n";
		cout << e.what() << "\n";
	}
}

void Shader::load(string filename)
{
	// set the filename
	this->shaderFileName = filename;

	// load the source file
	ifstream shaderIFS(filename);
	string line;
	while (getline(shaderIFS, line)) {
		// add to source line by line
		this->shaderSource.append(line + "\n");		
	}

	const GLchar* source = this->shaderSource.c_str();

	if (filename.find(".vert") != string::npos) {
		// compile the source file
		shader = glCreateShader(GL_VERTEX_SHADER);
		shaderMap[shader] = filename;
	}
	else if (filename.find(".frag") != string::npos) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		shaderMap[shader] = filename;
	}
	else {
		cout << "***Error: unknown shader type for file: " << filename << "!\n";
	}
	
	glShaderSource(shader, 1, &source, NULL);
	
	this->compile();
}

void Shader::compile() {
	glCompileShader(shader);

	// make sure the shader compiled successfully
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		cout << "\n***ERROR: \n" << buffer << "\n";
	}
}