#include "Camera.h"



Camera::Camera()
{
	// mouse sensitivity
	sensitivity = .0005;

	// persp cam matrix
	zoom = 1.f;
	width = 1600;
	height = 800;

	fieldOfView = 3.14159f * .4f / zoom;
	aspectRatio = (float)width / (float)height;
	nearZ = .01f;
	farZ = 1000.f;

	perspective = glm::perspective(fieldOfView, aspectRatio, nearZ, farZ);

	// create camera matrices
	camLoc = { 0, 0, 2 };
	glm::vec3 camRot = { 0, 0, 0 };

	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);

	camMatrix = perspective * lookAtMat;
}


Camera::~Camera()
{
}

void Camera::update(GLFWwindow* win, map<int, bool> keyIsDown, map<int, bool> keyWasDown)
{
	// calculate camera movement
	// takes window pointer and mouse x and y
	glfwGetCursorPos(win, &x, &y);

	camRot.y -= sensitivity * (x - width * .5f); // Yaw
	camRot.x -= sensitivity * (y - height * .5f); // Pitch
	camRot.x = glm::clamp(camRot.x, -.5f * glm::pi<float>(), .5f * glm::pi<float>());

	glfwSetCursorPos(win, width * .5f, height * .5f);

	// FPS controls for camera
	// move camera with WASDZX
	// rotate with mouse
	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	if (keyIsDown[GLFW_KEY_A])
		camVel += R * glm::vec3(-1, 0, 0);
	if (keyIsDown[GLFW_KEY_D])
		camVel += R * glm::vec3(1, 0, 0);
	if (keyIsDown[GLFW_KEY_W])
		camVel += R * glm::vec3(0, 1, 1);
	if (keyIsDown[GLFW_KEY_S])
		camVel += R * glm::vec3(0, -1, -1);
	if (keyIsDown[GLFW_KEY_Z])
		camVel += R * glm::vec3(0, 0, 1);
	if (keyIsDown[GLFW_KEY_X])
		camVel += R * glm::vec3(0, 0, -1);

	if (!keyIsDown[GLFW_KEY_A] && !keyIsDown[GLFW_KEY_D])
		camVel.x = 0;
	if (!keyIsDown[GLFW_KEY_W] && !keyIsDown[GLFW_KEY_S])
		camVel.y = 0;
	if (!keyIsDown[GLFW_KEY_Z] && !keyIsDown[GLFW_KEY_X])
		camVel.z = 0;


	float speed = .01f;
	if (camVel != glm::vec3())
		camVel = glm::normalize(camVel) * speed;

	camLoc += camVel;

	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	
	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);
	
	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);
	
	camMatrix = perspective * lookAtMat;

	// upload matrix
	glUniformMatrix4fv(3, 1, GL_FALSE, &camMatrix[0][0]);

	// hide the cursor
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
