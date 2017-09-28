/*
 * shape.cpp
 *
 * Shape class, to creat our objects
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#include "shape.h"

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
void Shape::addTriangle(float x0, float y0, float z0,
                        float x1, float y1, float z1,
                        float x2, float y2, float z2){

    // Vertices
    vertices.push_back(x0);
    vertices.push_back(y0);
    vertices.push_back(z0);

    vertices.push_back(x1);
    vertices.push_back(y1);
    vertices.push_back(z1);

    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(z2);

    // Normals
    float vec1[] = {x1 - x0, y1 - y0, z1 - z0};
    float vec2[] = {x2 - x0, y2 - y0, z2 - z0};

    float nx = (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]);
    float ny = (vec1[2] * vec2[0]) - (vec1[0] * vec2[2]);
    float nz = (vec1[0] * vec2[1]) - (vec1[1] * vec2[0]);

    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);

    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);

    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

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
void Shape::addTriangle(float v0[], float v1[], float v2[]) {
    // Vertices
    vertices.push_back(v0[0]);
    vertices.push_back(v0[1]);
    vertices.push_back(v0[2]);

    vertices.push_back(v1[0]);
    vertices.push_back(v1[1]);
    vertices.push_back(v1[2]);

    vertices.push_back(v2[0]);
    vertices.push_back(v2[1]);
    vertices.push_back(v2[2]);

    // Normals
    float vec1[] = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
    float vec2[] = {v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]};

    float nx = (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]);
    float ny = (vec1[2] * vec2[0]) - (vec1[0] * vec2[2]);
    float nz = (vec1[0] * vec2[1]) - (vec1[1] * vec2[0]);

    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);

    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);

    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

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
void Shape::addTriangleAndNormals(float x0, float y0, float z0,
                                  float x1, float y1, float z1,
                                  float x2, float y2, float z2,
                                  float nx0, float ny0, float nz0,
                                  float nx1, float ny1, float nz1,
                                  float nx2, float ny2, float nz2){

    vertices.push_back(x0);
    vertices.push_back(y0);
    vertices.push_back(z0);

    vertices.push_back(x1);
    vertices.push_back(y1);
    vertices.push_back(z1);

    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(z2);

    normals.push_back(nx0);
    normals.push_back(ny0);
    normals.push_back(nz0);

    normals.push_back(nx1);
    normals.push_back(ny1);
    normals.push_back(nz1);

    normals.push_back(nx2);
    normals.push_back(ny2);
    normals.push_back(nz2);
}

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
void Shape::addTriangleAndNormals(float v0[], float v1[], float v2[],
                                  float n0[], float n1[], float n2[]){

    // Vertices
    vertices.push_back(v0[0]);
    vertices.push_back(v0[1]);
    vertices.push_back(v0[2]);

    vertices.push_back(v1[0]);
    vertices.push_back(v1[1]);
    vertices.push_back(v1[2]);

    vertices.push_back(v2[0]);
    vertices.push_back(v2[1]);
    vertices.push_back(v2[2]);

    normals.push_back(n0[0]);
    normals.push_back(n0[1]);
    normals.push_back(n0[2]);

    normals.push_back(n1[0]);
    normals.push_back(n1[1]);
    normals.push_back(n1[2]);

    normals.push_back(n2[0]);
    normals.push_back(n2[1]);
    normals.push_back(n2[2]);
}

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
void Shape::addTriangleWithSubdivision(float v0[], float v1[], float v2[], int subDiv, int normalType) {
    float v3[] = {v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2]};
    float v4[] = {v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]};
    float v5[] = {v2[0] + v0[0], v2[1] + v0[1], v2[2] + v0[2]};

    normalize(v3);
    normalize(v4);
    normalize(v5);

    if (subDiv == 1) {
        if (normalType == FLAT) {
            addTriangle(v0,v3,v5);
            addTriangle(v3,v1,v4);
            addTriangle(v5,v4,v2);
            addTriangle(v3,v4,v5);
        } else {
            addTriangleAndNormals(v0,v3,v5,v0,v3,v5);
            addTriangleAndNormals(v3,v1,v4,v3,v1,v4);
            addTriangleAndNormals(v5,v4,v2,v5,v4,v2);
            addTriangleAndNormals(v3,v4,v5,v3,v4,v5);
        }
    } else {
        subDiv--;
        addTriangleWithSubdivision(v0,v3,v5,subDiv,normalType);
        addTriangleWithSubdivision(v3,v1,v4,subDiv,normalType);
        addTriangleWithSubdivision(v5,v4,v2,subDiv,normalType);
        addTriangleWithSubdivision(v3,v4,v5,subDiv,normalType);
    }
}

/*
 * Shape
 *
 * DESCRIPTION:
 *         Default shape constructor. All it does is initialize some
 *         variables to 0 such as the number of vertices of the shape.
 *
 */
