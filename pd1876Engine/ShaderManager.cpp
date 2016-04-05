#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	program = 0;
}

ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::getProgram() const
{
	return program;
}

// private helper method
GLuint ShaderManager::loadShader(const char* file, GLenum shaderType)
{
	// create ifstream object
	ifstream inFile(file, ios::binary); // opens file in binary
	char* fileCont;
	if (inFile.is_open()) // checks if file is open
	{
		// seek and get length
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();
		inFile.seekg(0, ios::beg); // seek back to begining

		// create array to hold file
		fileCont = new char[length + 1];

		// read file and set null pointer
		inFile.read(fileCont, length);
		fileCont[length] = '\0';
		inFile.close();
	}
	else // error opening file
	{
		// print error message and return zero
		cout << "Error opening file" << endl;
		return 0;
	}

	// call OpenGL functions
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &fileCont, 0);
	glCompileShader(shader);
	delete[] fileCont;

	try
	{
		GLint shaderTest;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderTest);

		cout << "Shader Compile Works!" << endl;

		return shader;
	}
	catch(...)
	{
		cout << "Shader Compile Failure!" << endl;

		// get the log
		GLint debugShader;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &debugShader);

		// set up GLchar array
		GLchar* logArr = new GLchar[debugShader];

		// get the log
		glGetShaderInfoLog(shader, debugShader, 0, logArr);
		
		// print results
		for (int i = 0; i < debugShader; i++)
		{
			cout << logArr[i];
		}

		// delete and return
		glDeleteShader(shader);
		delete[] logArr;
		return 0;
	}
}

// public method to load shaders
bool ShaderManager::loadShaders(const char* vertexFile, const char* fragmentFile)
{
	GLuint vertShader;
	GLuint fragShader;

	// vertex shader loading
	try
	{
		vertShader = loadShader(vertexFile, GL_VERTEX_SHADER);
	}
	catch (...)
	{
		cout << "Error Loading Vertex Shader" << endl;
		return 0;
	}
	// fragment shader loading
	try
	{
		fragShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	}
	catch (...)
	{
		cout << "Error Loading Fragment Shader" << endl;
		return 0;
	}

	// create shader program
	program = glCreateProgram();

	// attach shaders
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	// link the program
	glLinkProgram(program);

	// check the program link
	try
	{
		GLint debugProg;
		glGetProgramiv(program, GL_LINK_STATUS, &debugProg);
		cout << "Program Link Works" << endl;

		return true;
	}
	catch (...)
	{
		cout << "Error With Program Link" << endl;

		// get the log
		GLint progLog;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &progLog);

		// make array
		GLchar* progArr = new GLchar[progLog];

		// get log
		glGetProgramInfoLog(program, progLog, 0, progArr);

		// print results
		for (int i = 0; i < progLog; i++)
		{
			cout << progArr[i];
		}

		// delete and return
		glDeleteProgram(program);
		delete[] progArr;
		return false;
	}
}