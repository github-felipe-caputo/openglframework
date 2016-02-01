
// OpenGL stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

// C libraries
#include <iostream>

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

// Shapes we will use
Shape sphere;
Shape cube; // streched cube = floor

// Buffers
GLuint vbuffer;
GLuint ebuffer;

GLuint vaoSphere;
GLuint vaoCube;

GLuint cubeElementByteOffset; // used on the draw

// Total number of elements that will be draw
int sphereNumElements;
int cubeNumElements;

// Our Camera
Camera cam(PROJ_PERSP);

// x, y and z vectors for rotation
float xVec[] = {1,0,0};
float yVec[] = {0,1,0};
float zVec[] = {0,0,1};

void createShapes(int normalType, int shaders, int materials) {
    //
    // SHAPES
    //

    // spheres
    sphere.makeSphere(3, SMOOTH);

    int vSphereDataSize = sphere.getNumVertices()*3*sizeof(GLfloat);
    int nSphereDataSize = sphere.getNumNormals()*3*sizeof(GLfloat);
    int eSphereDataSize = sphere.getNumElements()*sizeof(GLshort);

    int totalSphereDataSize = vSphereDataSize + nSphereDataSize;

    // floor/cube
    cube.makeCube(3);

    int vCubeDataSize = cube.getNumVertices()*3*sizeof(GLfloat);
    int nCubeDataSize = cube.getNumNormals()*3*sizeof(GLfloat);
    int eCubeDataSize = cube.getNumElements()*sizeof(GLshort);

    int totalCubeDataSize = vCubeDataSize + nCubeDataSize;

    // Program
    program = shader::makeShaderProgram( "shaders/phongLightingVert.glsl", 
                                         "shaders/phongLightingFrag.glsl" );

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
    glBufferData( GL_ARRAY_BUFFER, totalSphereDataSize+totalCubeDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vSphereDataSize, sphere.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, vSphereDataSize, nSphereDataSize, sphere.getNormals() );
    glBufferSubData( GL_ARRAY_BUFFER, totalSphereDataSize, vCubeDataSize, cube.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, totalSphereDataSize+vCubeDataSize, nCubeDataSize, cube.getNormals() );

    //
    // ELEMENT ARRAY BUFFER
    //

    // Create the element buffer
    glGenBuffers( 1, &ebuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Create space for the data, load the data
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, eSphereDataSize+eCubeDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, eSphereDataSize, sphere.getElements() );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, eSphereDataSize, eCubeDataSize, cube.getElements() );

    sphereNumElements = sphere.getNumElements();
    cubeNumElements = cube.getNumElements();

    cubeElementByteOffset = eSphereDataSize; // cube starts after sphere

    //
    // SETTING UP THE SHADER
    //

    // Use program
    glUseProgram( program );

    //
    // VERTEX ARRAY OBJECTS
    //

    // Setting up vertex array object
    GLuint vPosition;
    GLuint vNormal;

    glGenVertexArrays(1, &vaoSphere);

    // Sphere
    glBindVertexArray(vaoSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSphereDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Cube
    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalSphereDataSize) );

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalSphereDataSize + vCubeDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );
}

void init () {
    // Create the shapes
    createShapes(FLAT, 0, 0);

    // Some openGL initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
}

void display () {
    // clear 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


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
    Lighting light(0.5f, 2.5f, 0.0f,
                   1.0f, 1.0f, 1.0f,
                   0.5f, 0.5f, 0.5f);

    //
    // First the Sphere -> Binding the shape, transforms, etc
    //
    glBindVertexArray(vaoSphere);

    sphere.setMaterials(0.5f, 1.0f, 0.1f, 0.5f, 
                        0.0f, 0.7f, 0.0f, 0.7f, 
                        1.0f, 1.0f, 0.7f, 1.0f, 10.0f);

    light.setPhongIllumination(program, sphere);
    mTransform = translate(0,0.1,-2) * scale(0.4,0.4,0.4);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, sphereNumElements, GL_UNSIGNED_SHORT, (void*)0);

    glBindVertexArray(0); // Unbind our Vertex Array Object 

    //
    // Another Sphere
    //
    glBindVertexArray(vaoSphere);

    sphere.setMaterials(0.5f, 0.1f, 0.9f, 0.5f, 
                        0.89f, 0.0f, 0.0f, 0.7f, 
                        1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

    light.setPhongIllumination(program, sphere);
    mTransform = translate(-0.5,-0.2,-2.5) * scale(0.3,0.3,0.3);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, sphereNumElements, GL_UNSIGNED_SHORT, (void*)0);

    //
    // Now the cube
    //
    glBindVertexArray(vaoCube);

    cube.setMaterials(0.1f, 0.5f, 0.9f, 0.8f, 
                      0.0f, 0.0f, 0.5f, 0.9f, 
                      1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

    light.setPhongIllumination(program, cube);
    mTransform = translate(-0.7,-0.8,-4) * scale(3,0.1,6);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)cubeElementByteOffset);



    // swap the buffers
    glutSwapBuffers();
}

// to use the keyboard
void keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
        // Camera
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
        case 033: case 'q': case 'Q':  // terminate the program
            exit( 0 );
            break;
    }
    glutPostRedisplay();
}

// Animate the objects (maybe)
void animate () {
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    //set window size
    glutInitWindowSize(512, 512);

    // window
    glutCreateWindow( "OpenGL Framework" );

    // Try to initalize glew to use gl apis
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW error");
        return 1;
    }

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( animate );

    glutMouseFunc ( mousePressedEvent );
    glutMotionFunc( mouseMovementEvent );

    glutMainLoop();

    return 0;
}