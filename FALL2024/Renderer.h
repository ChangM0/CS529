#pragma once

#include <glew.h>
#include <glfw3.h>
#include "GameWindow.h"

class Renderer {
public:
	Renderer(GameWindow& window);

	void clear(float r, float g, float b, float a);
	void swapBuffers();

private:
	void initialize();
	void setupCallbacks();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	GameWindow& pWindow;
};