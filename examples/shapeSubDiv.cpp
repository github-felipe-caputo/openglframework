
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

int cubeSphereSubDiv = 3;
int cylinderBaseSubdiv = 16;

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
int shapeType = 0; // 0 = cube, 1 = sphere, 2 = cylinder
bool animatingX = false;
bool animatingY = false;
bool animatingZ = false;
float xtheta = 30.0f;
float ytheta = 30.0f;
float ztheta = 30.0f;

void createShapes() {
    //
    // SHAPE 
    //
    shape.clearShape();

    if (shapeType == 0)
        shape.makeCube(cubeSphereSubDiv);
    else if (shapeType == 1)
        shape.makeSphere(cubeSphereSubDiv,SMOOTH);
    else
        shape.makeCylinder(cylinderBaseSubdiv,cubeSphereSubDiv,SMOOTH);

    int vShapeDataSize = shape.getNumVertices()*3*sizeof(GLfloat);
    int eShapeDataSize = shape.getNumElements()*sizeof(GLshort);

    // Load shaders
    program = shader::makeShaderProgram( "shaders/simpleVert.glsl", 
                                         "shaders/simpleFrag.glsl" );

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
    // DThis example, just vertex data
    glBufferData( GL_ARRAY_BUFFER, vShapeDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vShapeDataSize, shape.getVertices() );

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

    glGenVertexArrays(1, &vaoShape);

    // Shape
    glBindVertexArray(vaoShape);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Buffers were created, need to destroy if we use them again
    bufferInit = true;
}

void init () {
    // Create the shapes
    createShapes();

    // Some openGL initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // This shows just the lines
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
    // Binding the shape, transforms, etc
    //
    glBindVertexArray(vaoShape);

    if (shapeType == 1) // if it's the sphere, let's scale it so it's easier to see 
        mTransform = translate(0,0,-2) * scale(0.5,0.5,0.5) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    else
        mTransform = translate(0,0,-2) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    
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
        // Shapes
        case 'z':
            if (shapeType == 2)
                shapeType = 0;
            else
                shapeType++;
            createShapes();
            break;        
        // Subdivisions
        case 'm':
            cubeSphereSubDiv++;
            createShapes();
            break;
        case 'n':
            if (cubeSphereSubDiv > 1)
                cubeSphereSubDiv--;
            createShapes();
            break;
        case 'b':
            cylinderBaseSubdiv++;
            createShapes();
            break;
        case 'v':
            if (cylinderBaseSubdiv > 1)
                cylinderBaseSubdiv--;
            createShapes();
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