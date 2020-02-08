#define STB_IMAGE_IMPLEMENTATION // Must be done before using stb_image.h

#include "Main.h"

int main() {
	int errorGraphics = myGraphics.Init();
	if(errorGraphics) { return 0; }

	startup();

	while(!quit) {

		// Process Camera
		updateCamera();



		std::vector<std::shared_ptr<I_AABB>> collisions = boundary_volume.check_overlaps(game_objects[0]);
		if(!collisions.empty()) {
			collision = true;

			for(std::shared_ptr<I_AABB> collide : collisions) {
				if(object_map[collide] == game_objects[1]) {
					std::wcout << "Collision: " << (*object_map[collide]).get_id() << std::endl;
				}
			}
		}

		// Update Scene
		for(unsigned i = 0; i < game_objects.size(); i++) {
			if((*game_objects[i]).get_id() == "left") {
				if(collision) {
					(*game_objects[i]).translate(glm::vec3(-0.01, 0, 0));
				} else {
					(*game_objects[i]).translate(glm::vec3(0.01, 0, 0));
				}


				if((*game_objects[i]).get_position().x < -2) { collision = false; }
			}
		}







		updateSceneElements();

		// Render a still frame into an off-screen frame buffer known as the backbuffer.
		renderScene();

		// Swap the back buffer with the front buffer, making the most recently rendered image visible on-screen.
		glfwSwapBuffers(myGraphics.window);
	}

	myGraphics.endProgram();

	std::cout << "Press return to exit" << std::endl;
	std::cin.ignore();
	return 0;
}

void startup() {
	// Timing
	GLfloat currentTime = (GLfloat) glfwGetTime();
	deltaTime = currentTime;
	lastTime = currentTime;

	// Callbacks
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);
	glfwSetMouseButtonCallback(myGraphics.window, onMouseButtonCallback);
	glfwSetCursorPosCallback(myGraphics.window, onMouseMoveCallback);
	glfwSetScrollCallback(myGraphics.window, onMouseWheelCallback);

	// Shader
	shader = new Shader("shader_vertex.vs", "shader_fragment.fs");

	// Player Character / Camera
	player = new Player_Character();
	player->set_position(glm::vec3(0, 0, 10));

	// Initial Game Objects
	srand(glfwGetTime());
	GameObject left = *new GameObject("left", "resources/graphics_objects/lamp_standing.obj", glm::vec3(-5, 0, 0));
	GameObject right = *new GameObject("right", "resources/graphics_objects/lamp_standing.obj", glm::vec3(1, 0, 0));


	game_objects.push_back(std::make_shared<GameObject>(left));
	game_objects.push_back(std::make_shared<GameObject>(right));

	for(unsigned i = 0; i < game_objects.size(); i++) {
		boundary_volume.insert(game_objects[i]);
		object_map[game_objects[i]] = game_objects[i];
	}

	// Cull and depth testing
	myGraphics.SetOptimisations();

}

void updateCamera() {
	// Player is the camera
	player->process_keyboard(keyStatus);
}

void updateSceneElements() {
	glfwPollEvents();

	// Timing
	GLfloat currentTime = (GLfloat) glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// Set background colour and clear buffers
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();

	myGraphics.proj_matrix = glm::perspective(glm::radians(45.0f), (float) (myGraphics.WINDOW_WIDTH / myGraphics.WINDOW_HEIGHT), 0.1f, 100.0f);
	shader->setMat4("projection", myGraphics.proj_matrix);
	shader->setMat4("view", player->look_at());

	time_step += 0.01f; // increment movement variable

	if(glfwWindowShouldClose(myGraphics.window) == GL_TRUE) { quit = true; } // If quit by pressing x on window.
}

void renderScene() {
	myGraphics.ClearViewport();

	for(unsigned i = 0; i < game_objects.size(); i++) {

		//g.scale(glm::vec3(0.5, 0.5, 0.5));
		//g.rotate((int) (currentFrame * 100), glm::vec3(0, 1, 0));
		shader->setMat4("model", (*game_objects[i]).get_matrix());

		// g.randomize_mesh_vertices();
		(*game_objects[i]).Draw(*shader, true);
	}
}

void onResizeCallback(GLFWwindow* window, int w, int h) {
	glfwGetFramebufferSize(window, &myGraphics.WINDOW_WIDTH, &myGraphics.WINDOW_HEIGHT);
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), (float) (myGraphics.WINDOW_WIDTH / myGraphics.WINDOW_HEIGHT), 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	if(action == GLFW_PRESS) { keyStatus[key] = true; }
	if(action == GLFW_RELEASE) { keyStatus[key] = false; }
	if(keyStatus[GLFW_KEY_ESCAPE]) { glfwSetWindowShouldClose(window, true); }

	if(keyStatus[GLFW_KEY_M]) {
		// mouseEnabled = !mouseEnabled;
		myGraphics.ToggleMouse(mouseEnabled);
	}


}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	if(firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	player->process_mouse_movement(xpos - lastX, lastY - ypos);
	lastX = xpos;
	lastY = ypos;
}

void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}
