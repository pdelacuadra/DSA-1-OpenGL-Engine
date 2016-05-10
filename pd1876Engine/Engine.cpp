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

// create all objects to use in the constructor
Engine::Engine()
{
	// create array for textures
	texIDs = new GLuint[7];
	
	// create objects
	//<-----Sky----->
	Object sky;
	sky.fileName = texIDs[0];
	sky.transform.size = glm::vec3(1, 1, 1);
	sky.transform.location = glm::vec3(0, 0, 0);
	sky.transform.rotation = glm::vec3(0, 0, 0);
	sky.rb.velocity = glm::vec3(0, 0, 0);
	sky.rb.force = glm::vec3(0, 0, 0);
	sky.rb.mass = .5;
	sky.collideType = Object::none;

	objects.push_back(sky);

	//<-----Helicopter----->
	Object heli;
	heli.fileName = texIDs[1];
	heli.transform.size = glm::vec3(.1, .1, .1);
	heli.transform.location = glm::vec3(-.8, 0, 1);
	heli.transform.rotation = glm::vec3(0, 0, 0);
	heli.rb.velocity = glm::vec3(0, 0, 0);
	heli.rb.force = glm::vec3(0, 0, 0);
	heli.rb.mass = .5;
	heli.collideType = Object::AABB;
	
	objects.push_back(heli);

	//<-----Projectile----->
	Object projectile;
	projectile.fileName = texIDs[2];
	projectile.transform.size = glm::vec3(.03, .03, .03);
	projectile.transform.location = glm::vec3(1, 0, 1);
	projectile.transform.rotation = glm::vec3(0, 0, 0);
	projectile.rb.velocity = glm::vec3(0, 0, 0);
	projectile.rb.force = glm::vec3(0, 0, 0);
	projectile.rb.mass = .5;
	projectile.collideType = Object::AABB;

	objects.push_back(projectile);

	//<-----Projectile----->
	Object projectile2;
	projectile2.fileName = texIDs[3];
	projectile2.transform.size = glm::vec3(.03, .03, .03);
	projectile2.transform.location = glm::vec3(2, 0, 1);
	projectile2.transform.rotation = glm::vec3(0, 0, 0);
	projectile2.rb.velocity = glm::vec3(0, 0, 0);
	projectile2.rb.force = glm::vec3(0, 0, 0);
	projectile2.rb.mass = .5;
	projectile2.collideType = Object::AABB;

	objects.push_back(projectile2);

	//<-----Projectile----->
	Object projectile3;
	projectile3.fileName = texIDs[4];
	projectile3.transform.size = glm::vec3(.03, .03, .03);
	projectile3.transform.location = glm::vec3(3, 0, 1);
	projectile3.transform.rotation = glm::vec3(0, 0, 0);
	projectile3.rb.velocity = glm::vec3(0, 0, 0);
	projectile3.rb.force = glm::vec3(0, 0, 0);
	projectile3.rb.mass = .5;
	projectile3.collideType = Object::AABB;
	
	objects.push_back(projectile3);

	//<-----Boss----->
	Object boss;
	boss.fileName = texIDs[5];
	boss.transform.size = glm::vec3(.1, .1, .1);
	boss.transform.location = glm::vec3(2, 0, 1);
	boss.transform.rotation = glm::vec3(0, 0, 0);
	boss.rb.velocity = glm::vec3(0, 0, 0);
	boss.rb.force = glm::vec3(0, 0, 0);
	boss.rb.mass = .5;
	boss.collideType = Object::AABB;

	objects.push_back(boss);

	//<-----Bullet----->
	Object bullet;
	bullet.fileName = texIDs[6];
	bullet.transform.size = glm::vec3(.01, .01, .01);
	bullet.transform.location = glm::vec3(-2, 0, 1);
	bullet.transform.rotation = glm::vec3(0, 0, 0);
	bullet.rb.velocity = glm::vec3(0, 0, 0);
	bullet.rb.force = glm::vec3(0, 0, 0);
	bullet.rb.mass = .5;
	bullet.collideType = Object::AABB;

	objects.push_back(bullet);
}


