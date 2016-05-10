#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <FreeImage.h>
#include "ShaderManager.h"
#include <time.h>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

// structs
struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Transform
{
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 size;
	glm::mat4 matrix;
};

struct RigidBody
{
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
};

struct Object
{
	Transform transform;
	RigidBody rb;
	string fileName;

	// enumeration for collision detection
	enum CollideType { none, AABB, sphere } collideType;

	// function to update object
	void update(float dt);
};

class Camera
{
public:
	Camera();
	~Camera();

	void update(GLFWwindow* win, map<int, bool> keyIsDown, map<int, bool> keyWasDown);

	glm::mat4 camMatrix;

private:
	// camera matrix
	glm::mat4 perspective;
	glm::vec3 camRot, camVel, camLoc;
	float fieldOfView, aspectRatio;
	float nearZ, farZ;
	Transform trans;
	RigidBody rigid;

	// camera controls variables
	float sensitivity;
	float zoom;

	// window width and height
	int width, height;

	// mouse x and y
	double x, y;
};