Shape::Shape () : numVertices(0), numColors(0), numNormals(0), numElements(0) {
}

/*
 * ~Shape
 *
 * DESCRIPTION:
 *         Default descontructor
 *
 */
Shape::~Shape () {
}

/*
 * clearShape
 *
 * DESCRIPTION:
 *         Clears every variable from the shape object.
 *
 */
void Shape::clearShape () {
    vertices.clear();
    numVertices = 0;

    colors.clear();
    numColors = 0;

    normals.clear();
    numNormals = 0;

    elements.clear();
    numElements = 0;

    ambientMaterial.clear();
    diffuseMaterial.clear();
    specularMaterial.clear();
    Ka = 0;
    Kd = 0;
    Ks = 0;
    specExp = 0;
}

/*
 * getVertices
 *
 * RETURN:
 *         The vector/array of the vertices.
 *
 */
float* Shape::getVertices() {
    return &vertices[0];
}

/*
 * getNumVertices
 *
 * RETURN:
 *         The number of the vertices.
 *
 */
GLuint Shape::getNumVertices() {
    return numVertices;
}

/*
 * getColors
 *
 * RETURN:
 *         The vector/array of colors.
 *
 */
float* Shape::getColors() {
    return &colors[0];
}

/*
 * getNumColors
 *
 * RETURN:
 *         The number of the colors.
 *
 */
GLuint Shape::getNumColors() {
    return numColors;
}

/*
 * getUV
 *
 * RETURN:
 *         The vector/array of the texture coordinates.
 *
 */
float* Shape::getUV() {
    return &uvtextures[0];
}

/*
 * getNumUV
 *
 * RETURN:
 *         The number of the texture coordinates.
 *
 */
GLuint Shape::getNumUV() {
    return numTextures;
}

/*
* getDiffTextureID
*
* RETURN:
*         The ID of the diffuse texture map.
*
*/
GLuint Shape::getDiffTextureID(){
    return textureDiffMapID;
}
     
/*
* getSpecTextureID
*
* RETURN:
*         The ID of the specular texture map.
*
*/
GLuint Shape::getSpecTextureID(){
    return textureSpecMapID;
}

/*
 * getNormals
 *
 * RETURN:
 *         The vector/array of the normals.
 *
 */
float* Shape::getNormals() {
    return &normals[0];
}

/*
 * getNumNormals
 *
 * RETURN:
 *         The number of the normals.
 *
 */
GLuint Shape::getNumNormals() {
    return numNormals;
}

/*
 * getElements
 *
 * RETURN:
 *         The vector/array of elements.
 *
 */
GLshort* Shape::getElements() {
    return &elements[0];
}

/*
 * getNumElements
 *
 * RETURN:
 *         The number of the elements.
 *
 */
GLuint Shape::getNumElements() {
    return numElements;
}

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
void Shape::setMaterials(float newAmbientMaterial[], float newKa,
                         float newDiffuseMaterial[], float newKd,
                         float newSpecularMaterial[], float newKs, float newSpecExp) {

    ambientMaterial.clear();
    diffuseMaterial.clear();
    specularMaterial.clear();

    for (int i = 0; i < 3; ++i) {
        ambientMaterial.push_back(newAmbientMaterial[i]);
        diffuseMaterial.push_back(newDiffuseMaterial[i]);
        specularMaterial.push_back(newSpecularMaterial[i]);
    }

    Ka = newKa;
    Kd = newKd;
    Ks = newKs;
    specExp = newSpecExp;
}

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
void Shape::setMaterials(float ambientR, float ambientG, float ambientB, float newKa,
                         float diffuseR, float diffuseG, float diffuseB, float newKd,
                         float specularR, float specularG, float specularB, float newKs, float newSpecExp) {

    ambientMaterial.clear();
    diffuseMaterial.clear();
    specularMaterial.clear();

    ambientMaterial.push_back(ambientR);
    ambientMaterial.push_back(ambientG);
    ambientMaterial.push_back(ambientB);

    diffuseMaterial.push_back(diffuseR);
    diffuseMaterial.push_back(diffuseG);
    diffuseMaterial.push_back(diffuseB);

    specularMaterial.push_back(specularR);
    specularMaterial.push_back(specularG);
    specularMaterial.push_back(specularB);

    Ka = newKa;
    Kd = newKd;
    Ks = newKs;
    specExp = newSpecExp;
}

