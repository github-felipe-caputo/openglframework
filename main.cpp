
// OpenGL stuff
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#endif

// C libraries
#include <iostream>

// Helper variables
#include "screenQuadHelper.h"

// Header-only
#include "shader.h"
#include "mathHelper.h"

// Classes
#include "shape.h"
#include "camera.h"
#include "lighting.h"

using namespace std;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// PROGRAM ID
GLuint program;
GLuint programScreen;

// Shapes we will use
Shape cube;
Shape sphere;
Shape cylinder;

// BUFFERS
GLuint vbuffer;
GLuint ebuffer;

GLuint vaoCube;
GLuint vaoSphere;
GLuint vaoCylinder;
GLuint vaoScreenQuad;

GLuint framebuffer;
GLuint renderbuffer;

GLuint texColorBuffer; // screen quad buffer

GLuint sphereElementByteOffset; // used on the draw
GLuint cylinderElementByteOffset; // used on the draw
GLuint screenQuadElementByteOffset; // used on the draw

// Total number of elements that will be draw
int cubeNumElements;
int sphereNumElements;
int cylinderNumElements;
int screenQuadNumElements;

// Our Camera
Camera cam(PROJ_PERSP);

// x, y and z vectors for rotation
float xVec[] = {1,0,0};
float yVec[] = {0,1,0};
float zVec[] = {0,0,1};

// Variables for animation
bool animatingX = false;
bool animatingY = false;
bool animatingZ = false;
float xtheta = 0.0f;
float ytheta = 0.0f;
float ztheta = 0.0f;

