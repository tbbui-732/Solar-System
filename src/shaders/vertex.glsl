#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

out vec3 ourColor;

void main() {
    gl_Position = vec4(vec3, 1.0f);
    ourColor = inColor;
}