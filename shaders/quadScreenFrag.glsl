#version 410

// in values
in vec2 uvTexCoord;

// out frag
out vec4 fragColor;

// uniform
uniform sampler2D screenTexture;

void main () {
    fragColor = texture(screenTexture, uvTexCoord);
}