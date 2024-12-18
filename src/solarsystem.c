// -- includes --
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../include/glad/glad.h"
#include "../include/glfw/glfw3.h"
#include "../include/solar_system/shader.h"

// -- global variables --
const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 1200;

// -- callback functions --
void framebufferSizeCallback(GLFWwindow* pWindowIn, int width, int height);

// -- helper functions --
void processInput(GLFWwindow* pWindowIn);
char* concat(const char* str1, const char* str2);

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
        printf("GLFW::INIT::ERROR - Failed to create glfw window");
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
         // positions             colors
        -0.5f,  -0.5f,   0.0f,    1.0f,   0.0f,   0.0f,
         0.5f,  -0.5f,   0.0f,    0.0f,   1.0f,   0.0f,
         0.0f,   0.5f,   0.0f,    0.0f,   0.0f,   1.0f
    };

    // -- initialize buffers --
    unsigned int VAO, VBO; 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // -- set up attribute pointers for vertex data --
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // -- unbind to prevent accidental state changes --
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // -- create shader program --
    GLuint shaderProgram = glCreateProgram();

    // -- get shader sources -- 
    char cwd[512];
    getcwd(cwd, 512);
    char* vertexShaderPath = concat(cwd, "/src/shaders/vertex.glsl");
    char* fragmentShaderPath = concat(cwd, "/src/shaders/fragment.glsl");
    const char* vertexShaderSource = shaderGetShaderSource(vertexShaderPath);
    const char* fragmentShaderSource = shaderGetShaderSource(fragmentShaderPath);
    free(vertexShaderPath);
    free(fragmentShaderPath);

    // -- create and compile shaders --
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // -- error check shaders --
    int success; char log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        printf("ERROR: Unable to compile vertex shader\n%s", log);
        exit(1);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        printf("ERROR: Unable to compile fragment shader\n%s", log);
        exit(1);
    }

    // -- attach shaders --
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // -- link program --
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        printf("ERROR: Unable to compile shader program\n%s", log);
        exit(1);
    }

    // -- clean up shaders --
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free((void*)vertexShaderSource); // NOTE: i don't know if this causes any side effects!!!
    free((void*)fragmentShaderSource);

    // -- render loop --
    while (!glfwWindowShouldClose(pWindow)) {
        // keyboard input
        processInput(pWindow);

        // gl: background
        glClearColor(0.0f, 0.7f, 0.7f, 1.0f); // bright blue
        glClear(GL_COLOR_BUFFER_BIT);

        // draw sample triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: input calls
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    // -- deallocate buffers --
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// -- callback functions --
void framebufferSizeCallback(GLFWwindow* pWindowIn, int width, int height) {
    glViewport(0, 0, width, height);
}

// -- helper functions --
void processInput(GLFWwindow* pWindowIn) {
    if (glfwGetKey(pWindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(pWindowIn, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindowIn, GL_TRUE);
}

char* concat(const char* str1, const char* str2) {
    char* combinedString = malloc( sizeof(char) * (strlen(str1) + strlen(str2)) );
    strcpy(combinedString, str1);
    strcat(combinedString, str2);
    return combinedString;
}
