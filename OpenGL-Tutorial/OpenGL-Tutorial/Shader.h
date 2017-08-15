#pragma once
#include <string>
#include <unordered_map>

#include "Application.h"

using namespace std;

class Shader
{
private:
	//instance of shader objects will be managed by the shader class
	static unordered_map<string, Shader> instances;
	static unordered_map<GLuint,string> shaderMap;

	//private constructor and destructor
	void load(string filename);		
	void compile();
public:	
	//static class based methods
	static GLuint getInstance(string shaderFileName);
	static void destroyInstance(string shaderFileName);
	static void destroyInstance(GLuint shaderID);

	Shader();
	~Shader();

private:
	// Shader instance level data
	string shaderFileName = "";
	string shaderSource = "";
	
	GLuint shader = 0;
};

