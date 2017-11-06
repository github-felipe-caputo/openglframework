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
Matrix4 makeViewMatrix( Vector4 newEyePoint, Vector4 newLookAt, Vector4 newUp ) {
    // n vector
    float n[3];
    n[0] = newEyePoint[0][0] - newLookAt[0][0];
    n[1] = newEyePoint[1][0] - newLookAt[1][0];
    n[2] = newEyePoint[2][0] - newLookAt[2][0];
    normalize(n);

    // u vector
    float u[3];
    u[0] = (newUp[1][0] * n[2]) - (newUp[2][0] * n[1]);
    u[1] = (newUp[2][0] * n[0]) - (newUp[0][0] * n[2]);
    u[2] = (newUp[0][0] * n[1]) - (newUp[1][0] * n[0]);
    normalize(u);

    // v vector
    float v[3];
    v[0] = (n[1] * u[2]) - (n[2] * u[1]);
    v[1] = (n[2] * u[0]) - (n[0] * u[2]);
    v[2] = (n[0] * u[1]) - (n[1] * u[0]);
    normalize(v);

    float viewMat[] = {u[0], u[1], u[2], -1*dotProduct(u, &newEyePoint[0][0]),
                       v[0], v[1], v[2], -1*dotProduct(v, &newEyePoint[0][0]),
                       n[0], n[1], n[2], -1*dotProduct(n, &newEyePoint[0][0]),
                       0.0f, 0.0f, 0.0f,                                 1.0f};

    Matrix4 worldToCamera(4,4,viewMat);
    return worldToCamera;
}

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
Matrix4 makeViewMatrix( float newEyePoint[], float newLookAt[], float newUp[] ) {
    // n vector
    float n[3];
    n[0] = newEyePoint[0] - newLookAt[0];
    n[1] = newEyePoint[1] - newLookAt[1];
    n[2] = newEyePoint[2] - newLookAt[2];
    normalize(n);

    // u vector
    float u[3];
    u[0] = (newUp[1] * n[2]) - (newUp[2] * n[1]);
    u[1] = (newUp[2] * n[0]) - (newUp[0] * n[2]);
    u[2] = (newUp[0] * n[1]) - (newUp[1] * n[0]);
    normalize(u);

    // v vector
    float v[3];
    v[0] = (n[1] * u[2]) - (n[2] * u[1]);
    v[1] = (n[2] * u[0]) - (n[0] * u[2]);
    v[2] = (n[0] * u[1]) - (n[1] * u[0]);
    normalize(v);

    float viewMat[] = {u[0], u[1], u[2], -1*dotProduct(u, newEyePoint),
                       v[0], v[1], v[2], -1*dotProduct(v, newEyePoint),
                       n[0], n[1], n[2], -1*dotProduct(n, newEyePoint),
                       0.0f, 0.0f, 0.0f,                            1.0f};

    Matrix4 worldToCamera(4,4,viewMat);
    return worldToCamera;
}

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
Matrix4 makeOrthographicMatrix ( float l, float r, float b, float t, float n, float f ) {
    float orthoMatrix4[] = {2.0f/(r-l),       0.0f,        0.0f, -(r+l)/(r-l),
                                  0.0f, 2.0f/(t-b),        0.0f, -(t+b)/(t-b),
                                  0.0f,       0.0f, -2.0f/(f-n), -(f+n)/(f-n),
                                  0.0f,       0.0f,        0.0f,         1.0f};

    Matrix4 ortho(4,4,orthoMatrix4);
    return ortho;
}

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
Matrix4 makePerspectiveMatrix ( float fov, float w, float h, float zn, float zf ) {
    float r = w/h;
    float f = tan((fov * PI) / 180.0f);

    float projMat[] = {  f/r, 0.0f,            0.0f,              0.0f,
                        0.0f,    f,            0.0f,              0.0f,
                        0.0f, 0.0f, (zf+zn)/(zn-zf), (2*zf*zn)/(zn-zf),
                        0.0f, 0.0f,           -1.0f,              0.0f};

    Matrix4 proj(4,4,projMat);
    return proj;
}

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
Matrix4 makePerspectiveMatrix2 ( float l, float r, float b, float t, float n, float f ) {
    float projMat[] = {(2.0f*n)/(r-l),          0.0f,        (r+l)/(r-l),              0.0f,
                                 0.0f, (2.0f*n)/(t-b),        (t+b)/(t-b),              0.0f,
                                 0.0f,          0.0f, (-1.0f*(f+n))/(f-n), (-2.0f*f*n)/(f-n),
                                 0.0f,          0.0f,              -1.0f,              0.0f};

    Matrix4 proj(4,4,projMat);
    return proj;
}

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
    float rotate[] = { static_cast<float>(cosTheta + pow(wx,2) * (1 - cosTheta))     , wx * wy * (1 - cosTheta) - wz * sinTheta , wy * sinTheta + wx * wz * (1 - cosTheta)  , 0.0f ,
                       wz * sinTheta + wx * wy * (1 - cosTheta)  , static_cast<float>(cosTheta + pow(wy,2) * (1 - cosTheta))    , -wx * sinTheta + wy * wz * (1 - cosTheta) , 0.0f ,
                       -wy * sinTheta + wx * wz * (1 - cosTheta) , wx * sinTheta + wy * wz * (1 - cosTheta) , static_cast<float>(cosTheta + pow(wz,2) * (1 - cosTheta))     , 0.0f ,
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