void init () {
    //
    // SHAPES
    //

    // First: cube
    cube.makeCube(3);
    cube.setMaterials(0.5f, 0.1f, 0.9f, 0.5f,
                      0.89f, 0.0f, 0.0f, 0.7f,
                      1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

    int vCubeDataSize = cube.getNumVertices()*3*sizeof(GLfloat);
    int nCubeDataSize = cube.getNumNormals()*3*sizeof(GLfloat);
    int eCubeDataSize = cube.getNumElements()*sizeof(GLshort);

    int totalCubeDataSize = vCubeDataSize + nCubeDataSize;

    // Second: sphere
    sphere.makeSphere(3, SMOOTH);
    sphere.setMaterials(0.1f, 0.5f, 0.9f, 0.5f,
                        0.0f, 0.0f, 0.5f, 0.9f,
                        1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

    int vSphereDataSize = sphere.getNumVertices()*3*sizeof(GLfloat);
    int nSphereDataSize = sphere.getNumNormals()*3*sizeof(GLfloat);
    int eSphereDataSize = sphere.getNumElements()*sizeof(GLshort);

    int totalSphereDataSize = vSphereDataSize + nSphereDataSize;

    // Third: Cylinder
    cylinder.makeCylinder(16,5, SMOOTH);
    cylinder.setMaterials(0.5f, 0.9f, 0.2f, 0.5f,
                          0.0f, 1.0f, 0.5f, 0.6f,
                          1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

    int vCylinderDataSize = cylinder.getNumVertices()*3*sizeof(GLfloat);
    int nCylinderDataSize = cylinder.getNumNormals()*3*sizeof(GLfloat);
    int eCylinderDataSize = cylinder.getNumElements()*sizeof(GLshort);

    int totalCylinderDataSize = vCylinderDataSize + nCylinderDataSize;

    // Screen Quad
    int vScreenQuadDataSize = 4*2*sizeof(GLfloat);
    int uvScreenQuadDataSize = 4*2*sizeof(GLfloat);
    int eScreenQuadDataSize = 6*sizeof(GLshort);

    int totalScreenQuadDataSize = vScreenQuadDataSize + uvScreenQuadDataSize + eScreenQuadDataSize;

    // Load shaders
    program = shader::makeShaderProgram( "shaders/phongLightingVert.glsl",
                                         "shaders/phongLightingFrag.glsl" );
    programScreen = shader::makeShaderProgram( "shaders/framebufferScreenVert.glsl",
                                               "shaders/framebufferScreenFrag.glsl" );

    //
    // VERTEX ARRAY BUFFER
    //

    // Create Vertex Array Buffer
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // Create space for the data, load the data
    // Data looks like
    //    shape 1        shape 2
    // (VVVV) (NNNN) | (VVVV) (NNNN) | ...
    glBufferData( GL_ARRAY_BUFFER, totalCubeDataSize +
                                   totalSphereDataSize +
                                   totalCylinderDataSize +
                                   totalScreenQuadDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vCubeDataSize, cube.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, vCubeDataSize, nCubeDataSize, cube.getNormals() );
    glBufferSubData( GL_ARRAY_BUFFER, totalCubeDataSize, vSphereDataSize, sphere.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, totalCubeDataSize + vSphereDataSize, nSphereDataSize, sphere.getNormals() );
    glBufferSubData( GL_ARRAY_BUFFER, totalCubeDataSize + totalSphereDataSize, vCylinderDataSize, cylinder.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, totalCubeDataSize + totalSphereDataSize + vCylinderDataSize, nCylinderDataSize, cylinder.getNormals() );
    glBufferSubData( GL_ARRAY_BUFFER, totalCubeDataSize + totalSphereDataSize + totalCylinderDataSize, vScreenQuadDataSize, quadVertices );
    glBufferSubData( GL_ARRAY_BUFFER, totalCubeDataSize + totalSphereDataSize + totalCylinderDataSize + vScreenQuadDataSize, uvScreenQuadDataSize, quadTextures );

    //
    // ELEMENT ARRAY BUFFER
    //

    // Create the element buffer
    glGenBuffers( 1, &ebuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Create space for the data, load the data
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, eCubeDataSize +
                                           eSphereDataSize +
                                           eCylinderDataSize +
                                           eScreenQuadDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, eCubeDataSize, cube.getElements() );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, eCubeDataSize, eSphereDataSize, sphere.getElements() );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, eCubeDataSize + eSphereDataSize, eCylinderDataSize, cylinder.getElements() );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, eCubeDataSize + eSphereDataSize + eCylinderDataSize, eScreenQuadDataSize, quadElements );

    cubeNumElements = cube.getNumElements();

    sphereNumElements = sphere.getNumElements();
    sphereElementByteOffset = eCubeDataSize; // sphere starts after cube

    cylinderNumElements = cylinder.getNumElements();
    cylinderElementByteOffset = eCubeDataSize + eSphereDataSize; // cylinder starts after cube and sphere

    screenQuadNumElements = 6;
    screenQuadElementByteOffset = eCubeDataSize + eSphereDataSize + eCylinderDataSize; // screen quade after cube, sphere, cylinder

    //
    // VERTEX ARRAY OBJECTS
    //

    // Setting up vertex array object
    GLuint vPosition;
    GLuint vNormal;
    GLuint vTexCoord;

    glGenVertexArrays(1, &vaoCube);
    glGenVertexArrays(1, &vaoSphere);
    glGenVertexArrays(1, &vaoCylinder);
    glGenVertexArrays(1, &vaoScreenQuad);

    // Cube
    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vCubeDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Sphere
    glBindVertexArray(vaoSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize) );

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + vSphereDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Cylinder
    glBindVertexArray(vaoCylinder);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize) );

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize + vCylinderDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Screen Quad
    glBindVertexArray(vaoScreenQuad);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( programScreen , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize + totalCylinderDataSize) );

    vTexCoord = glGetAttribLocation( programScreen, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize + totalCylinderDataSize + vScreenQuadDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    //
    // FRAMEBUFFER OBJECTS
    //

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    //
    // RENDERBUFFER OBJECTS
    //

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::printf("Error building Framebuffer!\n");
    }

    // Wireframe test
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void display () {
    //
    // Normal render, to framebuffer
    //

    // Bind texture framebuffer, render as you would normally
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable( GL_DEPTH_TEST );

    // clear framebuffer content
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Use our "normal" shaders to render the scene
    glUseProgram( program );

    // Transform will be different for the objects, for now just set it up
    Matrix mTransform;
    GLuint mTransformID = glGetUniformLocation(program, "mTransform");

    // View matrix will be the same for all the objects, just send it now
    Matrix mViewMatrix = cam.getViewMatrix();
    GLuint mViewMatrixID = glGetUniformLocation(program, "mViewMatrix");
    glUniformMatrix4fv(mViewMatrixID, 1, GL_TRUE, &mViewMatrix[0][0]);

    // View matrix will be the same for all the objects, just send it now
    Matrix mProjMatrix = cam.getProjMatrix();
    GLuint mProjMatrixID = glGetUniformLocation(program, "mProjMatrix");
    glUniformMatrix4fv(mProjMatrixID, 1, GL_TRUE, &mProjMatrix[0][0]);

    //
    // Illumination BLOW UP
    //

    Lighting light(2.0f, 2.0f, -4.0f,
                   1.0f, 1.0f,  1.0f,
                   0.5f, 0.5f,  0.5f);

    //
    // First the cube
    //

    glBindVertexArray(vaoCube);

    light.setPhongIllumination(program, cube);

    mTransform = translate(-1,-0.9,-4) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)0);

    //
    // Now the sphere
    //

    glBindVertexArray(vaoSphere);

    light.setPhongIllumination(program, sphere);

    mTransform = translate(1,-0.9,-4) * scale(0.5,0.5,0.5) * rotate(ztheta, zVec) *  rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, sphereNumElements, GL_UNSIGNED_SHORT, (void*)sphereElementByteOffset);

    //
    // Another the cube
    //

    glBindVertexArray(vaoCube);

    light.setPhongIllumination(program, cube);

    mTransform = translate(1,1.1,-4) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)0);

    //
    // Finally the cylinder
    //

    glBindVertexArray(vaoCylinder);

    light.setPhongIllumination(program, cylinder);

    mTransform = translate(-1,1.1,-4) * rotate(ztheta, zVec) *  rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cylinderNumElements, GL_UNSIGNED_SHORT, (void*)cylinderElementByteOffset);

    //
    // Render screen quad, defu=ault framebuffer
    //

    // Now back to default framebuffer and render to screen quad
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test on screen-space quad

    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear(GL_COLOR_BUFFER_BIT);

    // Use our "screen quad" shaders
    glUseProgram( programScreen );

    glBindVertexArray(vaoScreenQuad);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glDrawElements( GL_TRIANGLES, screenQuadNumElements, GL_UNSIGNED_SHORT, (void*)screenQuadElementByteOffset);

    // swap the buffers
    glutSwapBuffers();
}

