#version 130

in vec3 vPosition;
in vec4 vColor;

uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

out vec4 color;

void main () {
    gl_Position = mProjMatrix * mViewMatrix * mTransform * vec4(vPosition, 1.0);
    color = vColor;
}