/*
 * getAmbientMaterial
 *
 * RETURN:
 *         The vector/array of ambient material values.
 *
 */
float* Shape::getAmbientMaterial() {
    return &ambientMaterial[0];
}

/*
 * getDiffuseMaterial
 *
 * RETURN:
 *         The vector/array of diffuse material values.
 *
 */
float* Shape::getDiffuseMaterial() {
    return &diffuseMaterial[0];
}

/*
 * getSpecularMaterial
 *
 * RETURN:
 *         The vector/array of specular material values.
 *
 */
float* Shape::getSpecularMaterial() {
    return &specularMaterial[0];
}

/*
 * getKa
 *
 * RETURN:
 *         The ambient coefficient.
 *
 */
float Shape::getKa() {
    return Ka;
}

/*
 * getKd
 *
 * RETURN:
 *         The diffuse coefficient.
 *
 */
float Shape::getKd() {
    return Kd;
}

/*
 * getKs
 *
 * RETURN:
 *         The specular coefficient.
 *
 */
float Shape::getKs() {
    return Ks;
}

/*
 * getSpecExp
 *
 * RETURN:
 *         The specular expoent.
 *
 */
float Shape::getSpecExp() {
    return specExp;
}

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
void Shape::makeCube (int subDiv) {
    if (subDiv < 1) {
        subDiv = 1;
    }

    // Length of each side depends on the sub division
    float side = 1.0f/subDiv;

    // "Anchor" points to start the cube subdivisions
    float p1[] = {-0.5, 0.5, 0.5};
    float p2[] = { 0.5, 0.5,-0.5};
    float p3[] = { 0.5, 0.5, 0.5};
    float p4[] = {-0.5, 0.5,-0.5};
    float p5[] = {-0.5,-0.5, 0.5};

    for(int i = 0; i < subDiv; ++i) {
        for(int j = 0; j < subDiv; ++j) {
            // Front (z = 0.5)
            addTriangle(p1[0]+(i*side)    , p1[1]-(j*side)    , p1[2],
                        p1[0]+(i*side)    , p1[1]-((j+1)*side), p1[2],
                        p1[0]+((i+1)*side), p1[1]-(j*side)    , p1[2]);

            addTriangle(p1[0]+(i*side)    , p1[1]-((j+1)*side), p1[2],
                        p1[0]+((i+1)*side), p1[1]-((j+1)*side), p1[2],
                        p1[0]+((i+1)*side), p1[1]-(j*side)    , p1[2]);

            // Back (z = -0.5)
            addTriangle(p2[0]-(i*side)    , p2[1]-(j*side)    , p2[2],
                        p2[0]-(i*side)    , p2[1]-((j+1)*side), p2[2],
                        p2[0]-((i+1)*side), p2[1]-(j*side)    , p2[2]);

            addTriangle(p2[0]-(i*side)    , p2[1]-((j+1)*side), p2[2],
                        p2[0]-((i+1)*side), p2[1]-((j+1)*side), p2[2],
                        p2[0]-((i+1)*side), p2[1]-(j*side)    , p2[2]);

            // Right (x = 0.5)
            addTriangle(p3[0], p3[1]-(j*side)    , p3[2]-(i*side),
                        p3[0], p3[1]-((j+1)*side), p3[2]-(i*side),
                        p3[0], p3[1]-(j*side)    , p3[2]-((i+1)*side));

            addTriangle(p3[0], p3[1]-((j+1)*side), p3[2]-(i*side),
                        p3[0], p3[1]-((j+1)*side), p3[2]-((i+1)*side),
                        p3[0], p3[1]-(j*side)    , p3[2]-((i+1)*side));

            // Left (x = -0.5)
            addTriangle(p4[0], p4[1]-(j*side)    , p4[2]+(i*side),
                        p4[0], p4[1]-((j+1)*side), p4[2]+(i*side),
                        p4[0], p4[1]-(j*side)    , p4[2]+((i+1)*side));

            addTriangle(p4[0], p4[1]-((j+1)*side), p4[2]+(i*side),
                        p4[0], p4[1]-((j+1)*side), p4[2]+((i+1)*side),
                        p4[0], p4[1]-(j*side)    , p4[2]+((i+1)*side));

            // Top (y = 0.5) (obs: yep, same point as p4)
            addTriangle(p4[0]+(i*side)    , p4[1], p4[2]+(j*side),
                        p4[0]+(i*side)    , p4[1], p4[2]+((j+1)*side),
                        p4[0]+((i+1)*side), p4[1], p4[2]+(j*side));

            addTriangle(p4[0]+(i*side)    , p4[1], p4[2]+((j+1)*side),
                        p4[0]+((i+1)*side), p4[1], p4[2]+((j+1)*side),
                        p4[0]+((i+1)*side), p4[1], p4[2]+(j*side));

            // Bottom (y = -0.5)
            addTriangle(p5[0]+(i*side)    , p5[1], p5[2]-(j*side),
                        p5[0]+(i*side)    , p5[1], p5[2]-((j+1)*side),
                        p5[0]+((i+1)*side), p5[1], p5[2]-(j*side));

            addTriangle(p5[0]+(i*side)    , p5[1], p5[2]-((j+1)*side),
                        p5[0]+((i+1)*side), p5[1], p5[2]-((j+1)*side),
                        p5[0]+((i+1)*side), p5[1], p5[2]-(j*side));

        }
    }

    numVertices = vertices.size()/3;
    numNormals = normals.size()/3;
    numElements = numVertices; // in this case elements is the same size as vertices

    // Elements
    for(int i = 0; i < numElements; ++i){
        elements.push_back(i);
    }
}

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
void Shape::makeCylinder ( int subDivBase, int subDivHeight, int normalType ) {
    if(subDivBase < 3) {
        subDivBase = 3;
    }
    if(subDivHeight < 1) {
        subDivHeight = 3;
    }

    // Data
    float theta = (2*PI)/subDivBase;
    float side = 1.0f/subDivHeight;
    float r = 0.5f;

    // Top and bottom center points
    float top[] = { 0.0, 0.5, 0.0};
    float bot[] = { 0.0,-0.5, 0.0};

    // Somne aux points
    float p1[3];
    float p2[3];

    if(normalType == FLAT) {
        for (int i = 0; i < subDivBase; ++i) {
            p1[0] = r * cos((i+1) * theta);
            p1[2] = r * sin((i+1) * theta);

            p2[0] = r * cos(i * theta);
            p2[2] = r * sin(i * theta);

            // Bottom
            p1[1] = -0.5f;
            p2[1] = -0.5f;
            addTriangle(bot, p2, p1);

            // Top
            p1[1] = 0.5f;
            p2[1] = 0.5f;
            addTriangle(top, p1, p2);

            // Sides
            for (int j = 0; j < subDivHeight; ++j) {
                addTriangle(p1[0], p1[1]-(j*side)    , p1[2],
                            p1[0], p1[1]-((j+1)*side), p1[2],
                            p2[0], p2[1]-(j*side)    , p2[2]);

                addTriangle(p1[0], p1[1]-((j+1)*side), p1[2],
                            p2[0], p2[1]-((j+1)*side), p2[2],
                            p2[0], p2[1]-(j*side)    , p2[2]);
            }
        }
    } else {
        for (int i = 0; i < subDivBase; ++i) {
            p1[0] = r * cos((i+1) * theta);
            p1[2] = r * sin((i+1) * theta);

            p2[0] = r * cos(i * theta);
            p2[2] = r * sin(i * theta);

            // Bottom
            p1[1] = -0.5f;
            p2[1] = -0.5f;
            addTriangle(bot, p2, p1);

            // Top
            p1[1] = 0.5f;
            p2[1] = 0.5f;
            addTriangle(top, p1, p2);

            // Sides
            for (int j = 0; j < subDivHeight; ++j) {
                addTriangleAndNormals(p1[0], p1[1]-(j*side)    , p1[2],
                                      p1[0], p1[1]-((j+1)*side), p1[2],
                                      p2[0], p2[1]-(j*side)    , p2[2],
                                      p1[0], 0                 , p1[2],
                                      p1[0], 0                 , p1[2],
                                      p2[0], 0                 , p2[2]);

                addTriangleAndNormals(p1[0], p1[1]-((j+1)*side), p1[2],
                                      p2[0], p2[1]-((j+1)*side), p2[2],
                                      p2[0], p2[1]-(j*side)    , p2[2],
                                      p1[0], 0                 , p1[2],
                                      p2[0], 0                 , p2[2],
                                      p2[0], 0                 , p2[2]);
            }
        }
    }

    numVertices = vertices.size()/3;
    numNormals = normals.size()/3;
    numElements = numVertices; // in this case elements is the same size as vertices

    // Elements
    for(int i = 0; i < numElements; ++i){
        elements.push_back(i);
    }
}

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
void Shape::makeSphere ( int subDiv, int normalType ) {
    if (subDiv < 1) {
        subDiv = 1;
    }

    // Icosahedron vertices
    float vert[12][3];

    // Needed values
    float theta = 26.565 * PI / 180.0f;
    float sth = sin(theta);
    float cth = cos(theta);
    float dps = 2 * PI / 5.0f;

    // top vertex
    vert[0][0] = 0;
    vert[0][1] = 0;
    vert[0][2] = 1;

    // upper pentagon
    float psi = 0;
    for( int i = 1; i < 6; ++i ) {
        vert[i][0] = cth * cos(psi);
        vert[i][1] = cth * sin(psi);
        vert[i][2] = sth;

        psi += dps;
    }

    // lower pentagon
    psi = PI / 5.0f;
    for( int i = 6; i < 11; ++i ) {
        vert[i][0] = cth * cos(psi);
        vert[i][1] = cth * sin(psi);
        vert[i][2] = -sth;

        psi += dps;
    }

    // lower vertex
    vert[11][0] = 0;
    vert[11][1] = 0;
    vert[11][2] = -1;

    // Creating the triangles
    addTriangleWithSubdivision(vert[1], vert[2], vert[0], subDiv, normalType);
    addTriangleWithSubdivision(vert[2], vert[3], vert[0], subDiv, normalType);
    addTriangleWithSubdivision(vert[3], vert[4], vert[0], subDiv, normalType);
    addTriangleWithSubdivision(vert[4], vert[5], vert[0], subDiv, normalType);
    addTriangleWithSubdivision(vert[5], vert[1], vert[0], subDiv, normalType);

    addTriangleWithSubdivision(vert[6], vert[2], vert[1], subDiv, normalType);
    addTriangleWithSubdivision(vert[6], vert[7], vert[2], subDiv, normalType);
    addTriangleWithSubdivision(vert[7], vert[3], vert[2], subDiv, normalType);
    addTriangleWithSubdivision(vert[7], vert[8], vert[3], subDiv, normalType);
    addTriangleWithSubdivision(vert[8], vert[4], vert[3], subDiv, normalType);
    addTriangleWithSubdivision(vert[8], vert[9], vert[4], subDiv, normalType);
    addTriangleWithSubdivision(vert[9], vert[5], vert[4], subDiv, normalType);
    addTriangleWithSubdivision(vert[9], vert[10], vert[5], subDiv, normalType);
    addTriangleWithSubdivision(vert[10], vert[1], vert[5], subDiv, normalType);
    addTriangleWithSubdivision(vert[10], vert[6], vert[1], subDiv, normalType);

    addTriangleWithSubdivision(vert[11], vert[7], vert[6], subDiv, normalType);
    addTriangleWithSubdivision(vert[11], vert[8], vert[7], subDiv, normalType);
    addTriangleWithSubdivision(vert[11], vert[9], vert[8], subDiv, normalType);
    addTriangleWithSubdivision(vert[11], vert[10], vert[9], subDiv, normalType);
    addTriangleWithSubdivision(vert[11], vert[6], vert[10], subDiv, normalType);

    numVertices = vertices.size()/3;
    numNormals = normals.size()/3;
    numElements = numVertices; // in this case elements is the same size as vertices

    // Elements
    for(int i = 0; i < numElements; ++i){
        elements.push_back(i);
    }
}

