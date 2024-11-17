#include "Renderer.h"
#include "GameWindow.h"
#include "Vector3.h"
#include <iostream>

int main() {
    try {
        GameWindow window(800, 600, "OpenGL Window");
        Renderer renderer(window);

        float angle = 0.0f;
        float scaleAmount = 10.0f;
        float scaleSpeed = 10.0f;

        while (!window.shouldClose()) {
            window.pollEvents();

            renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);

            angle += 0.01f;
            scaleAmount = 1.0f + 0.5f * std::sin(scaleSpeed * glfwGetTime());

            Matrix4 translation = Matrix4::translation(angle, 0.0f, 0.0f);
            Matrix4 rotation = Matrix4::rotationY(angle*-10);
            Matrix4 scale = Matrix4::scale(scaleAmount, scaleAmount, scaleAmount);
            
            Matrix4 modelMatrix = rotation * scale;

            renderer.drawTriangle(Vector3(-0.5f, -0.5f, 0.0f), 
                Vector3(0.5f, -0.5f, 0.0f), 
                Vector3(0.0f, 0.5f, 0.0f),
                modelMatrix);
            
            // Render your 3D scene here
            renderer.swapBuffers();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
