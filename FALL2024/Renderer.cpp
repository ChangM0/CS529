#include <stdexcept>
#include <iostream>
#include "Renderer.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

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

	int success;
	char infoLog[512];

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW");

	// create compile shader
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 ModelMatrix, ViewMatrix, ProjectionMatrix;\n"
		"void main()\n"
		"{\n"
		" gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Get uniform locations
	modelLoc = glGetUniformLocation(shaderProgram, "ModelMatrix");
	viewLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
	projectionLoc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

}

void Renderer::setupCallbacks() {
	pWindow.setResizeCallback([this](GLFWwindow* window, int width, int height) {
		this->framebufferSizeCallback(window, width, height);
		});
}

void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void Renderer::drawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Matrix4 modelMatrix) {

	glUseProgram(shaderProgram);


	// Create transformation matrices
	Matrix4 model = Matrix4();
	Matrix4 view = Matrix4();
	Matrix4 projection = Matrix4();

	// Model transformations
	float time = (float)glfwGetTime();
	model = model.Matrix4::rotationY(time);  // Rotate around Y-axis
	model = model.Matrix4::scale(0.5f, 0.5f, 0.5f);  // Scale down

	// View transformation
	view = view.Matrix4::translation(0.0f, 0.0f, -3.0f);  /// Move scene away from camera

	// Projection transformation
	float aspectRatio = static_cast<float>(pWindow.getWidth()) / static_cast<float>(pWindow.getHeight());
	projection = projection.Matrix4::perspective(45.0f * 3.14159f / 180.0f, aspectRatio, 0.1f, 10000.0f);

	// Set uniforms
	modelLoc = glGetUniformLocation(shaderProgram, "ModelMatrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix.getData());

	// temp view matrix
	float tmp = 0;
	tmp += 0.01f;
    Vector3 cameraPos(0.0f, 0.0f, 2.0);
    Vector3 cameraTarget = cameraPos + Vector3(0.0f, 0.0f, -1.0f);
    Vector3 upVector(0.0f, 1.0f, 0.0f);
	Matrix4 viewMatrix;
	//viewMatrix = Matrix4::lookAt(cameraPos, cameraTarget, upVector);
	viewLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.getData());

	projectionLoc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix.getData());

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glUseProgram(0);
}
