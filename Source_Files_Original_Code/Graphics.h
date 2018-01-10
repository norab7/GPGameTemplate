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


	GLFWwindow*		window;
	int				windowWidth = 640;
	int				windowHeight = 480;
	float           aspect;
	glm::mat4		proj_matrix = glm::mat4(1.0f);

};
