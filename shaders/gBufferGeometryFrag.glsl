#version 410

// Out gbuffer values
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gColorAlb;
layout (location = 3) out vec3 gColorSpec;

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

    // g buffer texture
    gColorAlb = texture(material.diffuse, uvTexCoord).rgb;
    gColorSpec = texture(material.specular, uvTexCoord).rgb;
}
