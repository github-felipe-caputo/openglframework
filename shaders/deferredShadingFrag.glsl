#version 410

// Text coord of the quad screen
in vec2 vTexCoord;

// Textures from gBuffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorAlbSpec;

// Light values
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 lightIntensity;
};

// out frag
out vec4 fragColor;

void main() {
    // TODO: the whole the shader :)
    fragColor = vec4( 0.0, 1.0, 0.0, 1.0 );
}