/*
 * readObjVert
 *
 * INPUT:
 *         filename - the .obj file you want to load
 *
 * DESCRIPTION:
 *         This function reads an .obj file and creates the geometry for the
 *         object it describes. In particular this function will only work
 *         for .objs that have only vertex and face data of order 3.
 *
 */
void Shape::readObjVert ( char* filename ) {
    std::ifstream ifs(filename, std::ifstream::in);
    std::string line, firstWord;
    float values[3];

    while(std::getline(ifs, line)) {
        if(!line.empty()) {
            std::istringstream ss(line);
            ss >> firstWord >> values[0] >> values[1] >> values[2];

            // vertices
            if (!firstWord.compare("v")) {
                vertices.push_back(values[0]);
                vertices.push_back(values[1]);
                vertices.push_back(values[2]);
            }  // faces
            else if (!firstWord.compare("f")) {
                // Wavefront .obj files start counting from 1
                elements.push_back((int) values[0]-1);
                elements.push_back((int) values[1]-1);
                elements.push_back((int) values[2]-1);
            }
        }
        firstWord.clear();
    }

    numVertices = vertices.size()/3;
    numElements = elements.size();
}

/*
 * readObjVertNorm
 *
 * INPUT:
 *         filename - the .obj file you want to load
 *
 * DESCRIPTION:
 *         This function reads an .obj file and creates the geometry for the
 *         object it describes. In particular this function will only work
 *         for .objs that have vertex, normals and face data of order 3 with
 *         the pattern "number//number"
 *
 */