// to use the keyboard
void keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
        case 'w':
            cam.moveForward();
            break;
        case 's':
            cam.moveBackward();
            break;
        case 'd':
            cam.strafeRight();
            break;
        case 'a':
            cam.strafeLeft();
            break;
        case 'r':
            cam.moveUp();
            break;
        case 'f':
            cam.moveDown();
            break;
        case 'j':
            animatingX = !animatingX;
            break;
        case 'k':
            animatingY = !animatingY;
            break;
        case 'l':
            animatingZ = !animatingZ;
            break;
        case 033: case 'q': case 'Q':  // terminate the program
            exit( 0 );
            break;
    }
    glutPostRedisplay();
}

// Animate the objects (maybe)
void animate () {

    if( animatingX ) {
        xtheta += 0.1f;
    }
    if ( animatingY ) {
        ytheta += 0.1f;
    }
    if ( animatingZ ) {
        ztheta += 0.1f;
    }
    glutPostRedisplay();
}

// Function to help setting the movement of the camera given the mouse
void mousePressedEvent( int button, int state, int x, int y ) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        cam.setInitialMouseCoord(x,y);
    }

}

// Function to help setting the movement of the camera given the mouse
void mouseMovementEvent( int x, int y ) {
    cam.moveCameraTarget(x,y);
}


int main ( int argc, char **argv ) {
    // initialize glut
    glutInit(&argc, argv);

    // memory buffers
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    //set window size
    glutInitWindowSize(512, 512);

    // window
    glutCreateWindow( "OpenGL Framework" );

    #ifndef __APPLE__
    // Try to initalize glew to use gl apis
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW error");
        return 1;
    }
    #endif

    std::printf("%s\n%s\n", glGetString(GL_RENDERER),  glGetString(GL_VERSION));

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( animate );

    glutMouseFunc ( mousePressedEvent );
    glutMotionFunc( mouseMovementEvent );

    glutMainLoop();

    return 0;
}