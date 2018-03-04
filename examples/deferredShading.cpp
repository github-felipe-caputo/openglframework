
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
void renderGBufferToQuad(const GLuint &targetProgram);

using namespace std;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// PROGRAM ID
GLuint programGeometryPass;
GLuint programLightPass;
GLuint programScreen; // debugging

// Shapes we will use
Shape shape;

// BUFFERS
GLuint vbuffer;
GLuint ebuffer;

GLuint vaoShape;
GLuint vaoScreenQuad;

// G-Buffer Data
GLuint gBuffer;
GLuint gPosition, gNormal, gColorAlb, gColorSpec;

GLuint screenQuadElementByteOffset; // used on the draw

// Total number of elements that will be draw
int shapeNumElements;
int screenQuadNumElements;

// Our Camera
Camera cam(PROJ_PERSP);

// Light Values
float lightPos[] = { 2.0f, 2.0f, 5.0f };
float lightIntensityRGB[] = { 1.0f, 1.0f, 1.0f };
float lightAmbientRGB[] = { 0.5f, 0.5f,  0.5f };

float lightDir[] = { 0.0f, 0.0f, 0.0f };
float lightUp[] = { 0.0f, 1.0f, 0.0f };

// Shape positions
vector<vector<float>> shapePositions;

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

    // TODO: the specular texture NEEDS TO BE a specular intensity texture
    // in other words, not a texture where we care for 3 encoded values
    // RGB, but a texture with only one encoded value that should be the
    // specular intensity
    shape.clearShape();
    shape.readObjLightMap( "objects/Pokemon3.obj" ,
                           "objects/Final_Pokemon_Diffuse.png",
                           "objects/Final_Pokemon_Specular.png" );

    int vShapeDataSize = shape.getNumVertices()*3*sizeof(GLfloat);
    int nShapeDataSize = shape.getNumNormals()*3*sizeof(GLfloat);
    int uvShapeDataSize = shape.getNumUV()*2*sizeof(GLfloat);
    int eShapeDataSize = shape.getNumElements()*sizeof(GLshort);

    int totalShapeDataSize = vShapeDataSize + nShapeDataSize + uvShapeDataSize;

    // Screen Quad
    int vScreenQuadDataSize = 4*2*sizeof(GLfloat);
    int uvScreenQuadDataSize = 4*2*sizeof(GLfloat);
    int eScreenQuadDataSize = 6*sizeof(GLshort);

    int totalScreenQuadDataSize = vScreenQuadDataSize + uvScreenQuadDataSize;

    // Load shaders
    programGeometryPass = shader::makeShaderProgram( "shaders/gBufferGeometryVert.glsl",
                                                     "shaders/gBufferGeometryFrag.glsl" );
    programLightPass = shader::makeShaderProgram( "shaders/deferredShadingVert.glsl",
                                                  "shaders/deferredShadingFrag.glsl" );
    programScreen = shader::makeShaderProgram( "shaders/quadScreenVert.glsl",
                                               "shaders/quadScreenFrag.glsl" );

    //
    // VERTEX ARRAY BUFFER
    //

    // Create Vertex Array Buffer
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // Create space for the data, load the data
    // Data looks like
    //    shape 1              shape 2 (quad)
    // (VVVV) (NNNN) (UVUV) | (VVVV) (UVUV) | ...
    glBufferData( GL_ARRAY_BUFFER, totalShapeDataSize +
                                   totalScreenQuadDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vShapeDataSize, shape.getVertices() );
    glBufferSubData( GL_ARRAY_BUFFER, vShapeDataSize, nShapeDataSize, shape.getNormals() );
    glBufferSubData( GL_ARRAY_BUFFER, vShapeDataSize + nShapeDataSize, uvShapeDataSize, shape.getUV() );
    glBufferSubData( GL_ARRAY_BUFFER, totalShapeDataSize, vScreenQuadDataSize, quadVertices );
    glBufferSubData( GL_ARRAY_BUFFER, totalShapeDataSize + vScreenQuadDataSize, uvScreenQuadDataSize, quadTextures );

    //
    // ELEMENT ARRAY BUFFER
    //

    // Create the element buffer
    glGenBuffers( 1, &ebuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // Create space for the data, load the data
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, eShapeDataSize +
                                           eScreenQuadDataSize, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, eShapeDataSize, shape.getElements() );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, eShapeDataSize, eScreenQuadDataSize, quadElements );

    shapeNumElements = shape.getNumElements();

    screenQuadNumElements = 6;
    screenQuadElementByteOffset = eShapeDataSize ; // screen quad starts after shape

    //
    // VERTEX ARRAY OBJECTS
    //

    glGenVertexArrays(1, &vaoShape);
    glGenVertexArrays(1, &vaoScreenQuad);

    //
    // Shape
    glBindVertexArray(vaoShape);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // vertex position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // normals
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vShapeDataSize) );

    // texture coords
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vShapeDataSize + nShapeDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    //
    // Screen Quad
    glBindVertexArray(vaoScreenQuad);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

    // vertex position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalShapeDataSize) );

    // texture coords
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(totalShapeDataSize + vScreenQuadDataSize) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );


    //
    // Set up shapes positions
    //

    shapePositions.push_back({ 0.0, -0.5, 3.0 });
    shapePositions.push_back({ 0.0, -0.5, 1.0 });
    shapePositions.push_back({ 0.0, -0.5, -1.0 });
    shapePositions.push_back({ 0.0, -0.5, -3.0 });
    shapePositions.push_back({ -2.0, -0.5, 3.0 });
    shapePositions.push_back({ -2.0, -0.5, 1.0 });
    shapePositions.push_back({ -2.0, -0.5, -1.0 });
    shapePositions.push_back({ -2.0, -0.5, -3.0 });
    shapePositions.push_back({ 2.0, -0.5, 3.0 });
    shapePositions.push_back({ 2.0, -0.5, 1.0 });
    shapePositions.push_back({ 2.0, -0.5, -1.0 });
    shapePositions.push_back({ 2.0, -0.5, -3.0 });

    //
    // Set up camera positions
    //

    cam.setCameraPosition(3.37219f, 1.97521f, 6.71925f);
    cam.setLookAt(-0.494656f, -0.313991f, -0.810377f);
    cam.setRight(0.853549f, 0, -0.521007f);

    // Wireframe test
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //glEnable(GL_MULTISAMPLE);
    //glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_CULL_FACE );
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

    // colors diff
    glGenTextures(1, &gColorAlb);
    glBindTexture(GL_TEXTURE_2D, gColorAlb);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorAlb, 0);

    // colors spec
    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gColorSpec, 0);

    // Attachements to draw buffer
    unsigned int attachments[4] = {
        GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    //
    // Renderbuffer for Depth
    //
    GLuint renderbufferDepth;
    glGenRenderbuffers(1, &renderbufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferDepth);

    // Check frame buffer
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::printf("Error building Framebuffer!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void display () {
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error: " << err << " - " << gluErrorString(err) << endl;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    // Render to g buffer!
    //

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram( programGeometryPass );

    //glCullFace(GL_FRONT);
    renderScene( programGeometryPass );
    //glCullFace(GL_BACK);

    //
    // Render to to the light pass shader using the generated textures from the geometry one!
    //

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // clear framebuffer content
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Use our lighting shader
    glUseProgram( programLightPass );

    renderGBufferToQuad( programLightPass );
/*

    //
    // Render to Depth Map Quad, on our default framebuffer (debbuging)
    //

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // clear framebuffer content
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Use our "normal" shaders to render the scene
    glUseProgram( programScreen );

    glBindVertexArray(vaoScreenQuad);

    // set up textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gNormal); // render to quad for debugging
    GLuint textureID = glGetUniformLocation(programScreen, "screenTexture");
    glUniform1i(textureID, 0);

    glDrawElements( GL_TRIANGLES, screenQuadNumElements, GL_UNSIGNED_SHORT, (void*)screenQuadElementByteOffset);
*/
    // swap the buffers
    glutSwapBuffers();
}

void renderScene ( const GLuint &targetProgram ) {
    // Camera view matrix
    Matrix mViewMatrix = cam.getViewMatrix();
    GLuint mViewMatrixID = glGetUniformLocation(targetProgram, "mViewMatrix");
    glUniformMatrix4fv(mViewMatrixID, 1, GL_TRUE, &mViewMatrix[0][0]);

    // Camera projection matrix
    Matrix mProjMatrix = cam.getProjMatrix();
    GLuint mProjMatrixID = glGetUniformLocation(targetProgram, "mProjMatrix");
    glUniformMatrix4fv(mProjMatrixID, 1, GL_TRUE, &mProjMatrix[0][0]);

    // Transform will be different for the objects, for now just set it up
    Matrix mTransform;
    GLuint mTransformID = glGetUniformLocation(targetProgram, "mTransform");

    //
    // The shape
    //


    // set up textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shape.getDiffTextureID());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shape.getSpecTextureID());

    GLuint textureDiffID = glGetUniformLocation(targetProgram, "material.diffuse");
    glUniform1i(textureDiffID, 0);

    GLuint textureSpecID = glGetUniformLocation(targetProgram, "material.specular");
    glUniform1i(textureSpecID, 1);

    for(int i = 0; i < shapePositions.size(); ++i) {
        glBindVertexArray(vaoShape);

        // transforms
        vector<float> pos = shapePositions[i];

        mTransform = translate(pos[0],pos[1],pos[2]) * rotate(0, zVec) * rotate(180.0f, yVec) * rotate(0, xVec);
        glUniformMatrix4fv(mTransformID, 1, GL_TRUE, &mTransform[0][0]);

        // Drawing elements
        glDrawElements( GL_TRIANGLES, shapeNumElements, GL_UNSIGNED_SHORT, (void*)0);
    }

}

void renderGBufferToQuad (const GLuint &targetProgram) {
    // First the gbuffer textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColorAlb);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);

    GLuint textureIDgPos = glGetUniformLocation(targetProgram, "gPosition");
    glUniform1i(textureIDgPos, 0);

    GLuint textureIDgNorm = glGetUniformLocation(targetProgram, "gNormal");
    glUniform1i(textureIDgNorm, 1);

    GLuint textureIDgColorAlb = glGetUniformLocation(targetProgram, "gColorAlb");
    glUniform1i(textureIDgColorAlb, 2);

    GLuint textureIDgColorSpec = glGetUniformLocation(targetProgram, "gColorSpec");
    glUniform1i(textureIDgColorSpec, 3);

    // Send camera pos
    vector<float> camWorldPos = cam.getCameraPosition();
    GLuint CameraWorldPosID = glGetUniformLocation(targetProgram, "CameraWorldPos");
    glUniform3fv(CameraWorldPosID, 1, &camWorldPos[0]);

    // Illumination
    Lighting light(lightPos, lightIntensityRGB, lightAmbientRGB);
    light.setPhongIllumination( targetProgram );

    //
    // The Shape
    //

    glBindVertexArray(vaoScreenQuad);

    glDrawElements( GL_TRIANGLES, screenQuadNumElements, GL_UNSIGNED_SHORT, (void*)screenQuadElementByteOffset);
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