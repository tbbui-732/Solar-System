// -- includes --
#include <stdio.h>
#include "../include/glad/glad.h"
#include "../include/glfw/glfw3.h"

// -- global variables --
const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 900;

// -- callback functions --
void framebufferSizeCallback(GLFWwindow* pWindowIn, int width, int height);

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

    // -- set up callback functions for poll event --
    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback); // window resizing

    // -- initialize glad --
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Unable to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    // -- vertex data --
    float vertices[] = {
        -0.5f,  -0.5f,  0.0f,
         0.5f,  -0.5f,  0.0f,
         0.0f,   0.5f,  0.0f
    };

    // -- initialize buffers --
    unsigned int VAO, VBO; 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)/sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    
    // -- set up attribute pointers for vertex data --
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);

    // -- unbind to prevent accidental state changes --
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // -- render loop --
    while (!glfwWindowShouldClose(pWindow)) {
        // gl: background
        glClearColor(0.0f, 0.7f, 0.7f, 1.0f); // bright blue
        glClear(GL_COLOR_BUFFER_BIT);

        // draw sample triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: input calls
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    // -- deallocate buffers --
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* pWindowIn, int width, int height) {
    glViewport(0, 0, width, height);
}
