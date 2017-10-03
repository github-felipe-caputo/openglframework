#version 410

// Shape values
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBitangent;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

// Phong Illumination values
uniform vec3 lightPos;

// Out values to the fragment shader
out vec3 L;
out vec3 V;
out vec2 uvTexCoord;
out mat3 TBN;

void main () {
    //
    // Vertex positions
    //

    mat4 modelView = mViewMatrix * mTransform;
    gl_Position = mProjMatrix * modelView * vec4(vPosition, 1.0);

    // texture
    uvTexCoord = vTexCoord;

    // TBN matrix for normal mapping
    vec3 T = normalize(vec3(modelView * vec4(vTangent, 1.0)));
    vec3 B = normalize(vec3(modelView * vec4(vBitangent, 1.0)));
    vec3 N = normalize(vec3(modelView * vec4(vNormal, 1.0)));
    mat3 TBN = mat3(T, B, N);

    //
    // Illumination
    //
    
    // vertex position in camera coord
    vec4 posEyeCoord = modelView * vec4(vPosition, 1.0);

    // light position in camera coord
    // but no transformation (it's not moving like the objects)
    vec4 lightPosEyeCoord = mViewMatrix * vec4(lightPos, 1.0);

    // Calculate the values we will pass to the fragment shader
    L = lightPosEyeCoord.xyz - posEyeCoord.xyz;
    V = -posEyeCoord.xyz;
}
