#pragma once

// Standard C++ libraries
#include <iostream>
#include <vector>
using namespace std;

// Helper graphic libraries
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "graphics.h"
#include "shapes.h"

// MAIN FUNCTIONS
void startup();
void updateCamera();
void updateSceneElements();
void renderScene();

// CALLBACK FUNCTIONS
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);

// VARIABLES
bool        quit = false;
float       deltaTime = 0.0f;    // Keep track of time per frame.
float       lastTime = 0.0f;    // variable to keep overall time.
bool        keyStatus[1024];    // Hold key status.
bool		mouseEnabled = true; // keep track of mouse toggle.

// MAIN GRAPHICS OBJECT
Graphics    myGraphics;        // Runing all the graphics in this object

// Some global variable to do the animation.
float time_step = 0.001f;            // Global variable for animation

// ### --- ###			Added Files, Includes, Variables			### --- ###
#include "Shader.h"
#include "Player_Character.h"
#include "GameObject.h"
#include "AABB_Tree.h"

using namespace _GameObject;
using namespace _Player_Character;

bool firstMouse = true;
int lastX = 0;
int lastY = 0;

Shader* shader;
Player_Character* player;
// std::vector<GameObject> objects;

// Objects and Physics
bool collision = false;
std::vector<std::shared_ptr<GameObject>> game_objects;
AABB_Tree boundary_volume(10);
std::map<std::shared_ptr<I_AABB>, std::shared_ptr<GameObject>> object_map;



/*
			Archived Code - Can possibly be deleted
*/

// DEMO OBJECTS
Cube        myCube;
Sphere      mySphere;
Arrow       arrowX;
Arrow       arrowY;
Arrow       arrowZ;
Cube        myFloor;
Line        myLine;
Cylinder    myCylinder;