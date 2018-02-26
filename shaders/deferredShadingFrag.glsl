#version 410

// Text coord of the quad screen
in vec2 uvTexCoord;

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

// Structs
uniform Light light;

// Camera position, world coordinate
uniform vec3 CameraWorldPos;

// out frag
out vec4 fragColor;

void main() {

    vec3 FragPos = texture(gPosition, uvTexCoord).rgb;
    vec3 Normal = texture(gNormal, uvTexCoord).rgb;
    vec3 Albedo = texture(gColorAlbSpec, uvTexCoord).rgb;
    float SpecIntensity = texture(gColorAlbSpec, uvTexCoord).a;

    // Calculate Light
    vec3 viewDir = normalize(CameraWorldPos - FragPos); // everything is in world pos!
    vec3 lightDir = normalize(light.position - FragPos); // everything is in world pos!

    // ambient
    vec3 ambientColor = light.ambient * Albedo;

    // diffuse
    vec3 diffuseColor = light.lightIntensity * max(dot(Normal, lightDir), 0.0) * Albedo;

    // spec
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specularColor = light.lightIntensity * pow(max(dot(Normal, halfwayDir), 0.0), 16.0) * SpecIntensity; // 16, hard coded spec exp
    //if(dot(Normal, lightDir) < 0.0)
    //     specularColor = vec3(0.0,0.0,0.0);

    //fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
    //fragColor = vec4( 0.0, 1.0, 0.0, 1.0 );
    //fragColor = vec4(ambientColor, 1.0);
    fragColor = vec4(diffuseColor, 1.0);
    //fragColor = vec4(specularColor, 1.0);
    //fragColor = vec4(FragPos, 1.0);
}