#pragma once
#include "Camera.h"
#include <sstream>

struct VertInd
{
	unsigned int locInd, uvInd, normInd;
};

class Model
{
public:
	Model();
	~Model();
	bool buffer(string objFile);
	void render();

private:
	unsigned int vertCount;
	GLuint vertArr;
};

