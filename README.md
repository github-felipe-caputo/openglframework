# openglframework
As the name implies, this is a (C++) framework for interaction with OpenGL 3.1+. Currently it includes the following:

- A Vertex and Fragment shader "reader".
- A Shape creator with a few primitives such as cube, cylinder and sphere.
- An .obj loader, for vertices, textures and normals.
- Some simple transformations (translation, rotation, scaling).
- Phong Illumination implementation.
- Camera manipulation.

## Compiling the code
To compile the code you will need

- [GLEW](http://glew.sourceforge.net/) - Determining which OpenGL extensions are supported.
- [freeglut](http://freeglut.sourceforge.net/) - Takes care of all the system-specific chores required for creating windows
- [Matrix TCL Pro](http://www.techsoftpl.com/matrix/) - A generic C++ template class library for performing matrix algebra calculations.
- [Simple OpenGL Image Library](http://www.lonesock.net/soil.html) - A library to load images in OpenGL.

A `Makefile` is available on the repo as an example.

## Shader reader
The files `shader.cpp` and `shader.h` have the code used to read GLSL Vertex and Fragment shaders. They will read the files, bind the code to the shaders, compile and link the shaders into a "program". If an error occurs, a log will be printed.

Usage is easy, the only function you will use essentially is `makeShaderProgram`:

```c++
// Load shaders
GLuint program;
program = shader::makeShaderProgram( "shaders/simpleVert.glsl", 
                                     "shaders/simpleFrag.glsl" );
```

## Creating shapes
To create shapes you will used the files `shape.cpp` and `shape.h`. They will create the geometry of the shapes and the normals (either flat or smooth). It's also possible to assign material values that will be used in the lighting model. Only three primitives so far, cube, cylinder and sphere. Some samples:

```c++
// Creating a cube, three sub divisions
Shape shape;
shape.makeCube(3); 

// setting materials
shape.setMaterials(0.5f, 0.1f, 0.9f, 0.5f, 
                   0.89f, 0.0f, 0.0f, 0.7f, 
                   1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

shape.getVertices(); // return vertices
```

# Transformations
On `mathHelper.cpp` and `mathHelper.h` you have the source code to generate matrices for translation, rotation and scaling (with help of the Matrix TCL lib).

```c++
// Creating a transformation matrix with some rotations and a translation
Matrix mTransform = translate(-1,-0.9,-4) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
```

## Illumination
So far only the Phong Illumination model is available. Lighting can be set up using `lighting.cpp` and `lighting.h` (and obviously you will need to implement shaders to deal with the values accordingly). Example:

```c++
// Creating a light source, 
Lighting light(2.0f, 2.0f, -4.0f,  // the light position (x, y, z)
               1.0f, 1.0f,  1.0f,  // the light intensity (r, g, b)
               0.5f, 0.5f,  0.5f); // the value of the ambient light (r, g, b)

// set up phong for a shape
light.setPhongIllumination(program, cube);
```

## Camera manipulation
You are able to create either orthogonal or perspective projections in the framework. The source is on the files (you guessed it!) `camera.cpp` and `camera.h`. It's possible to choose the projection type, but it will create a view for default values. It's also possible to move around the camera (i.e., move the world) forward, backward, strafe left, strafe right, etc.

```c++
// Our Camera, perspective projection
Camera cam(PROJ_PERSP);

// gets the view matrix
cam.getViewMatrix();

// gets the projection matrix
cam.getProjMatrix();
```

## Examples
Thre examples of how to use the framework can be seen on the `examples` folder:

- `shapeSubDiv.cpp`: shows different shapes with different mesh subdivisions.
- `phongLightModel.cpp`: shows flat, gouraud and phong lighting on a sphere.
- `shapesAndLight.cpp`: shows 4 shapes being affected by lighting at the same time.
- `readingObjs.cpp`, `readingObjVertNorm.cpp`, `readingObjVertTexNorm.cpp`: shows how to load objects from .obj files.

## More
Check [http://fvcaputo.github.io/](http://fvcaputo.github.io/).