void Shape::readObjVertNorm ( char* filename ) {
    std::ifstream ifs(filename, std::ifstream::in);
    std::string line, firstWord, aux, aux2;;
    float values[3];

    // aux values
    vector<float> in_vertices;
    vector<float> in_uvtextures;
    vector<float> in_normals;
    vector<float> in_elements;

    while(std::getline(ifs, line)) {
        if(!line.empty()) {
            std::istringstream ss(line);
            ss >> firstWord;// >> values[0] >> values[1] >> values[2];

            // vertices
            if (!firstWord.compare("v")) {
                ss >> values[0] >> values[1] >> values[2];
                in_vertices.push_back(values[0]);
                in_vertices.push_back(values[1]);
                in_vertices.push_back(values[2]);
            } // textures
            else if (!firstWord.compare("vt")) {
                ss >> values[0] >> values[1];
                in_uvtextures.push_back(values[0]);
                in_uvtextures.push_back(values[1]);
            } // normals
            else if (!firstWord.compare("vn")) {
                ss >> values[0] >> values[1] >> values[2];
                in_normals.push_back(values[0]);
                in_normals.push_back(values[1]);
                in_normals.push_back(values[2]);
            } // faces
            else if (!firstWord.compare("f")) {
                while(ss >> aux) { // aux is like "1//3"
                    std::istringstream ss2(aux);
                    while(std::getline(ss2, aux2, '/')) {
                        if(!aux2.empty()) {
                            in_elements.push_back( atoi(aux2.c_str()) - 1); // Wavefront .obj files start counting from 1
                        }
                    }
                }
            }
        }
        firstWord.clear();
    }

    // Filling the index the correct way, we make a map of the faces. IF we see "1/2/3" for the
    // first time we add on the final "elements" vector, if we see it again we check on this
    // map what is the index of it, then we add the same index on "elements" again
    std::map<vector<int>,int> facesValues;
    int index = 0;

    // we will see each input vertex, so we will for every 3 values on our in_vertices vector
    for(int i = 0; i < in_elements.size(); i+=2) {
        vector<int> auxVec;
        auxVec.push_back(in_elements[i]);
        auxVec.push_back(in_elements[i+1]);

        // if "1//3" is not on the map
        if(facesValues.find(auxVec) == facesValues.end()){
            // lets put the values on the actual vectors we will output
            int vectorIndex = in_elements[i]*3;
            int normalIndex = in_elements[i+1]*3;

            vertices.push_back(in_vertices[vectorIndex]);
            vertices.push_back(in_vertices[vectorIndex+1]);
            vertices.push_back(in_vertices[vectorIndex+2]);

            normals.push_back(in_normals[normalIndex]);
            normals.push_back(in_normals[normalIndex+1]);
            normals.push_back(in_normals[normalIndex+2]);

            index = (vertices.size()/3) - 1;
            elements.push_back(index);
            facesValues.insert( std::pair<vector<int>,int> (auxVec,index) );
        } // if it is on the map, just add it again
        else {
            elements.push_back(facesValues.find(auxVec)->second);
        }
    }

    numVertices = vertices.size()/3;
    numNormals = normals.size()/3;
    numElements = elements.size();
}

