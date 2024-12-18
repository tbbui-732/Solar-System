#include "../../include/solar_system/shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* shaderGetShaderSource(char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("ERROR: File [%s] can not be opened\n", fileName);
        exit(1);
    }

    // get file size
    fseek(fp, 0L, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // place shader source into buffer
    char* shaderSourceBuffer = malloc((fileSize + 1) * sizeof(char));
    if (!shaderSourceBuffer) {
        printf("SHADER::ERROR - Unable to allocate space for shaderSourceBuffer");
        fclose(fp);
        free(shaderSourceBuffer);
        exit(1);
    }
    fread(shaderSourceBuffer, 1, fileSize, fp);
    shaderSourceBuffer[fileSize] = '\0';
    fclose(fp);

    return (const char*) shaderSourceBuffer;
}
