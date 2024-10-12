#pragma once

#define GLFW_INCLUDE_NONE
#include <glew.h>
#include <glfw3.h>
#include <string>
#include <functional>
#include <stdexcept>

class GameWindow {
public:
	GameWindow(int width, int height, const std::string& title);

	~GameWindow();

	bool shouldClose() const;

	void pollEvents();

	GLFWwindow* getNativeWindow() const;

	void setWindowHints(const std::function<void()>& hintSetter);

	int getWidth() const;
	int getHeight() const;

	void setResizeCallback(std::function<void(GLFWwindow*, int, int)> callback);

private:

	void initialize();

	void shutdown();

	static void resizeCallbackWrapper(GLFWwindow* window, int width, int height);

	int width;
	int height;
	std::string title;
	GLFWwindow* pWindow;
	std::function<void(GLFWwindow*, int, int)> resizeCallback;

};


//
//class GameWindow {
//public:
//	GameWindow(int width, int height, const std::string& title)
//		:m_width(width), m_height(height), m_window(nullptr) {
//		initialize();
//	}
//
//	~GameWindow() {
//		shutdown();
//	}
//
//	bool shouldClose() const {
//		return glfwWindowShouldClose(m_window);
//	}
//
//	void pollEvents() {
//		glfwPollEvents();
//	}
//
//	void clear(float r, float g, float b, float a) {
//		glClearColor(r, g, b, a);
//		glClear(GL_COLOR_BUFFER_BIT);
//	}
//
//	void swapBuffers() {
//		glfwSwapBuffers(m_window);
//	}
//
//	GLFWwindow* getGLFWwindow() const {
//		return m_window;
//	}
//
//private:
//	void initialize() {
//		if (!glfwInit()) {
//			throw std::runtime_error("Fail to initialize GLFW");
//		}
//
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
//		if (!m_window) {
//			glfwTerminate();
//			throw std::runtime_error("Failed to create GLFW window");
//		}
//
//		glfwMakeContextCurrent(m_window);
//
//		if (glewInit() != GLEW_OK) {
//			glfwDestroyWindow(m_window);
//			glfwTerminate();
//			throw std::runtime_error("Failed to initialize GLEW");
//		}
//	}
//
//	void shutdown() {
//		if (m_window) {
//			glfwDestroyWindow(m_window);
//		}
//		glfwTerminate();
//	}
//
//	int m_width;
//	int m_height;
//	std::string m_title;
//	GLFWwindow* m_window;
//};
