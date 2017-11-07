#version 410

// Shape values
in vec3 vPosition;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

void main () {
    mat4 modelView = mViewMatrix * mTransform;
    gl_Position = mProjMatrix * modelView * vec4(vPosition, 1.0);
}