/*
 * readObjVertNorm
 *
 * INPUT:
 *         filename - the .obj file you want to load
 *         filetexture - the texture file, can be of any extension readable
 *                       by SOIL
 *
 * DESCRIPTION:
 *         This function reads an .obj file and creates the geometry for the
 *         object it describes. In particular this function will only work
 *         for .objs that have vertex, texture, normals and face data of
 *         order 3 with the pattern "number/number/number"
 *
 */
void Shape::readObjVertTexNorm ( char* filename , char* filetexture ) {
    std::ifstream ifs(filename, std::ifstream::in);
    std::string line, firstWord, aux, aux2;
    float values[3];

    // aux values
    vector<float> in_vertices;
    vector<float> in_uvtextures;
    vector<float> in_normals;
    vector<float> in_elements;

    if (!ifs.is_open()) {
        fprintf(stderr, "error while opening file %s \n", filename);
    }

    // Reading the file
    while(std::getline(ifs, line)) {
        if(!line.empty()) {
            std::istringstream ss(line);
            ss >> firstWord;// >> values[0] >> values[1] >> values[2];

            // vertices
            if (!firstWord.compare("v")) {
                ss >> values[0] >> values[1] >> values[2];
                in_vertices.push_back(values[0]);
                in_vertices.push_back(values[1]);
                in_vertices.push_back(values[2]);
            } // textures
            else if (!firstWord.compare("vt")) {
                ss >> values[0] >> values[1];
                in_uvtextures.push_back(values[0]);
                in_uvtextures.push_back(values[1]);
            } // normals
            else if (!firstWord.compare("vn")) {
                ss >> values[0] >> values[1] >> values[2];
                in_normals.push_back(values[0]);
                in_normals.push_back(values[1]);
                in_normals.push_back(values[2]);
            } // faces
            else if (!firstWord.compare("f")) {
                while(ss >> aux) { // aux is like "1/2/3"
                    std::istringstream ss2(aux);
                    while(std::getline(ss2, aux2, '/')) {
                        in_elements.push_back( atoi(aux2.c_str()) - 1); // Wavefront .obj files start counting from 1
                    }
                }
            }
        }
        firstWord.clear();
    }

    // Filling the index the correct way, we make a map of the faces. IF we see "1/2/3" for the
    // first time we add on the final "elements" vector, if we see it again we check on this
    // map what is the index of it, then we add the same index on "elements" again
    std::map<vector<int>,int> facesValues;
    int index = 0;

    // we will see each input vertex, so we will for every 3 values on our in_vertices vector
    for(int i = 0; i < in_elements.size(); i+=3) {
        vector<int> auxVec;
        auxVec.push_back(in_elements[i]);
        auxVec.push_back(in_elements[i+1]);
        auxVec.push_back(in_elements[i+2]);

        // if "1/2/3" is not on the map
        if(facesValues.find(auxVec) == facesValues.end()){
            // lets put the values on the actual vectors we will output
            int vectorIndex = in_elements[i]*3;
            int textureIndex = in_elements[i+1]*2;
            int normalIndex = in_elements[i+2]*3;

            vertices.push_back(in_vertices[vectorIndex]);
            vertices.push_back(in_vertices[vectorIndex+1]);
            vertices.push_back(in_vertices[vectorIndex+2]);

            uvtextures.push_back(in_uvtextures[textureIndex]);
            uvtextures.push_back(in_uvtextures[textureIndex+1]);

            normals.push_back(in_normals[normalIndex]);
            normals.push_back(in_normals[normalIndex+1]);
            normals.push_back(in_normals[normalIndex+2]);

            index = (vertices.size()/3) - 1;
            elements.push_back(index);
            facesValues.insert( std::pair<vector<int>,int> (auxVec,index) );
        } // if it is on the map, just add it again
        else {
            elements.push_back(facesValues.find(auxVec)->second);
        }
    }

    numVertices = vertices.size()/3;
    numTextures = uvtextures.size()/2;
    numNormals = normals.size()/3;
    numElements = elements.size();

    // Now, reading the texture using SOIL directly as a new OpenGL texture
    //textureID = load_bmp(filetexture);
    textureID = load_png(filetexture);

    // check for an error during the load process
    if( 0 == textureID ) {
        printf( "Error loading texture\n" );
    }
}

