#version 410

// Note, this is similar to quadScreenVert.glsl because
// all we do is pass the quad screen data (vertex pos
// and texture coords) to the frag shader, but the
// frag shader used for deffered light is different!

// in values
layout (location = 0) in vec3 vPosition;
in vec2 vTexCoord;

// out values
out vec2 uvTexCoord;

void main() {
    gl_Position = vec4(vPosition.x, vPosition.y, 0.0, 1.0);
    uvTexCoord = vTexCoord;
}