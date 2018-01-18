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

#include "mathHelper.h"

#include <vector>
#include <iostream>

using namespace std;

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

    /*
     * getCameraPosition
     *
     * RETURN:
     *         The camera (eye pos) position in a vector<float> format.
     *
     * DESCRIPTION:
     *         Simply returns the camera position in a vector format.
     *
     */
    vector<float> getCameraPosition ();
};

#endif