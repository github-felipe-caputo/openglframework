/*
 * mathHelper.h
 *
 * Set up transformation matrixes and othermathe related functions.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#ifndef _MATHHELPER_H_
#define _MATHHELPER_H_

#include "libs/cmatrix"

#define PI      3.14159265

// Typedef to help us use the cmatrix lib
typedef techsoft::matrix<float> Matrix;

// We use a "vector" (actually a matrix with 4 row and 1 col) to help us
// deal with some transformation to "move" the camera, taking advantage
// of the cmatrix lib. Since those vectors will go through transformations,
// they are in homogeneous coordinates (1.0f on the last coordinate)
typedef techsoft::matrix<float> Vector4;
typedef techsoft::matrix<float> Matrix4;

/*
* makeViewMatrix
*
* INPUT:
*         eyePoint - position of the eye/camera.
*         lookAt - where the camera is looking at.
*         up - "up" vector of the camera.
*
* RETURN:
*         The view matrix.
*
* DESCRIPTION:
*         This function is responsible for creating a
*         a view matrix given the basic values of the camera.
*
*/
Matrix4 makeViewMatrix( Vector4 newEyePoint, Vector4 newLookAt, Vector4 newUp );

/*
 * makeViewMatrix
 *
 * INPUT:
 *         eyePoint - position of the eye/camera, float[3].
 *         lookAt - where the camera is looking at, float[3].
 *         up - "up" vector of the camera, float[3].
 *
 * RETURN:
 *         The view matrix.
 *
 * DESCRIPTION:
 *         This function is responsible for creating a
 *         a view matrix given the basic values of the camera.
 *
 */
Matrix4 makeViewMatrix( float newEyePoint[], float newLookAt[], float newUp[] );

/*
* makeOrthographicMatrix
*
* INPUT:
*         l - left limit of the projection volume.
*         r - right limit of the projection volume.
*         b - bottom limit of the projection volume.
*         t - top limit of the projection volume.
*         n - near limit of the projection volume.
*         f - far limit of the projection volume.
*
* RETURN:
*         The orthographic projection matrix.
*
* DESCRIPTION:
*         This function is responsible for creating a
*         a viewing volume matrix (the projection matrix),
*         in specific this function creates an orthographic
*         projection matrix.
*
*/
Matrix4 makeOrthographicMatrix ( float l, float r, float b, float t, float n, float f );

/*
* makePerspectiveMatrix
*
* INPUT:
*         fov - the field of view of the whole projection volume.
*         w - the width of the projection volume.
*         h - the height limit of the projection volume.
*         zn - near limit of the projection volume.
*         zf - far limit of the projection volume.
*
* RETURN:
*         The perspective projection matrix.
*
* DESCRIPTION:
*         This function is responsible for creating a
*         a viewing volume matrix (the projection matrix),
*         in specific this function creates an perspective
*         projection matrix.
*
*/
Matrix4 makePerspectiveMatrix ( float fov, float w, float h, float zn, float zf );

/*
* makePerspectiveMatrix2
*
* INPUT:
*         l - left limit of the projection volume.
*         r - right limit of the projection volume.
*         b - bottom limit of the projection volume.
*         t - top limit of the projection volume.
*         n - near limit of the projection volume.
*         f - far limit of the projection volume.
*
* RETURN:
*         The perspective projection matrix.
*
* DESCRIPTION:
*         This function is responsible for creating a
*         a viewing volume matrix (the projection matrix),
*         in specific this function creates an perspective
*         projection matrix. It's a different version to calculate
*         this matrix.
*
*/
Matrix4 makePerspectiveMatrix2 ( float l, float r, float b, float t, float n, float f );

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
Matrix translate ( float tx, float ty, float tz );

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
Matrix scale ( float sx, float sy, float sz );

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
Matrix rotate ( float theta, float vector[] );

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
void normalize ( float v[] );

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
float dotProduct ( float n[], float u[] );

#endif
