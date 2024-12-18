#include "../../include/solar_system/spheres.h"

#include <math.h>
#include <stdlib.h>

void sphereBuildVertices(float* vertices, float* normals, float* texCoords, float radius, int sectorCount, int stackCount) {
    // allocate space for each array
    vertices    = malloc(sizeof(float) * 3 * sectorCount * stackCount); // 3 to account for x, y, and z
    normals     = malloc(sizeof(float) * 3 * sectorCount * stackCount); // same as vertices
    texCoords   = malloc(sizeof(float) * 2 * sectorCount * stackCount); // 2 to account for s and t

    const float PI = acos(-1.0f);
    int vertIdx = 0;
    int normIdx = 0;
    int texIdx  = 0;

    float lengthInv = 1.0f / radius;

    // sector and stack steps and angles
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute vertices
    for (int i = 0; i < stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;
        float rCosStackAngle = radius * cosf(stackAngle);
        float rSinStackAngle = radius * sinf(stackAngle);

        for (int j = 0; j < sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            // calculate vertices
            float x = rCosStackAngle * cosf(sectorAngle);
            float y = rCosStackAngle * sinf(sectorAngle);
            float z = rSinStackAngle;
            vertices[vertIdx++] = x;
            vertices[vertIdx++] = y;
            vertices[vertIdx++] = z;

            // calculate normals
            float nx = x * lengthInv;
            float ny = y * lengthInv;
            float nz = z * lengthInv;
            normals[normIdx++] = nx;
            normals[normIdx++] = ny;
            normals[normIdx++] = nz;

            // calculate texture coordinates
            float s = (float) j / sectorCount;
            float t = (float) i / stackCount;
            texCoords[texIdx++] = s;
            texCoords[texIdx++] = t;
        }
    }

    return vertices
}
