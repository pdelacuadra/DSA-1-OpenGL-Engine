#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	// getter
	GLuint getProgram() const;

	// public method to load shaders
	// set the value of program to 0 if it fails
	// set to something other than 0 if it succeeds
	bool loadShaders(const char* vertexFile, const char* fragmentFile);

private:
	// index where the loaded shaders are stored in the graphics card
	GLuint program;

	// private helper method used to load shaders
	// to read and compile a shader file
	// and return the index where it is stored
	GLuint loadShader(const char* file, GLenum shaderType);
};

