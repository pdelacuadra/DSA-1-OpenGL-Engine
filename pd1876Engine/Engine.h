#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include "ShaderManager.h"
#include <vector>
#include <map>

using namespace std;

class Engine
{
public:
	Engine();
	~Engine();

	// method declarations
	bool init();
	bool bufferModel();
	bool gameLoop();
	bool useShaders();
	GLuint loadTexture(const char* fileName);

private:
	unsigned int vertCount;
	GLuint vertArr;
	GLFWwindow* GLFWwindowPtr;
	ShaderManager sm;
};

