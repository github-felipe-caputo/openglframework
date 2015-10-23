#version 130

// Shape values
in vec3 vPosition;
in vec3 vNormal;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

// Phong Illumination values
uniform vec4 lightPos;

// Out values to the fragment shader
varying vec3 N;
varying vec3 L;
varying vec3 V;

void main () {
    //
    // Vertex positions
    //

    mat4 modelView = mViewMatrix * mTransform;
    gl_Position = mProjMatrix * modelView * vec4(vPosition, 1.0);

    //
    // Illumination
    //

    mat3 normalMatrix = mat3(transpose(inverse(modelView)));

    // vertex position in camera coord
    vec4 posEyeCoord = modelView * vec4(vPosition, 1.0); 

    // light position in camera coord
    // but no transformation (it's not moving like the objects)
    vec4 lightPosEyeCoord = mViewMatrix * lightPos;

    // Calculate the values we will pass to the fragment shader
    N = normalMatrix * vNormal;
    L = lightPosEyeCoord.xyz - posEyeCoord.xyz;
    V = -posEyeCoord.xyz;
}