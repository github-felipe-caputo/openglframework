/*
 * imageHelper.h
 *
 * Image Helper class, with some functions such as reading textures into opengl.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#ifndef _IMAGEHELPER_H
#define _IMAGEHELPER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <iostream>
#include <stdio.h>
#include <png.h>

/*
 * load_bmp
 *
 * INPUT:
 *         Filename - path to the texture file.
 *
 * RETURN:
 *         The textureID of the file used as argument.
 *
 * DESCRIPTION:
 *         This function is responsible for loading a texture into
 *         OpenGL with trilinear filtering and returning the texture ID.
 *         Reads a bmp file.
 */
GLuint load_bmp(char const* filename);

/*
 * load_png
 *
 * INPUT:
 *         Filename - path to the texture file.
 *
 * RETURN:
 *         The textureID of the file used as argument.
 *
 * DESCRIPTION:
 *         This function is responsible for loading a texture into
 *         OpenGL with trilinear filtering and returning the texture ID.
 *         Reads a png file.
 */
GLuint load_png(char const* filename);

#endif
