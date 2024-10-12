#include "GameWindow.h"
#include <iostream>
#include <stdexcept>

GameWindow::GameWindow(int width, int height, const std::string& title)
	:width(width), height(height), title(title), pWindow(nullptr) {
	initialize();
}

GameWindow::~GameWindow() {
	shutdown();
}

bool GameWindow::shouldClose() const {
	return glfwWindowShouldClose(pWindow);
}

void GameWindow::pollEvents() {
	glfwPollEvents();
}

GLFWwindow* GameWindow::getNativeWindow() const{
	return pWindow;
}


void GameWindow::setWindowHints(const std::function<void()>& hintSetter) {
	hintSetter();
}

int GameWindow::getWidth() const{
	return width;
}

int GameWindow::getHeight() const {
	return height;
}

void GameWindow::setResizeCallback(std::function<void(GLFWwindow*, int, int)> callback) {
	resizeCallback = std::move(callback);
	glfwSetFramebufferSizeCallback(pWindow, resizeCallbackWrapper);
	glfwSetWindowUserPointer(pWindow, this);
}

void GameWindow::initialize() {
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");
	pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!pWindow)
		throw std::runtime_error("Failes to create GLFW window");
}

void GameWindow::shutdown() {
	if (pWindow)
		glfwDestroyWindow(pWindow);
	glfwTerminate();
}

void GameWindow::resizeCallbackWrapper(GLFWwindow* window, int width,int height) {
	auto* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
	if (gameWindow && gameWindow->resizeCallback) {
		gameWindow->resizeCallback(window, width, height);
		std::cout << "Window size : (" << width << "," << height << ")" << std::endl;
	}
}