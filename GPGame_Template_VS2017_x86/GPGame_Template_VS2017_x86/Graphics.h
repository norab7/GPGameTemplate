#pragma once

#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



void ErrorCallbackGLFW(int error, const char* description);

class Graphics {
public:
	Graphics();
	~Graphics();

	int Init();
	void hintsGLFW();
	void SetupRender();
	void endProgram();
	void SetOptimisations();
	void ClearViewport();
	void ToggleMouse();


	GLFWwindow*		window;
	int				windowWidth = 640;
	int				windowHeight = 480;
	float           aspect;
	glm::mat4		viewMatrix = glm::mat4(1.0f); // View Matrix
	glm::mat4		proj_matrix = glm::mat4(1.0f);

	// canvas
	glm::vec4		backgroundColor = glm::vec4(25.0f / 200.0f, 135.0f / 255.0, 255.0f / 255.0f, 1.0f);

	// Camera
	bool			cameraFirstMouse = true;
	GLfloat			cameraYaw = 90.0f;	// init pointing to inside
	GLfloat			cameraPitch = 0.0f;	// start centered
	GLfloat			cameraLastX = (GLfloat)windowWidth / 2.0f;	// start middle screen
	GLfloat			cameraLastY = (GLfloat)windowHeight / 2.0f;	// start middle screen
	glm::vec3		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3		cameraPosition = glm::vec3(0.0f, 2.0f, -5.0f);
	glm::vec3		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 

	int				mouseX = windowWidth/2;
	int				mouseY = windowHeight/2;

	bool			showingMouse = false;	// keep track if showing mouse.
};
