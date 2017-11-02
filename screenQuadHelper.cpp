#include "screenQuadHelper.h"

float quadVertices[] = {
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f
};

float quadTextures[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

GLshort quadElements[] = {
    0, 1, 2,
    0, 2, 3
};