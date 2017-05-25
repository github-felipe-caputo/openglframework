#version 410

in vec2 uvTexCoord;

uniform sampler2D texture;

void main () {
    //gl_FragColor = vec4( 0.0, 1.0, 0.0, 1.0 );
    gl_FragColor = texture2D(texture, uvTexCoord);
}
