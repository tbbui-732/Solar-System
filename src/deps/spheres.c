#include "../../include/solar_system/spheres.h"


void sphereBuildVertices(float radius, int sectorCount, int stackCount) {
    const float PI = acos(-1.0f);
    
    float tmpVertices[3 * sectorCount * stackCount];
    int vi = 0;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute vertices
    for (int i = 0; i < stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;
        float rCos = radius * cosf(stackAngle); // r * cos(u)
        float rSin = radius * sinf(stackAngle); // r * sin(u)

        for (int j = 0; j < sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            float x = rCos * cosf(sectorAngle);
            float y = rCos * sinf(sectorAngle);
            float z = rSin;

            tmpVertices[vi++] = x;
            tmpVertices[vi++] = y;
            tmpVertices[vi++] = z;
        }
    }
}
