#version 410

// Shape values
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorAlbSpec;

// In values from  the fragment shader
in vec3 FragPos;
in vec3 Normal;
in vec2 uvTexCoord;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main () {
    // Position on the g buffer
    gPosition = FragPos;

    // Normal on the g buffer
    gNormal = normalize(Normal);

    // Colors come from separated textures, but we add them on the same
    // g buffer texture
    gColorAlbSpec.rgb = texture(texture_diffuse, uvTexCoord).rgb;
    gColorAlbSpec.a = texture(texture_specular, uvTexCoord).r;
}
