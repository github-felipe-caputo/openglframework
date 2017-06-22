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

//#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <iostream>

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
 *
 *         With the help of
 *         http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
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
 *
 *         In particular this function will change the png file into one with
 *         8 bits and with an alpha channel (if the image does not have one
 *         it will fill it with 0xFF).
 *
 *         With the help of
 *         https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example
 *         http://www.libpng.org/pub/png/libpng-manual.txt
 */
GLuint load_png(char const* filename);

#endif
