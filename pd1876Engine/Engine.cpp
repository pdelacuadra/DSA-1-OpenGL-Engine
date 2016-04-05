#include "Engine.h"

// user input namespace
namespace {
	map<int, bool> keyIsDown;
	map<int, bool> keyWasDown;

	void mouseClick(GLFWwindow* windpwPtr, int button, int action, int mods)
	{
		keyIsDown[button] = action;
	}

	void keyCallback(GLFWwindow* windpwPtr, int key, int scancode, int action, int mods)
	{
		keyIsDown[key] = action;
	}
}

struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
};

Engine::Engine()
{
}


Engine::~Engine()
{
}

// -------------------------
// Initialize window library
// -------------------------
bool Engine::init()
{
	try
	{
		if (glfwInit() == GL_FALSE)
			return false;

		// create a windowed mode window
		GLFWwindowPtr = glfwCreateWindow(800, 600, "pd1876 DSA1 Engine", NULL, NULL);

		// make it the currently active window or quit
		if (GLFWwindowPtr != nullptr)
			glfwMakeContextCurrent(GLFWwindowPtr);
		else {
			glfwTerminate();
			return false;
		}

		// initialize glew or quit
		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return false;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

// ---------------------
// Processing Model Data - buffers
// ---------------------
bool Engine::bufferModel()
{
	// define model vertex locations
	vector<glm::vec3> locs =
	{
		{1, 1, 0},
		{-1, 1, 0},
		{-1, -1, 0},
		{1, -1, 0}
	};
	// location indices
	vector<unsigned int> locInds = { 0, 1, 2, 0, 2, 3 };
	vertCount = locInds.size();

	// create uvs
	vector<glm::vec2> uvs =
	{
		{1, 1},
		{0, 1},
		{0, 0},
		{1, 0}
	};
	// create uv indices
	vector<unsigned int> uvInds = { 0, 1, 2, 0, 2, 3 };

	// duplicate location and uv vertices into a single buffer
	vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
		vertBufData[i] = { locs[locInds[i]], uvs[uvInds[i]] };

	// generate buffer
	GLuint vertBuf;
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	// bind buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	// storing data in the buffer
	glBufferData(GL_ARRAY_BUFFER, // where to copy to
		sizeof(Vertex) * vertCount, // # of bytes to copy
		&vertBufData[0],		  // where to copy from
		GL_STATIC_DRAW);		  // "hint" to OpenGL

								  // enable the attribute
	glEnableVertexAttribArray(0);

	// set up the attribute
	glVertexAttribPointer(
		0,					// attribute index - 0 in this case
		3,					// number of components
		GL_FLOAT,			// type of data
		GL_FALSE,			// should we normalize data?
		sizeof(Vertex),	// Stride (bytes per vertex)
		0);					// offset to this attribute

	// set up second attribute
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3)
		);

	// change window color
	glClearColor(.057f, .146f, .675f, 1.0f);

	return true;
}

// ---------
// GAME LOOP
// ---------
bool Engine::gameLoop()
{
	// bind vertex array
	glBindVertexArray(0);

	// load all textures
	// hard coded size of four since I only have four textures
	// create four files
	const char* file1 = "textures/TestTexture.png";
	const char* file2 = "textures/lolYasVRiv.png";
	const char* file3 = "textures/projLeona.png";
	const char* file4 = "textures/wukong.png";
	const GLuint texID[] = { loadTexture(file1), loadTexture(file2), loadTexture(file3), loadTexture(file4) };

	// keeps track of which texture to use
	int texIndex = 0;

	// input
	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);

	// loop until user closes window
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		keyWasDown = keyIsDown;

		// Process queued window, mouse & keyboard callback events
		glfwPollEvents();

		// update physical simulation
		// draw buffered models
		// process input/window events

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// render game objects
		glBindVertexArray(vertArr);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);
		// glBindVertexArray(0);
		// unbind after drawing object

		// process input events
		if (keyIsDown[GLFW_KEY_ESCAPE] && keyWasDown[GLFW_KEY_ESCAPE])
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		}
		if (keyIsDown[GLFW_MOUSE_BUTTON_1] && !keyWasDown[GLFW_MOUSE_BUTTON_1])
		{
			// change texture based on input
			// increment texture index
			texIndex++;
			// make sure not to leave array bounds
			if (texIndex >= 4)
			{
				// resets back to zero
				texIndex = 0;
			}
		}

		// bind texture
		glBindTexture(GL_TEXTURE_2D, texID[texIndex]);

		glBindVertexArray(0);

		// swap the front (what the screen displays) and back (what OpenGL draws to) buffers
		glfwSwapBuffers(GLFWwindowPtr);
	}

	// delete all textures
	for (int i = 0; i < 4; i++)
	{
		cout << "Texture " << i << " Deleted" << endl;

		glDeleteTextures(1, &texID[i]);
	}

	glfwTerminate();
	return true;
}

bool Engine::useShaders()
{
	if (sm.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl"))
	{
		glUseProgram(sm.getProgram());
		return true;
	}
	else
	{
		return false;
	}
}

// load and use textures
GLuint Engine::loadTexture(const char* fileName)
{
	// load texture
	GLuint localTexID;
	FIBITMAP* tex = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName);
	// end if texture holds nothing
	if (tex == nullptr) return false;

	// convert textures to 32bit bmp
	FIBITMAP* tex32Bit = FreeImage_ConvertTo32Bits(tex);

	// unload old image from memory
	FreeImage_Unload(tex);
	glGenTextures(1, &localTexID);
	glBindTexture(GL_TEXTURE_2D, localTexID);

	// upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(tex32Bit),
		FreeImage_GetHeight(tex32Bit), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(tex32Bit));

	// set min filter instead of minmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return localTexID;
}