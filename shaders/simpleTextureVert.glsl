#version 410

in vec3 vPosition;
in vec2 vTexCoord;

uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

out vec2 uvTexCoord;

void main () {
    gl_Position = mProjMatrix * mViewMatrix * mTransform * vec4(vPosition, 1.0);
    uvTexCoord = vTexCoord;
}
