/*
 * mathHelper.cpp
 *
 * Set up transformation matrixes.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#include "mathHelper.h"

/* 
 * translate
 *
 * INPUT: 
 *         tx - translation on the x axis.
 *         ty - translation on the y axis.
 *         tz - translation on the z axis.
 *
 * RETURN:
 *         The translation matrix.
 *
 * DESCRIPTION:
 *         This function is responsible for creating a
 *         a translation matrix given the inputs of the
 *         translation in each direction. 
 *
 */
Matrix translate ( float tx, float ty, float tz ) {
    float translate[] = {1.0f, 0.0f, 0.0f,    tx,
                         0.0f, 1.0f, 0.0f,    ty,
                         0.0f, 0.0f, 1.0f,    tz,
                         0.0f, 0.0f, 0.0f,  1.0f};
    Matrix mTranslate(4,4,translate);

    return mTranslate;
}

/* 
 * scale
 *
 * INPUT: 
 *         sx - scale on the x axis.
 *         sy - scale on the y axis.
 *         sz - scale on the z axis.
 *
 * RETURN:
 *         The scale matrix.
 *
 * DESCRIPTION:
 *         This function is responsible for creating a
 *         a scale matrix given the inputs of the
 *         scale in each direction. 
 *
 */
Matrix scale ( float sx, float sy, float sz ) {
    float scale[] = {  sx, 0.0f, 0.0f, 0.0f,
                     0.0f,   sy, 0.0f, 0.0f,
                     0.0f, 0.0f,   sz, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f};
    Matrix mScale(4,4,scale);

    return mScale;
}

/* 
 * rotate
 *
 * INPUT: 
 *         theta - angle (in degrees) of the rotation.
 *         vector - the angle the roation will use as a reference
 *
 * RETURN:
 *         The rotation matrix.
 *
 * DESCRIPTION:
 *         This function is responsible for creating a
 *         a rotation matrix by the theta angle about a fixed
 *         axis specified by the unity vector.
 *
 *         Reference:
 *         Belongie, Serge. "Rodrigues' Rotation Formula." 
 *         From MathWorld--A Wolfram Web Resource, created by Eric W. Weisstein.
 *         http://mathworld.wolfram.com/RodriguesRotationFormula.html
 *
 */
Matrix rotate ( float theta, float vector[] ) {
    float rad = ((theta * PI) / 180.0);
    float cosTheta = cos(rad);
    float sinTheta = sin(rad);
    float wx = vector[0];
    float wy = vector[1];
    float wz = vector[2];
    float rotate[] = { cosTheta + pow(wx,2) * (1 - cosTheta)     , wx * wy * (1 - cosTheta) - wz * sinTheta , wy * sinTheta + wx * wz * (1 - cosTheta)  , 0.0f ,
                       wz * sinTheta + wx * wy * (1 - cosTheta)  , cosTheta + pow(wy,2) * (1 - cosTheta)    , -wx * sinTheta + wy * wz * (1 - cosTheta) , 0.0f ,
                       -wy * sinTheta + wx * wz * (1 - cosTheta) , wx * sinTheta + wy * wz * (1 - cosTheta) , cosTheta + pow(wz,2) * (1 - cosTheta)     , 0.0f ,
                                                             0.0f,                                      0.0f,                                       0.0f, 1.0f };

    Matrix mRotate(4,4,rotate);

    return mRotate; 
}

/* 
 * normalize
 *
 * INPUT: 
 *         v - vector to be normalized.
 *
 * DESCRIPTION:
 *         Normalizes a vector.
 *
 */
void normalize ( float v[] ) {
    float len = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );

    if (len != 0) {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
        v[2] = v[2] / len;
    } 
}

/* 
 * dotProduct
 *
 * INPUT: 
 *         n - first vector for the dot product.
 *         u - second vector for the dot product.
 *
 * RETURN:
 *         The dot product.
 *
 * DESCRIPTION:
 *         Calculates the dot product of two vectors.
 *
 */
float dotProduct ( float n[], float u[] ) {
    return n[0] * u[0] + n[1] * u[1] + n[2] * u[2];
}
