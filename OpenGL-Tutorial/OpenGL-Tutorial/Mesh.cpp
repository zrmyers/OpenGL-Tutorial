#include "Mesh.h"
#include <fstream>
#include <iostream>

using namespace std;

unordered_map<string, Mesh> Mesh::instances;

Mesh::Mesh() : vertices(), indices(), buffers() {

}

Mesh::~Mesh()
{
	for (gl_buffer buffer : buffers) {
		glDeleteBuffers(1, &buffer.bObject);
	}
}

vector<gl_buffer> Mesh::getInstance(string filename) {
	Mesh* instance;
	try	{
		instance = &instances.at(filename);
	}
	catch (std::out_of_range e)	{
		instance = &instances[filename];
		instance->load(filename);
	}
	return instance->buffers;
}

void Mesh::destroyInstance(string filename) {
	try {
		Mesh* instance = &instances.at(filename);
		instances.erase(filename);
	}
	catch (std::out_of_range e)	{
		
	}
}

void Mesh::load(string filename) {
	this->inputFileName = filename;

	//parse the input file
	ifstream meshIFS(filename);
	string line;
	while (getline(meshIFS, line)) {
		if (line.find("V:") != string::npos) {
			line.erase(0,2);
			
			size_t position = 0;
			string token;
			while ((position = line.find(",")) != string::npos) {
				token = line.substr(0, position);
				vertices.push_back((GLfloat) stof(token));
				line.erase(0,position+1);
			}

			vertices.push_back(stof(line));
		}
		else if (line.find("I:") != string::npos) {
			line.erase(0,2);

			size_t position = 0;
			string token;
			while ((position = line.find(",")) != string::npos) {
				token = line.substr(0, position);
				unsigned long l = stoul(token);
				indices.push_back((GLuint) l);
				line.erase(0, position + 1);
			}

			indices.push_back(stoi(line));
		}
	}

	// build buffer objects
	if (vertices.size() > 0) {
		gl_buffer vbo;
		glGenBuffers(1, &vbo.bObject);
		vbo.bType = GL_ARRAY_BUFFER;

		// bind the buffer, and upload its data
		glBindBuffer(vbo.bType, vbo.bObject);
		glBufferData(vbo.bType, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}
		
	if (indices.size() > 0) {
		gl_buffer ebo;
		glGenBuffers(1, &ebo.bObject);
		ebo.bType = GL_ELEMENT_ARRAY_BUFFER;

		// bind the buffer, and upload its data
		glBindBuffer(ebo.bType, ebo.bObject);
		glBufferData(ebo.bType, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}

}