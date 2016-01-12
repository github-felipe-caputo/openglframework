/*
 * shape.h
 *
 * Shape class, to creat our objects
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#ifndef _SHAPE_H
#define _SHAPE_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <stdio.h>

#include <fstream>
#include <string>

#include "mathHelper.h"

using namespace std;

// Some helpful definitions
#define FLAT    0
#define SMOOTH  1
#define PI      3.14159265

/*
 * The Shader class
 */
class Shape {
    // the shape's vertices, and the number of vertices (not the same as
    // the size of the vector)
    vector<float> vertices;
    GLuint numVertices;

    // the color of each shape vertex, and the number of colors
    vector<float> colors;
    GLuint numColors;

    // the normals each shape vertex, and the number of normals
    vector<float> normals;
    GLuint numNormals;

    // the element vector of the shape, and the number of elements
    vector<GLshort> elements;
    GLuint numElements;

    // values for lighting materials
    vector<float> ambientMaterial;
    vector<float> diffuseMaterial;
    vector<float> specularMaterial;
    float Ka, Kd, Ks, specExp;

    /* 
     * addTriangle
     *
     * INPUT: 
     *         xN - the x value of the Nth point of the triangle.
     *         yN - the y value of the Nth point of the triangle.
     *         zN - the z value of the Nth point of the triangle.
     *
     * DESCRIPTION:
     *         This function gets as input three (x,y,z) points
     *         that will create a polygon (triangle). These points
     *         are required to be in counter clockwise order. This function
     *         will also calculate normals for the vertices, however every vertex
     *         of the triangle will have the same normal.
     *         The variables numVertices, numNormals andnumElements are also 
     *         changed accordingly.
     *
     */
    void addTriangle(float x0, float y0, float z0,
                     float x1, float y1, float z1,
                     float x2, float y2, float z2);

    /* 
     * addTriangle
     *
     * INPUT: 
     *         v0 - the first vertex of the triangle.
     *         v1 - the second vertex of the triangle.
     *         v2 - the third vertex of the triangle.
     *
     * DESCRIPTION:
     *         This function works exactly like the previous addTriangle, 
     *         with the exception that instead of using as arguments the x, y,
     *         and z of each vertex, we use three arrays of size 3 that hold
     *         those values. In other words, v0[3] = {x, y, z}.
     *
     */
    void addTriangle(float v0[], float v1[], float v2[]);

    /* 
     * addTriangleAndNormals
     *
     * INPUT: 
     *         xN - the x value of the Nth point of the triangle.
     *         yN - the y value of the Nth point of the triangle.
     *         zN - the z value of the Nth point of the triangle.
     *         nxN - the x value of the normal of the Nth point of the triangle.
     *         nyN - the y value of the normal of the Nth point of the triangle.
     *         nzN - the z value of the normal of the Nth point of the triangle.
     *
     * DESCRIPTION:
     *         This function works just like addTriangle, but with it you can also
     *         tell the normals of each vertex.
     *
     */
    void addTriangleAndNormals(float x0, float y0, float z0,
                               float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float nx0, float ny0, float nz0,
                               float nx1, float ny1, float nz1,
                               float nx2, float ny2, float nz2);

    /* 
     * addTriangleAndNormals
     *
     * INPUT: 
     *         v0 - the first vertex of the triangle.
     *         v1 - the second vertex of the triangle.
     *         v2 - the third vertex of the triangle.
     *         n0 - the normal of the first vertex of the triangle.
     *         n1 - the normal of the second vertex of the triangle.
     *         n2 - the normal of the third vertex of the triangle.
     *
     * DESCRIPTION:
     *         This function works exactly like the previous addTriangleAndNormals, 
     *         with the exception that instead of using as arguments the x, y,
     *         and z of each vertex, we use three arrays of size 3 that hold
     *         those values. In other words, v0[3] = {x, y, z}.
     *
     */
    void addTriangleAndNormals(float v0[], float v1[], float v2[],
                               float n0[], float n1[], float n2[]);

    /* 
     * addTriangleWithSubdivision
     *
     * INPUT: 
     *         v0 - the first vertex of the triangle.
     *         v1 - the second vertex of the triangle.
     *         v2 - the third vertex of the triangle.
     *         subDiv - number of subdivisions for the triangles.
     *
     * DESCRIPTION:
     *         This function is used together with the makeSphere function.
     *         It's responsible for subdividing the triangles of the sphere so that
     *         we get a better result. Essentially the triangle "0, 1, 2" will 
     *         become triangles "0,3,5", "3,1,4", "5,4,2" and "3,4,5".
     *
     *                      0
     *                  3       5
     *              1       4       2
     *
     *         It's a recursive function so it can keep sub dividing the triangles.
     *         This function will also be different depending on the type of
     *         normals. If you want the FLAT normals, then for each triangles, 
     *         the vertices will have the same normals. If you want the SMOOTH normals
     *         then another function will be called where we tell it what 
     *         are the normals, in the case of the sphere you can consider the normals
     *         of the points as the same as the points (they are vertices coming from
     *         the origin).
     *
     */
    void addTriangleWithSubdivision(float v0[], float v1[], float v2[], int subDiv, int normalType);

    

public:
    
    /* 
     * Shape
     *
     * DESCRIPTION:
     *         Default shape constructor. All it does is initialize some
     *         variables to 0 such as the number of vertices of the shape.
     *
     */
    Shape ();

