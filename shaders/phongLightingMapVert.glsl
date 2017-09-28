#version 410

// Shape values
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

// Phong Illumination values
uniform vec3 lightPos;

// Out values to the fragment shader
out vec3 N;
out vec3 L;
out vec3 V;
out vec2 uvTexCoord;

void main () {
    //
    // Vertex positions
    //

    mat4 modelView = mViewMatrix * mTransform;
    gl_Position = mProjMatrix * modelView * vec4(vPosition, 1.0);

    // texture
    uvTexCoord = vTexCoord;

    //
    // Illumination
    //

    mat3 normalMatrix = mat3(transpose(inverse(modelView)));

    // vertex position in camera coord
    vec4 posEyeCoord = modelView * vec4(vPosition, 1.0);

    // light position in camera coord
    // but no transformation (it's not moving like the objects)
    vec4 lightPosEyeCoord = mViewMatrix * vec4(lightPos, 1.0);

    // Calculate the values we will pass to the fragment shader
    N = normalMatrix * vNormal;
    L = lightPosEyeCoord.xyz - posEyeCoord.xyz;
    V = -posEyeCoord.xyz;
}
