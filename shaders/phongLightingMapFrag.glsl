#version 410

// Light values
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 lightIntensity;
};

// Phong Illumination values
uniform vec4 ambient;
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

out vec4 fragColor;

void main () {
    vec3 nN = normalize(N);
    vec3 nL = normalize(L);
    vec3 nV = normalize(V);
    vec3 R = reflect(-nL,nN);

    vec3 diffText = vec3(texture(material.diffuse, uvTexCoord));  
    vec3 specText = vec3(texture(material.specular, uvTexCoord));  

    vec4 ambientColor = ambient;
    vec4 diffuseColor = diffText * max(dot(nN, nL), 0.0);
    vec4 specularColor = specText * pow(max(dot(R, nV),0.0),specExp);
    if(dot(nL, nN) < 0.0)
        specularColor = vec4(0.0,0.0,0.0,1.0);

    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}
