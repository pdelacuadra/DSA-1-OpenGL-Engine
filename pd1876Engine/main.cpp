// include headings
// using statements
#include "Engine.h"

int main()
{
	// engine object
	Engine engine;

	// call engine functions
	if (!engine.init()) return -1;
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (!engine.bufferModels()) return -1;

	if (engine.useShaders())
	{
		engine.gameLoop();
	}
	else
	{
		return -1;
	}
	return 0;
}