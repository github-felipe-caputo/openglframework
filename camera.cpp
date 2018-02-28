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


void Camera::setCameraPosition (float x, float y, float z) {
    float eyeVec[] = { x, y, z, 1.0f};

    Vector4 eyeAux(4,1,eyeVec);
    eyePoint = eyeAux;
}

void Camera::setLookAt (float x, float y, float z) {
    float lookVec[] = { x, y, z, 1.0f};

    Vector4 lookVecAux(4,1,lookVec);
    lookAt = lookVecAux;
}

void Camera::setRight (float x, float y, float z) {
    float rightVec[] = { x, y, z, 1.0f};

    Vector4 rightVecAux(4,1,rightVec);
    right = rightVecAux;
}

void Camera::setUp (float x, float y, float z) {
    float upVec[] = { x, y, z, 1.0f};

    Vector4 upVecAux(4,1,upVec);
    up = upVecAux;
}

vector<float> Camera::getCameraPosition () {
    vector<float> pos;

    pos.push_back(eyePoint[0][0]);
    pos.push_back(eyePoint[1][0]);
    pos.push_back(eyePoint[2][0]);

    return pos;
}

vector<float> Camera::getLookAt () {
    vector<float> pos;

    pos.push_back(lookAt[0][0]);
    pos.push_back(lookAt[1][0]);
    pos.push_back(lookAt[2][0]);

    return pos;
}

vector<float> Camera::getRight () {
    vector<float> pos;

    pos.push_back(right[0][0]);
    pos.push_back(right[1][0]);
    pos.push_back(right[2][0]);

    return pos;
}

vector<float> Camera::getUp () {
    vector<float> pos;

    pos.push_back(up[0][0]);
    pos.push_back(up[1][0]);
    pos.push_back(up[2][0]);

    return pos;
}