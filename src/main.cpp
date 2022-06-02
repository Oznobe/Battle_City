#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int WindowX = 640;
int WindowY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    WindowX = width;
    WindowY = height;
    glViewport(0, 0, WindowX, WindowY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main() {
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(WindowX, WindowY, "Battle City", nullptr, nullptr);
    if (!pWindow) {
        std::cout << "glfwCreateWindow failed\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

	if(!gladLoadGL()) {
		std::cout << "Can`t load GLAD\n"; 
		return -1;
	}
    std::cout << "Render:\t" << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version:\t" << glGetString(GL_VERSION) << "\n";
	glClearColor (1, 1, 0, 1);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