    /* 
     * ~Shape
     *
     * DESCRIPTION:
     *         Default descontructor
     *
     */
    ~Shape ();

    /* 
     * clearShape
     *
     * DESCRIPTION:
     *         Clears every variable from the shape object.
     *
     */
    void clearShape ();

    /* 
     * getVertices
     *
     * RETURN:
     *         The vector/array of the vertices.
     *
     */
    float* getVertices();

    /* 
     * getNumVertices
     *
     * RETURN:
     *         The number of the vertices.
     *
     */
    GLuint getNumVertices();

    /* 
     * getColors
     *
     * RETURN:
     *         The vector/array of colors.
     *
     */
    float* getColors();

    /* 
     * getNumColors
     *
     * RETURN:
     *         The number of the colors.
     *
     */
    GLuint getNumColors();

    /* 
     * getNormals
     *
     * RETURN:
     *         The vector/array of the normals.
     *
     */
    float* getNormals();

    /* 
     * getNumNormals
     *
     * RETURN:
     *         The number of the normals.
     *
     */
    GLuint getNumNormals();

    /* 
     * getElements
     *
     * RETURN:
     *         The vector/array of elements.
     *
     */
    GLshort* getElements();

    /* 
     * getNumElements
     *
     * RETURN:
     *         The number of the elements.
     *
     */
    GLuint getNumElements();

    /* 
     * setMaterials
     *
     * INPUT: 
     *         newAmbientMaterial - RGB values for the ambient material.
     *         newKa - ambient coefficient.
     *         newDiffuseMaterial - RGB values for the diffuse material.
     *         newKd - diffuse coefficient.
     *         newSpecularMaterial - RGB values for the specular material.
     *         newKs - specular coefficient.
     *         newSpecExp - specular expoent.
     *
     * DESCRIPTION:
     *         This function will set up the ambient, diffuse and specular materials
     *         for this object so it can be used with the Phong illumination model.
     *
     */
    void setMaterials(float newAmbientMaterial[], float newKa,
                      float newDiffuseMaterial[], float newKd,
                      float newSpecularMaterial[], float newKs, float newSpecExp);

    /* 
     * setMaterials
     *
     * INPUT: 
     *         ambientX - RGB values for the ambient material.
     *         newKa - ambient coefficient.
     *         diffuseX - RGB values for the diffuse material.
     *         newKd - diffuse coefficient.
     *         specularX - RGB values for the specular material.
     *         newKs - specular coefficient.
     *         newSpecExp - specular expoent.
     *
     * DESCRIPTION:
     *         This function will set up the ambient, diffuse and specular materials
     *         for this object so it can be used with the Phong illumination model.
     *         It works just like the above function, however here you can use as
     *         arguments unit values instead of arrays.
     *
     */
    void setMaterials(float ambientR, float ambientG, float ambientB, float newKa,
                      float diffuseR, float diffuseG, float diffuseB, float newKd,
                      float specularR, float specularG, float specularB, float newKs, float newSpecExp);

    /* 
     * getAmbientMaterial
     *
     * RETURN:
     *         The vector/array of ambient material values.
     *
     */
    float* getAmbientMaterial();

    /* 
     * getDiffuseMaterial
     *
     * RETURN:
     *         The vector/array of diffuse material values.
     *
     */
    float* getDiffuseMaterial();

    /* 
     * getSpecularMaterial
     *
     * RETURN:
     *         The vector/array of specular material values.
     *
     */
    float* getSpecularMaterial();

    /* 
     * getKa
     *
     * RETURN:
     *         The ambient coefficient.
     *
     */
    float getKa();

    /* 
     * getKd
     *
     * RETURN:
     *         The diffuse coefficient.
     *
     */
    float getKd();

    /* 
     * getKs
     *
     * RETURN:
     *         The specular coefficient.
     *
     */
    float getKs();

    /* 
     * getSpecExp
     *
     * RETURN:
     *         The specular expoent.
     *
     */
    float getSpecExp();

    /* 
     * makeCube
     *
     * INPUT: 
     *         subDiv - number of subdivisions for the squares.
     *
     * DESCRIPTION:
     *         Creates a simple cube centered at the origin, with each
     *         side of total length 1, and squares with certain 
     *         sub divisions.
     *
     */
    void makeCube (int subDiv);

    /* 
     * makeSphere
     *
     * INPUT: 
     *         subDivBase - number of subdivisions on the bases.
     *         subDivHeight - number of subdivisions on height.
     *         normalType - this is the type of normal the sphere can have.
     *
     * DESCRIPTION:
     *         This function creates a cylinder tesselation with different
     *         subdivisions for the bases and for the height. It's also possible
     *         to choose flat or smooth normals.
     *
     */
    void makeCylinder ( int subDivBase, int subDivHeight, int normalType );

    /* 
     * makeSphere
     *
     * INPUT: 
     *         subDiv - number of subdivisions for the triangles.
     *         normalType - this is the type of normal the sphere can have.
     *
     * DESCRIPTION:
     *         This function creates a sphere tesselation by icosahedron
     *         subdivision. It can also have two types of normals, FLAT or
     *         SMOOTH.
     *
     *         Reference: 
     *         Hoffmann, Gernot. Sphere Tesselation by Icosahedron Subdivision. 
     *         http://docs-hoffmann.de/ikos27042002.pdf
     *
     */
    void makeSphere ( int subDiv, int normalType );

    void fromObj ( char* filename );
};

#endif