/*
 * readObjLightMap
 *
 * INPUT:
 *         filename - the .obj file you want to load
 *         filetexture - the texture file, can be of any extension readable
 *                       by SOIL
 *
 * DESCRIPTION:
 *         -----
 *
 */
 void Shape::readObjLightMap ( char* filename , char* filetextureDiff, char* filetextureSpec ) {
    std::ifstream ifs(filename, std::ifstream::in);
    std::string line, firstWord, aux, aux2;
    float values[3];

    // aux values
    vector<float> in_vertices;
    vector<float> in_uvtextures;
    vector<float> in_normals;
    vector<float> in_elements;

    if (!ifs.is_open()) {
        fprintf(stderr, "error while opening file %s \n", filename);
    }

    // Reading the file
    while(std::getline(ifs, line)) {
        if(!line.empty()) {
            std::istringstream ss(line);
            ss >> firstWord;// >> values[0] >> values[1] >> values[2];

            // vertices
            if (!firstWord.compare("v")) {
                ss >> values[0] >> values[1] >> values[2];
                in_vertices.push_back(values[0]);
                in_vertices.push_back(values[1]);
                in_vertices.push_back(values[2]);
            } // textures
            else if (!firstWord.compare("vt")) {
                ss >> values[0] >> values[1];
                in_uvtextures.push_back(values[0]);
                in_uvtextures.push_back(values[1]);
            } // normals
            else if (!firstWord.compare("vn")) {
                ss >> values[0] >> values[1] >> values[2];
                in_normals.push_back(values[0]);
                in_normals.push_back(values[1]);
                in_normals.push_back(values[2]);
            } // faces
            else if (!firstWord.compare("f")) {
                while(ss >> aux) { // aux is like "1/2/3"
                    std::istringstream ss2(aux);
                    while(std::getline(ss2, aux2, '/')) {
                        in_elements.push_back( atoi(aux2.c_str()) - 1); // Wavefront .obj files start counting from 1
                    }
                }
            }
        }
        firstWord.clear();
    }

    // Filling the index the correct way, we make a map of the faces. IF we see "1/2/3" for the
    // first time we add on the final "elements" vector, if we see it again we check on this
    // map what is the index of it, then we add the same index on "elements" again
    std::map<vector<int>,int> facesValues;
    int index = 0;

    // we will see each input vertex, so we will for every 3 values on our in_vertices vector
    for(int i = 0; i < in_elements.size(); i+=3) {
        vector<int> auxVec;
        auxVec.push_back(in_elements[i]);
        auxVec.push_back(in_elements[i+1]);
        auxVec.push_back(in_elements[i+2]);

        // if "1/2/3" is not on the map
        if(facesValues.find(auxVec) == facesValues.end()){
            // lets put the values on the actual vectors we will output
            int vectorIndex = in_elements[i]*3;
            int textureIndex = in_elements[i+1]*2;
            int normalIndex = in_elements[i+2]*3;

            vertices.push_back(in_vertices[vectorIndex]);
            vertices.push_back(in_vertices[vectorIndex+1]);
            vertices.push_back(in_vertices[vectorIndex+2]);

            uvtextures.push_back(in_uvtextures[textureIndex]);
            uvtextures.push_back(in_uvtextures[textureIndex+1]);

            normals.push_back(in_normals[normalIndex]);
            normals.push_back(in_normals[normalIndex+1]);
            normals.push_back(in_normals[normalIndex+2]);

            index = (vertices.size()/3) - 1;
            elements.push_back(index);
            facesValues.insert( std::pair<vector<int>,int> (auxVec,index) );
        } // if it is on the map, just add it again
        else {
            elements.push_back(facesValues.find(auxVec)->second);
        }
    }

    numVertices = vertices.size()/3;
    numTextures = uvtextures.size()/2;
    numNormals = normals.size()/3;
    numElements = elements.size();

    // Now, reading the texture using SOIL directly as a new OpenGL texture
    textureDiffMapID = load_png(filetextureDiff);
    textureSpecMapID = load_png(filetextureSpec);

    // check for an error during the load process
    if( 0 == textureDiffMapID || 0 == textureSpecMapID ) {
        printf( "Error loading textures\n" );
    }
}

/*
 * setUpTexture
 *
 * INPUT:
 *         program - the program ID of the shaders being used.
 *         textureShaderAttribute - the name of the variable "sampler2D" on
 *                                  the shader.
 *
 * DESCRIPTION:
 *         This function will bind the texture, setup the variables to
 *         display it and will activate it.
 *
 */
void Shape::setUpTexture (GLuint program, char* textureShaderAttribute) {
    /*
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/

    GLuint textID = glGetUniformLocation(program, textureShaderAttribute);
    //glActiveTexture(GL_TEXTURE0);
    //glUniform1i(textID, 0);
}
