#pragma once

#include <glew.h>
#include <glfw3.h>
#include "GameWindow.h"
#include "Vector3.h"
#include "Matrix4.h"

class Renderer {
public:
	Renderer(GameWindow& window);

	void clear(float r, float g, float b, float a);
	void swapBuffers();
	void drawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Matrix4 modelMatrix);

private:
	void initialize();
	void setupCallbacks();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	GameWindow& pWindow;

	GLuint shaderProgram;
	GLuint VAO;
	GLuint VBO;
	Matrix4 projectionMatrix;
	Matrix4 modelMatrix;
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projectionLoc;
};