/*
 * shader.h
 *
 * Set up the shaders, header file.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>

namespace shader 
{

/* 
 * readFile
 *
 * INPUT: 
 *         filename - the filename of the file we want to read.
 *
 * RETURN:
 *         The contents of the the file in a dinamycally alocated
 *         string of GLchar, or NULL if there was an error. 
 *
 * DESCRIPTION:
 *         This function is responsible for reading a text file and
 *         converting the whole content into a single string. 
 *
 */
GLchar* readFile(const char* filename);

/* 
 * readFile
 *
 * INPUT: 
 *         shaderID - the ID of the shader we want the log.
 *
 *
 * DESCRIPTION:
 *         This function reports the log of the shader that was
 *         compiled.
 *
 */
void printShaderLog(GLuint shaderID);

/* 
 * makeShaderProgram
 *
 * INPUT: 
 *         vert - the filename of vertex shader.
 *         frag - the filename of fragment shader.
 *
 * RETURN:
 *         The ID of the two shaders linked together
 *
 * DESCRIPTION:
 *         This function reads the vertex shader file and the
 *         the fragment shader file, and links those together
 *         into a "program", then its ID is returned. 
 *
 */
GLuint makeShaderProgram(const char *vert, const char *frag);

} // end namespace


#endif