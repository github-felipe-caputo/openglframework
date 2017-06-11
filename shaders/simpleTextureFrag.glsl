#version 410

in vec2 uvTexCoord;

uniform sampler2D textureSampler;

out vec4 fragColor;

void main () {
    //fragColor = vec4( 0.0, 1.0, 0.0, 1.0 );
    fragColor = texture(textureSampler, uvTexCoord);
}
