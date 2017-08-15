#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Application.h"

using namespace std;


class Mesh
{
private:
	//instance of shader objects will be managed by the shader class
	static unordered_map<string, Mesh> instances;

	//private constructor and destructor
	void load(string filename);
public:
	//static class based methods
	static vector<gl_buffer> getInstance(string filename);
	static void destroyInstance(string filename);

	Mesh();
	~Mesh();

private:
	// Shader instance level data
	string inputFileName = "";
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	vector<gl_buffer> buffers;
};

