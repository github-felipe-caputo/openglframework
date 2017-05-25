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
