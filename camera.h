/*
 * camera.h
 *
 * Set up the camera.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#ifndef _CAMERA_H
#define _CAMERA_H

#include "libs/cmatrix"
#include "mathHelper.h"

#include <iostream>

using namespace std;

// sometTypedef to help us use the cmatrix lib

// We use a "vector" (actually a matrix with 4 row and 1 col) to help us
// deal with some transformation to "move" the camera, taking advantage
// of the cmatrix lib. Since those vectors will go through transformations,
// they are in homogeneous coordinates (1.0f on the last coordinate)
typedef techsoft::matrix<float> Vector4;
typedef techsoft::matrix<float> Matrix4;

// Some helpful definitions
#define PROJ_PERSP    1
#define PROJ_ORTHO    2
#define PI            3.14159265

const float CAM_SPEED = 0.1;

/*
 * The camera class.
 */
class Camera {
    // eye/camera position
    Vector4 eyePoint;

    // where the camera is looking at
    Vector4 lookAt;

    // up vector of the camera
    Vector4 up;

    // help vector to find "up" and to strafe right and left
    Vector4 right;

    // view matrix
    Matrix4 viewMatrix;

    // projection matrix
    Matrix4 projMatrix;

    // mouse values to move the camera
    int mouseOldX, mouseOldY;

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

public:

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
    Camera (int type);

    /* 
     * ~Camera
     *
     * DESCRIPTION:
     *         Default descontructor.
     *
     */
    ~Camera ();

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
    Matrix4 getViewMatrix ();

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
    Matrix4 getProjMatrix ();

    /* 
     * moveForward
     *
     * DESCRIPTION:
     *         This function simply moves the camera forward.
     *
     */
    void moveForward ();

    /* 
     * moveBackward
     *
     * DESCRIPTION:
     *         This function simply moves the camera backwards.
     *
     */
    void moveBackward ();

    /* 
     * strafeRight
     *
     * DESCRIPTION:
     *         This function strafes the camera right.
     *
     */
    void strafeRight ();

    /* 
     * strafeLeft
     *
     * DESCRIPTION:
     *         This function strafes the camera left.
     *
     */
    void strafeLeft ();

    /* 
     * moveUp
     *
     * DESCRIPTION:
     *         This function simply moves the camera up.
     *
     */
    void moveUp ();

    /* 
     * moveDown
     *
     * DESCRIPTION:
     *         This function simply moves the camera down.
     *
     */
    void moveDown ();

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
    void setInitialMouseCoord (int x, int y);

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
    void moveCameraTarget (int x, int y);

};

#endif