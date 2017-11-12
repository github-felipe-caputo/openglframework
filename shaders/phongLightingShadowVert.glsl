#version 410

// Shape values
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

// viewMatrix and projMatrix considering the light point of view
uniform mat4 mLightViewMatrix;
uniform mat4 mLightProjMatrix;

// Phong Illumination values
uniform vec4 lightPos;

// Out values to the fragment shader
out vec3 N;
out vec3 L;
out vec3 V;

// Out value position on light space
out vec4 posLightSpace;

void main () {
    //
    // Vertex positions
    //

    mat4 modelView = mViewMatrix * mTransform;
    gl_Position = mProjMatrix * modelView * vec4(vPosition, 1.0);

    //
    // Vertex position (light space)
    //

    posLightSpace = mLightProjMatrix * mLightViewMatrix * mTransform * vec4(vPosition, 1.0);

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
