// -- includes --
#include <stdio.h>
#include "../include/glad/glad.h"
#include "../include/glfw/glfw3.h"

// -- global variables --
const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 800;


int main(void) {
    // -- initialize glfw --
    if (!glfwInit()) {
        printf("GLFW::INIT - Failed to initialize glfw");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_CHDIR_RESOURCES, GL_FALSE);
#endif

    // -- set up glfw window -- 
    GLFWwindow* pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
    if (!pWindow) {
        printf("GLFW::INIT - Failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(pWindow);

    // -- initialize glad --
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Unable to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    // -- render loop --
    while (!glfwWindowShouldClose(pWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
