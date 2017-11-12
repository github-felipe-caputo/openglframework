#version 410

// Light values
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 lightIntensity;
};

// Phong Illumination values
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     specExp;
};

// "in" values from vertex shader
in vec3 N;
in vec3 L;
in vec3 V;

// in for texture
in vec2 uvTexCoord;

// uut frag
out vec4 fragColor;

// structs
uniform Material material;
uniform Light light;

void main () {
    vec3 nN = normalize(N);
    vec3 nL = normalize(L);
    vec3 nV = normalize(V);
    vec3 R = reflect(-nL,nN);

    vec3 diffText = vec3(texture(material.diffuse, uvTexCoord));
    vec3 specText = vec3(texture(material.specular, uvTexCoord));

    vec3 ambientColor = light.ambient * diffText;
    vec3 diffuseColor = light.lightIntensity * max(dot(nN, nL), 0.0) * diffText;
    vec3 specularColor = light.lightIntensity * pow(max(dot(R, nV),0.0), material.specExp) * specText ;
    if(dot(nL, nN) < 0.0)
        specularColor = vec3(0.0,0.0,0.0);

    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
    //fragColor = vec4( 0.0, 1.0, 0.0, 1.0 );
    //fragColor = vec4(ambientColor, 1.0);
    //fragColor = vec4(diffuseColor, 1.0);
    //fragColor = vec4(specularColor, 1.0);
}
