#version 410

// Shape values
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

// Out values to the fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 uvTexCoord;

void main () {
    // Position to the frag shader
    vec4 worldPos = mTransform * vec4(vPosition, 1.0);
    FragPos = worldPos.xyz;

    // texture to frag shader
    uvTexCoord = vTexCoord;

    // Calculate the normal values we will pass to the frag shader
    mat3 normalMatrix = mat3(transpose(inverse(mTransform)));
    Normal = normalMatrix * vNormal;

    gl_Position = mProjMatrix * mViewMatrix * worldPos;
}
