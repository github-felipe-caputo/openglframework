#version 410

// Out gbuffer values
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorAlbSpec;

// In values from the fragment shader
in vec3 FragPos;
in vec3 Normal;
in vec2 uvTexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
};

// struct
uniform Material material;

void main () {
    // Position on the g buffer
    gPosition = FragPos;

    // Normal on the g buffer
    gNormal = normalize(Normal);

    // Colors come from separated textures, but we add them on the same
    // g buffer texture
    gColorAlbSpec.rgb = texture(material.diffuse, uvTexCoord).rgb;
    gColorAlbSpec.a = texture(material.specular, uvTexCoord).r;
}
