
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

// funcs
void renderScene(const GLuint &targetProgram);

using namespace std;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// PROGRAM ID
GLuint programGeometryPass;

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

// G-Buffer Data
GLuint gBuffer;
GLuint gPosition, gNormal, gColorAlbSpec;

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

// Light Values
float lightPos[] = { 2.0f, 2.0f, 5.0f };
float lightIntensityRGB[] = { 1.0f, 1.0f, 1.0f };
float lightAmbientRGB[] = { 0.5f, 0.5f,  0.5f };

float lightDir[] = { 0.0f, 0.0f,  0.0f };
float lightUp[] = { 0.0f, 1.0f,  0.0f };

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

// Variables for shadow viewport
const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;


// Variables for window viewport
const unsigned int WINDOW_WIDTH = 512;
const unsigned int WINDOW_HEIGHT = 512;

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
    // TODO: remove these three and create geometry and lighting shaders
    programGeometryPass = shader::makeShaderProgram( "shaders/gBufferGeometryVert.glsl",
                                                     "shaders/gBufferGeometryFrag.glsl" );




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

    // vertex position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // normals
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vCubeDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Sphere
    glBindVertexArray(vaoSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // vertex position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize) );

    // normals
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + vSphereDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Cylinder
    glBindVertexArray(vaoCylinder);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // vertex position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize) );

    // normals
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize + vCylinderDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Screen Quad
    glBindVertexArray(vaoScreenQuad);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // vertex position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize + totalCylinderDataSize) );

    // texture coords
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalCubeDataSize + totalSphereDataSize + totalCylinderDataSize + vScreenQuadDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Wireframe test
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_CULL_FACE );
}

void prepareFramebuffers () {
    //
    // G Buffer
    //

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // position
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // normals
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // colors
    glGenTextures(1, &gColorAlbSpec);
    glBindTexture(GL_TEXTURE_2D, gColorAlbSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorAlbSpec, 0);

    //
    // Renderbuffer for Depth
    //
    GLuint renderbufferDepth;
    glGenRenderbuffers(1, &renderbufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferDepth);

    // Attachements to draw buffer and check frame buffer

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::printf("Error building Framebuffer!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void display () {
    // Both will use camera view and projections

    // Camera view matrix
    Matrix mViewMatrix = cam.getViewMatrix();
    GLuint mViewMatrixID = glGetUniformLocation(programGeometryPass, "mViewMatrix");
    glUniformMatrix4fv(mViewMatrixID, 1, GL_TRUE, &mViewMatrix[0][0]);

    // Camera projection matrix
    Matrix mProjMatrix = cam.getProjMatrix();
    GLuint mProjMatrixID = glGetUniformLocation(programGeometryPass, "mProjMatrix");
    glUniformMatrix4fv(mProjMatrixID, 1, GL_TRUE, &mProjMatrix[0][0]);

    //
    // Render to g buffer!
    //

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram( programGeometryPass );

    //glCullFace(GL_FRONT);
    renderScene( programGeometryPass );
    //glCullFace(GL_BACK);

    //
    // Render to to the light pass shader using the generated textures from the geometry one!
    //

    glViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // clear framebuffer content
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Use our "normal" shaders to render the scene
    //glUseProgram( programShadowMap );
    //use the shador to light pass !

    //renderScene( programShadowMap );
    //render to light pass !

    // swap the buffers
    glutSwapBuffers();
}

void renderScene(const GLuint &targetProgram ) {
    // Transform will be different for the objects, for now just set it up
    Matrix mTransform;
    GLuint mTransformID = glGetUniformLocation(targetProgram, "mTransform");

    //
    // Illumination BLOW UP
    //

    Lighting light(lightPos, lightIntensityRGB, lightAmbientRGB);

    //
    // First the cube
    //

    glBindVertexArray(vaoCube);

    light.setPhongIllumination(targetProgram, cube);

    mTransform = translate(-1,-0.9,-4) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)0);

    //
    // Now the sphere
    //

    glBindVertexArray(vaoSphere);

    light.setPhongIllumination(targetProgram, sphere);

    mTransform = translate(1,-0.9,-4) * scale(0.5,0.5,0.5) * rotate(ztheta, zVec) *  rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, sphereNumElements, GL_UNSIGNED_SHORT, (void*)sphereElementByteOffset);

    //
    // Another the cube
    //

    glBindVertexArray(vaoCube);

    light.setPhongIllumination(targetProgram, cube);

    mTransform = translate(1,1.1,-4) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)0);

    //
    // Finally the cylinder
    //

    glBindVertexArray(vaoCylinder);

    light.setPhongIllumination(targetProgram, cylinder);

    mTransform = translate(-1,1.1,-4) * rotate(ztheta, zVec) * rotate(ytheta, yVec) * rotate(xtheta, xVec);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cylinderNumElements, GL_UNSIGNED_SHORT, (void*)cylinderElementByteOffset);

    // Wall and floor

    float planeAmb[] = {0.5f, 0.5f, 0.5f};
    float planeDiff[] = {0.5f, 0.5f, 0.5f};
    float planeSpec[] = {1.0f, 1.0f, 1.0f};

    //
    // Another cube, scaled behind the scene as a sort of plane (wall)
    //
    glBindVertexArray(vaoCube);
    light.setPhongIllumination(targetProgram,
                               planeAmb, 0.5f,
                               planeDiff, 0.6f,
                               planeSpec, 0.5f, 30.0f);

    mTransform = translate(0,0,-7) * scale(10, 10, 0.1f);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)0);

    //
    // Another cube, scaled behind the scene as a sort of plane (floor)
    //
    glBindVertexArray(vaoCube);
    light.setPhongIllumination(targetProgram,
                               planeAmb, 0.5f,
                               planeDiff, 0.6f,
                               planeSpec, 0.5f, 30.0f);

    mTransform = translate(0,-2,-3) * rotate(90, xVec) * scale(10, 10, 0.1f);
    glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

    // Drawing elements
    glDrawElements( GL_TRIANGLES, cubeNumElements, GL_UNSIGNED_SHORT, (void*)0);
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
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);

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
    prepareFramebuffers();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( animate );

    glutMouseFunc ( mousePressedEvent );
    glutMotionFunc( mouseMovementEvent );

    glutMainLoop();

    return 0;
}