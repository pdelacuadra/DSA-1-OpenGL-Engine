#pragma once
#include "Camera.h"
#include "Model.h"

class Engine
{
public:
	Engine();
	~Engine();

	// method declarations
	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShaders();
	GLuint loadTexture(const char* fileName);
	bool collides(const Object* o1, const Object* o2);

private:
	GLFWwindow* GLFWwindowPtr;
	ShaderManager sm;
	GLuint* texIDs;
	vector<Object> objects;
	float currentTime;
	float prevFrameTime;
	float deltaTime;
	Camera cam;
	Model model;
};

