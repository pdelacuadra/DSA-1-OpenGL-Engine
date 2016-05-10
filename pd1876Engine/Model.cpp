#include "Model.h"



Model::Model()
{
	vertCount = 0;
	vertArr = 0;
}


Model::~Model()
{
}

bool Model::buffer(string objFile)
{
	// declare four vector collections
	vector<glm::vec3> locations;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<VertInd> vertInds;

	// open the file for reading
	ifstream inFile;
	inFile.open(objFile);

	// input string using stringstream to parse file
	string input;
	
	// if file opened right
	if (inFile.is_open())
	{
		// loop through file till empty
		while (getline(inFile, input))
		{
			istringstream ss(input);
			string lineLabel;

			ss >> lineLabel;

			if (lineLabel == "v") // LOCATIONS
			{
				float a, b, c;
				ss >> a >> b >> c;
				locations.push_back(glm::vec3(a, b, c));
			}
			else if (lineLabel == "vt") // UVS
			{
				float a, b;
				ss >> a >> b;
				uvs.push_back(glm::vec2(a, b));
			}
			else if (lineLabel == "vn") // NORMALS
			{
				float a, b, c;
				ss >> a >> b >> c;
				normals.push_back(glm::vec3(a, b, c));
			}
			else if (lineLabel == "f") // INDICES
			{
				// do three times
				for (int i = 0; i < 3; i++)
				{
					unsigned int a, b, c;
					char s1, s2;
					// write int then slash...
					ss >> a >> s1 >> b >> s2 >> c;
					// decrement each int by one and add to vertInds
					vertInds.push_back(VertInd{ a - 1, b - 1, c - 1 });
				}
			}
		}
		// close the file
		inFile.close();
	}

	vertCount = vertInds.size();

	// duplicate location and uv vertices into a single buffer
	vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
		vertBufData[i] = { locations[vertInds[i].locInd],
							uvs[vertInds[i].uvInd],
							normals[vertInds[i].normInd] };

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
		(void*)sizeof(VertInd)
		);

	// change window color
	glClearColor(.057f, .146f, .675f, 1.0f);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
