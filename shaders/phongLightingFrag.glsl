#version 410

// Phong Illumination values
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float specExp;

// "in" values from vertex shader
in vec3 N;
in vec3 L;
in vec3 V;

out vec4 fragColor;

void main () {
    vec3 nN = normalize(N);
    vec3 nL = normalize(L);
    vec3 nV = normalize(V);
    vec3 R = reflect(-nL,nN);

    vec4 ambientColor = ambient;
    vec4 diffuseColor = diffuse * max(dot(nN, nL), 0.0);
    vec4 specularColor = specular * pow(max(dot(R, nV),0.0),specExp);
    if(dot(nL, nN) < 0.0)
        specularColor = vec4(0.0,0.0,0.0,1.0);

    fragColor = ambientColor + diffuseColor + specularColor;
}
