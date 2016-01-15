
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
Shape shape;

// BUFFERS
bool bufferInit = false;

GLuint vbuffer;
GLuint ebuffer;

GLuint vaoShape;

// Total number of elements that will be draw
int shapeNumElements;

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

// Shading and normals
int normalType = FLAT;
int shaders = 0;
int materials = 0;

void createShapes(int normalType, int shaders, int materials) {
    //
    // SHAPE 
    //
    shape.clearShape();
    shape.makeSphere(3, normalType);

    if(materials == 0)
        shape.setMaterials(0.1f, 0.5f, 0.9f, 0.5f, 
                           0.0f, 0.0f, 0.5f, 0.9f, 
                           1.0f, 1.0f, 1.0f, 1.0f, 10.0f);
    else
        shape.setMaterials(0.5f, 0.1f, 0.9f, 0.5f, 
                           0.89f, 0.0f, 0.0f, 0.7f, 
                           1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

    int vShapeDataSize = shape.getNumVertices()*3*sizeof(GLfloat);
    int nShapeDataSize = shape.getNumNormals()*3*sizeof(GLfloat);
    int eShapeDataSize = shape.getNumElements()*sizeof(GLshort);

    // Load shaders
    if (shaders == 0)
        program = shader::makeShaderProgram( "shaders/flatLightingVert.glsl", 
                                             "shaders/flatLightingFrag.glsl" );
    else 
        program = shader::makeShaderProgram( "shaders/phongLightingVert.glsl", 
                                             "shaders/phongLightingFrag.glsl" );

    //
    // VERTEX ARRAY BUFFER
    //

    if( bufferInit ) {
        glDeleteBuffers( 1, &vbuffer );
        glDeleteBuffers( 1, &ebuffer );
        glDeleteVertexArrays( 1, &vaoShape );
    }

    // Create Vertex Array Buffer
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // Create space for the data, load the data
    // This example, just vertex data
    glBufferData( GL_ARRAY_BUFFER, vShapeDataSize+nShapeDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vShapeDataSize, shape.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, vShapeDataSize, nShapeDataSize, shape.getNormals() );

    //
    // ELEMENT ARRAY BUFFER
    //

    // Create the element buffer
    glGenBuffers( 1, &ebuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Create space for the data, load the data
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, eShapeDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, eShapeDataSize, shape.getElements() );

    shapeNumElements = shape.getNumElements();

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

    glGenVertexArrays(1, &vaoShape);

    // Shape
    glBindVertexArray(vaoShape);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vShapeDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Buffers were created, need to destroy if we use them again
    bufferInit = true;
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
    Lighting light(2.0f, 2.0f, 0.0f,
                   1.0f, 1.0f,  1.0f,
                   0.5f, 0.5f,  0.5f);

    //
    // Binding the shape, transforms, etc
    //
    glBindVertexArray(vaoShape);

    light.setPhongIllumination(program, shape);
    mTransform = translate(0,0,-1.5) * scale(0.5,0.5,0.5) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, shapeNumElements, GL_UNSIGNED_SHORT, (void*)0);


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
        // Animation
        case 'j':                      
            animatingX = !animatingX;
            break;
        case 'k':                      
            animatingY = !animatingY;
            break;
        case 'l':                      
            animatingZ = !animatingZ;
            break;
        // normals
        case 'z':
            if (normalType == FLAT){
                createShapes(SMOOTH, shaders, materials);
                normalType = SMOOTH;
            }else{
                createShapes(FLAT, shaders, materials);
                normalType = FLAT;
            }
            break;
        // shaders
        case 'x':
            if (shaders == 0){
                createShapes(normalType, 1, materials);
                shaders = 1;
            }else{
                createShapes(normalType, 0, materials);
                shaders = 0;
            }
            break;
        // materials
        case 'c':
            if (materials == 0){
                createShapes(normalType, shaders, 1);
                materials = 1;
            }else{
                createShapes(normalType, shaders, 0);
                materials = 0;
            }
            break;
        // Subdivisions
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