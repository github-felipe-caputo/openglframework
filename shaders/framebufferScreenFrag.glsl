#version 410

// in values
in vec2 uvTexCoord;

// out frag
out vec4 fragColor;

// uniform
uniform sampler2D screenTexture;

void main () {
    fragColor = texture(screenTexture, uvTexCoord);
    //fragColor = vec4( 0.0, 1.0, 0.0, 1.0 );
}