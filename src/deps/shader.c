#include "../../include/solar_system/shader.h"
#include <stdio.h>
#include <stdlib.h>

void shaderProcessFile(char* fileName, GLenum type) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("ERROR: File [%s] can not be opened\n", fileName);
        exit(1);
    }

    // get file size
    fseek(fp,0L, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp,0L, SEEK_SET);

    // place shader source into buffer
    char* shaderSourceBuffer = malloc((fileSize + 1) * sizeof(char));
    if (!shaderSourceBuffer) {
        printf("ERROR: Unable to allocate space for shaderSourceBuffer");
        fclose(fp);
        free(shaderSourceBuffer);
        exit(1);
    }
    fread(shaderSourceBuffer, 1, fileSize, fp);
    shaderSourceBuffer[fileSize] = '\0';
    fclose(fp);
    
    // create a shader
    unsigned int shader = glCreateShader(type);

    // pass in shader source
    const char* shaderSource = shaderSourceBuffer;
    glShaderSource(shader, 1, &shaderSource, NULL);
    free(shaderSourceBuffer);

    // create/compile shader
    glCompileShader(shader);

    // error check shader
    int success;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("ERROR: Unable to compile shader\n");
        exit(1);
    }
}
