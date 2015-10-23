#version 130

// Phong Illumination values
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float specExp; 

// "in" values from vertex shader
varying vec3 N;
varying vec3 L;
varying vec3 V;

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

    gl_FragColor = ambientColor + diffuseColor + specularColor;
}