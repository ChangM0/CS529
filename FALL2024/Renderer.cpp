#include "Renderer.h"
#include <stdexcept>

Renderer::Renderer(GameWindow& window) : pWindow(window) {
	initialize();
	setupCallbacks();
}

void Renderer::clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::swapBuffers() {
	glfwSwapBuffers(pWindow.getNativeWindow());
}

void Renderer::initialize() {
	pWindow.setWindowHints([]() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		});

	glfwMakeContextCurrent(pWindow.getNativeWindow());

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW");
}

void Renderer::setupCallbacks() {
	pWindow.setResizeCallback([this](GLFWwindow* window, int width, int height) {
		this->framebufferSizeCallback(window, width, height);
		});
}

void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}