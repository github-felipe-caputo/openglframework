#version 410

// in values
in vec2 uvTexCoord;

// out frag
out vec4 fragColor;

// uniform
uniform sampler2D screenTexture;

void main () {
    float depthValue = texture(screenTexture, uvTexCoord).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}