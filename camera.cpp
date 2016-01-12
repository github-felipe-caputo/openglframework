/*
 * camera.cpp
 *
 * Set up the camera.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#include "camera.h"

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
Matrix4 Camera::makeViewMatrix( Vector4 newEyePoint, Vector4 newLookAt, Vector4 newUp ) {
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
Matrix4 Camera::makeOrthographicMatrix ( float l, float r, float b, float t, float n, float f ) {
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
Matrix4 Camera::makePerspectiveMatrix ( float fov, float w, float h, float zn, float zf ) {
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
Matrix4 Camera::makePerspectiveMatrix2 ( float l, float r, float b, float t, float n, float f ) {
    float projMat[] = {(2.0f*n)/(r-l),          0.0f,        (r+l)/(r-l),              0.0f,
                                 0.0f, (2.0*n)/(t-b),        (t+b)/(t-b),              0.0f,
                                 0.0f,          0.0f, (-1.0*(f+n))/(f-n), (-2.0f*f*n)/(f-n),
                                 0.0f,          0.0f,              -1.0f,              0.0f};

    Matrix4 proj(4,4,projMat);
    return proj;
}

/* 
 * Camera
 *
 * INPUT: 
 *         type - the type of the projection.
 *
 * DESCRIPTION:
 *         This is this class constructor. It creates a camera
 *         with some default values (centered at the origin, looking
 *         at the negative z axis, up vector is the world's positive y).
 *         And it also defines a default projection that can be either 
 *         orthographic or prespective
 *
 */
Camera::Camera ( int type ) {
    float eyeVec[] = {0.0f,0.0f,0.0f,1.0f};
    float lookVec[] = {0.0f,0.0f,-1.0f,1.0f};
    float upVec[] = {0.0f,1.0f,0.0f,1.0f};
    float rightVec[] = {1.0f,0.0f,0.0f,1.0f};

    Vector4 eyeAux(4,1,eyeVec);
    eyePoint = eyeAux;

    Vector4 lookAux(4,1,lookVec);
    lookAt = lookAux;

    Vector4 upAux(4,1,upVec);
    up = upAux;

    Vector4 rightAux(4,1,rightVec);
    right = rightAux;

    viewMatrix = makeViewMatrix(eyePoint, lookAt, up);
    if (type == PROJ_PERSP) {
        projMatrix = makePerspectiveMatrix( 60.0f, 1.0f, 1.0f, 0.5f, 50.0f );
    } else {
        projMatrix = makeOrthographicMatrix( -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10.0f );
    }
} 

/* 
 * ~Camera
 *
 * DESCRIPTION:
 *         Default descontructor.
 *
 */
Camera::~Camera () {
}

/* 
 * getViewMatrix
 *
 * RETURN:
 *         The view matrix.
 *
 * DESCRIPTION:
 *         This function simply returns the view matrix of 
 *         this camera object.
 *
 */
Matrix4 Camera::getViewMatrix () {
    return viewMatrix;
}

/* 
 * getProjMatrix
 *
 * RETURN:
 *         The projection matrix.
 *
 * DESCRIPTION:
 *         This function simply returns the projection matrix of 
 *         this camera object.
 *
 */
Matrix4 Camera::getProjMatrix () {
    return projMatrix;
}

/* 
 * moveForward
 *
 * DESCRIPTION:
 *         This function simply moves the camera forward.
 *
 */
void Camera::moveForward () {
    eyePoint += lookAt * CAM_SPEED;
    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);
}

/* 
 * moveBackward
 *
 * DESCRIPTION:
 *         This function simply moves the camera backwards.
 *
 */
void Camera::moveBackward () {
    eyePoint -= lookAt * CAM_SPEED;
    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);
}

/* 
 * strafeRight
 *
 * DESCRIPTION:
 *         This function strafes the camera right.
 *
 */
void Camera::strafeRight () {
    eyePoint += right * CAM_SPEED;
    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);
}

/* 
 * strafeLeft
 *
 * DESCRIPTION:
 *         This function strafes the camera left.
 *
 */
void Camera::strafeLeft () {
    eyePoint -= right * CAM_SPEED;
    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);
}

/* 
 * moveUp
 *
 * DESCRIPTION:
 *         This function simply moves the camera up.
 *
 */
void Camera::moveUp () {
    eyePoint += up * CAM_SPEED;
    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);
}

/* 
 * moveDown
 *
 * DESCRIPTION:
 *         This function simply moves the camera down.
 *
 */
void Camera::moveDown () {
    eyePoint -= up * CAM_SPEED;
    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);
}

/* 
 * setInitialMouseCoord
 *
 * INPUT: 
 *         x - the x coordinate of the mouse click.
 *         y - the y coordinate of the mouse click.
 *
 * DESCRIPTION:
 *         This function saves the position where the mouse just clicked,
 *         this information will be used to move the scene given the mouse
 *         movement.
 *
 */
void Camera::setInitialMouseCoord (int x, int y) {
    mouseOldX = x;
    mouseOldY = y;
}

/* 
 * moveCameraTarget
 *
 * INPUT: 
 *         x - the x coordinate of the current mouse position.
 *         y - the y coordinate of the current mouse position.
 *
 * DESCRIPTION:
 *         This function will calculate the distance of the last mouse
 *         position and the current one, and will move the camera accordingly.
 *         It works jsut like the camera of most FPS games.
 *
 */
void Camera::moveCameraTarget (int x, int y) {
    float deltaX = (x - mouseOldX) * CAM_SPEED;
    float deltaY = (y - mouseOldY) * CAM_SPEED;

    // changes lookat
    lookAt = rotate(-deltaX, &up[0][0]) * rotate(-deltaY, &right[0][0]) * lookAt;

    // right changed because of the movement, update
    right = rotate(-deltaX, &up[0][0]) * right;

    viewMatrix = makeViewMatrix(eyePoint, eyePoint+lookAt, up);

    mouseOldX = x;
    mouseOldY = y;
}