Engine::~Engine()
{
	for (int i = 0; i < 7; i++)
		glDeleteTextures(1, &texIDs[i]);
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
		GLFWwindowPtr = glfwCreateWindow(1600, 800, "pd1876 DSA1 Engine", NULL, NULL);

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

		// initialize camera
		cam = Camera();

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
bool Engine::bufferModels()
{
	if (!model.buffer("models/quad.obj"))
		return false;
	else
		return true;
}

// ---------
// GAME LOOP
// ---------
bool Engine::gameLoop()
{
	// load textures
	GLuint heli = loadTexture("textures/heli.png");
	GLuint projectile = loadTexture("textures/projectile.png");
	GLuint boss = loadTexture("textures/enemy.png");
	GLuint sky = loadTexture("textures/sky.png");
	GLuint projectile2 = loadTexture("textures/projectile2.png");
	GLuint projectile3 = loadTexture("textures/projectile3.png");
	GLuint bullet = loadTexture("textures/bullet.png");
	
	// populate texture array
	texIDs[0] = sky;
	texIDs[1] = heli;
	texIDs[2] = projectile;
	texIDs[3] = projectile2;
	texIDs[4] = projectile3;
	texIDs[5] = boss;
	texIDs[6] = bullet;

	// input
	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);

	// variables for shooting
	bool canShoot = true;

	// loop until user closes window
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		// create random number
		srand((unsigned int)time(0));

		float rando = -.5 + (float(rand()) / float(RAND_MAX));

		// initialize value for game time
		float time = glfwGetTime();

		// update time values
		prevFrameTime = currentTime;
		currentTime = time;
		deltaTime = currentTime - prevFrameTime;

		// update which key was down
		keyWasDown = keyIsDown;

		// Process queued window, mouse & keyboard callback events
		glfwPollEvents();

		// update physical simulation
		// draw buffered models
		// process input/window events

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// render game objects
		for (int i = 0; i < 7; i++)
		{
			Transform trans = objects[i].transform;
			objects[i].transform.matrix = translate(trans.location) * glm::yawPitchRoll(trans.rotation.x, trans.rotation.y, trans.rotation.z) * scale(trans.size);
		}
			
		for (int i = 0; i < 7; i++)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texIDs[i]);
			glUniformMatrix4fv(2, 1, GL_FALSE, &objects[i].transform.matrix[0][0]);

			// update camera
			cam.update(GLFWwindowPtr, keyIsDown, keyWasDown);

			model.render();
		}

		glBindVertexArray(0);

		// move projectile
		objects[2].rb.force = glm::vec3(-0.2, 0, 0);
		objects[3].rb.force = glm::vec3(-0.2, 0, 0);
		objects[4].rb.force = glm::vec3(-0.2, 0, 0);

		// limit all projectile velocity and reset position when necessary
		if (objects[2].transform.location.x <= -1)
			objects[2].transform.location = glm::vec3(objects[5].transform.location.x, objects[5].transform.location.y, objects[5].transform.location.z);
		if (objects[2].rb.velocity.x <= -.7)
			objects[2].rb.velocity.x = -.7;

		if (objects[3].transform.location.x <= -1)
			objects[3].transform.location = glm::vec3(objects[5].transform.location.x, objects[5].transform.location.y, objects[5].transform.location.z);
		if (objects[3].rb.velocity.x <= -.7)
			objects[3].rb.velocity.x = -.7;

		if (objects[4].transform.location.x <= -1)
			objects[4].transform.location = glm::vec3(objects[5].transform.location.x, objects[5].transform.location.y, objects[5].transform.location.z);
		if (objects[4].rb.velocity.x <= -.7)
			objects[4].rb.velocity.x = -.7;

		// stop moving when the boss gets to a certain point
		if (objects[5].transform.location.x >= 0.6)
		{
			objects[5].rb.force = glm::vec3(-0.03, .03, 0);

			if (objects[5].transform.location.y >= .7)
			{
				objects[5].rb.force.y = -.03;
			}
			if (objects[5].transform.location.y <= -.7)
			{
				objects[5].rb.force.y = .03;
			}
		}
		else
		{
			objects[5].rb.force = glm::vec3(0.03, -.03, 0);

			if (objects[5].transform.location.y >= .7)
			{
				objects[5].rb.force.y = -.03;
			}
			if (objects[5].transform.location.y <= -.7)
			{
				objects[5].rb.force.y = .03;
			}
		}

		// keep boss from leaving bounds of screen in y direction
		if (objects[5].transform.location.y >= .7)
			objects[5].transform.location.y = .7;
		if (objects[5].transform.location.y <= -.7)
			objects[5].transform.location.y = -.7;

		// limit boss velocity
		if (objects[5].rb.velocity.x <= -0.2)
			objects[5].rb.velocity.x = -0.2;
		if (objects[5].rb.velocity.x >= 0.2)
			objects[5].rb.velocity.x = 0.2;

		// process input events
		if (keyIsDown[GLFW_KEY_ESCAPE] && keyWasDown[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);

		// player controls
		if (keyIsDown[GLFW_KEY_UP])
			objects[1].rb.force += glm::vec3(0, 0.03, 0);

		if (keyIsDown[GLFW_KEY_DOWN])
			objects[1].rb.force += glm::vec3(0, -0.03, 0);

		if (keyIsDown[GLFW_KEY_UP] && keyIsDown[GLFW_KEY_DOWN])
			objects[1].rb.force += glm::vec3(0, 0, 0);

		if (keyIsDown[GLFW_KEY_RIGHT])
			objects[1].rb.force += glm::vec3(0.03, 0, 0);

		if (keyIsDown[GLFW_KEY_LEFT])
			objects[1].rb.force += glm::vec3(-0.03, 0, 0);

		if (keyIsDown[GLFW_KEY_UP] && keyIsDown[GLFW_KEY_LEFT ])
			objects[1].rb.force += glm::vec3(-0.03, 0.03, 0);

		else if (keyIsDown[GLFW_KEY_UP] && keyIsDown[GLFW_KEY_RIGHT])
			objects[1].rb.force += glm::vec3(0.03, 0.03, 0);

		if (keyIsDown[GLFW_KEY_DOWN] && keyIsDown[GLFW_KEY_LEFT])
			objects[1].rb.force += glm::vec3(-0.03, -0.03, 0);

		else if (keyIsDown[GLFW_KEY_DOWN] && keyIsDown[GLFW_KEY_RIGHT])
			objects[1].rb.force += glm::vec3(0.03, -0.03, 0);

		if (keyWasDown[GLFW_KEY_SPACE] && canShoot == true)
		{
			canShoot = false;
			objects[6].transform.location = objects[1].transform.location;
			objects[6].rb.force = glm::vec3(0.1, 0, 0);
		}

		// if bullet leaves the screen reset can shoot
		if (objects[6].transform.location.x >= 1)
			canShoot = true;

		// keep player heli from leaving the screen
		if (objects[1].transform.location.y >= .9)
			objects[1].transform.location.y = .9;
		if (objects[1].transform.location.y <= -.9)
			objects[1].transform.location.y = -.9;
		if (objects[1].transform.location.x <= -.9)
			objects[1].transform.location.x = -.9;

		// limit player heli velocity
		if (objects[1].rb.velocity.y <= -0.2)
			objects[1].rb.velocity.y = -0.2;
		if (objects[1].rb.velocity.y >= 0.2)
			objects[1].rb.velocity.y = 0.2;
		if (objects[1].rb.velocity.x <= -0.2)
			objects[1].rb.velocity.x = -0.2;
		if (objects[1].rb.velocity.x >= 0.2)
			objects[1].rb.velocity.x = 0.2;

		// limit bullet velocity
		if (objects[6].rb.velocity.x >= 1)
			objects[6].rb.velocity.x = 1;

		// loop through objects and update each one
		for (int i = 0; i < 7; i++)
			objects[i].update(deltaTime);

		// check if heli is colliding with projectile
		if (collides(&objects[1], &objects[2]) || collides(&objects[1], &objects[3]) || collides(&objects[1], &objects[4]))
			cout << "YOU HAVE DIED" << endl;

		// check if bullet is colliding with enemy
		if (collides(&objects[6], &objects[5]))
			cout << "Enemy has died" << endl;

		// swap the front (what the screen displays) and back (what OpenGL draws to) buffers
		glfwSwapBuffers(GLFWwindowPtr);
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

void Object::update(float dt)
{
	// get acceleration
	glm::vec3 acceleration = this->rb.force / this->rb.mass;
	
	// get velocity
	this->rb.velocity += (acceleration * dt);

	// get new position
	this->transform.location += (this->rb.velocity * dt);
}

bool Engine::collides(const Object* o1, const Object* o2)
{
	Object obj1 = *o1;
	Object obj2 = *o2;

	// no colliding - either one of the objects has no collider
	if (obj1.collideType <= Object::none || obj2.collideType <= Object::none)
		return false;

	// both colliders are spheres
	if ((obj1.collideType > Object::AABB && obj1.collideType <= Object::sphere) && (obj2.collideType > Object::AABB && obj2.collideType <= Object::sphere))
	{
		glm::vec3 distance = obj1.transform.location - obj2.transform.location;
		int sumRadii = obj1.transform.size.x + obj2.transform.size.x;

		if (distance.x > sumRadii && distance.y > sumRadii && distance.z > sumRadii)
			return false;
	}

	// if both colliders are AABB
	if ((obj1.collideType > Object::none && obj1.collideType <= Object::AABB) && (obj2.collideType > Object::none && obj2.collideType <= Object::AABB))
	{
		float xDist = abs(obj1.transform.location.x - obj2.transform.location.x);
		float yDist = abs(obj1.transform.location.y - obj2.transform.location.y);
		float zDist = abs(obj1.transform.location.z - obj2.transform.location.z);
		
		if (xDist > ((obj1.transform.size.x + obj2.transform.size.x))) return false;
		if (yDist > ((obj1.transform.size.y + obj2.transform.size.y))) return false;
		if (zDist > ((obj1.transform.size.z + obj2.transform.size.z))) return false;

		return true;
	}

	// if one is a sphere and one is AABB
	if ((obj1.collideType > Object::none && obj1.collideType <= Object::AABB) && (obj2.collideType > Object::AABB && obj2.collideType <= Object::sphere))
	{
		// find min and max values for AABB
		float maxX = obj1.transform.location.x + (obj1.transform.size.x / 2);
		float maxY = obj1.transform.location.y + (obj1.transform.size.y / 2);
		float maxZ = obj1.transform.location.z + (obj1.transform.size.z / 2);
		float minX = obj1.transform.location.x - (obj1.transform.size.x / 2);
		float minY = obj1.transform.location.y - (obj1.transform.size.y / 2);
		float minZ = obj1.transform.location.z - (obj1.transform.size.z / 2);

		// calculate max value between AABB and sphere
		float x = fmax(minX, fmin(obj2.transform.location.x, maxX));
		float y = fmax(minY, fmin(obj2.transform.location.y, maxY));
		float z = fmax(minZ, fmin(obj2.transform.location.z, maxZ));

		// calculate distance
		int dist = sqrt(pow((x - obj2.transform.location.x), 2) + pow((y - obj2.transform.location.y), 2) + pow((z - obj2.transform.location.z), 2));

		if (dist > obj2.transform.size.x)
			return false;
	}
	else if ((obj1.collideType > Object::AABB && obj1.collideType <= Object::sphere) && (obj2.collideType > Object::none && obj2.collideType <= Object::AABB))
	{
		// find min and max values for AABB
		float maxX = obj2.transform.location.x + (obj2.transform.size.x / 2);
		float maxY = obj2.transform.location.y + (obj2.transform.size.y / 2);
		float maxZ = obj2.transform.location.z + (obj2.transform.size.z / 2);
		float minX = obj2.transform.location.x - (obj2.transform.size.x / 2);
		float minY = obj2.transform.location.y - (obj2.transform.size.y / 2);
		float minZ = obj2.transform.location.z - (obj2.transform.size.z / 2);

		// calculate max value between AABB and sphere
		float x = fmax(minX, fmin(obj1.transform.location.x, maxX));
		float y = fmax(minY, fmin(obj1.transform.location.y, maxY));
		float z = fmax(minZ, fmin(obj1.transform.location.z, maxZ));

		// calculate distance
		int dist = sqrt(pow((x - obj1.transform.location.x), 2) + pow((y - obj1.transform.location.y), 2) + pow((z - obj1.transform.location.z), 2));

		if (dist > obj1.transform.size.x)
			return false;
	}

	// return true if all tests failed
	return true;
}