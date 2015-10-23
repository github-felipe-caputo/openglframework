#version 130

// Shape values
in vec3 vPosition;
in vec3 vNormal;

// ModelView and Projection values
uniform mat4 mTransform;
uniform mat4 mViewMatrix;
uniform mat4 mProjMatrix;

// Phong Illumination values
uniform vec4 lightPos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float specExp; 

// Out values to the fragment shader
out vec4 color;

void main () {
    //
    // Vertex positions
    //

    mat4 modelView = mViewMatrix * mTransform;
    gl_Position = mProjMatrix * modelView * vec4(vPosition, 1.0);

    //
    // Illumination
    //

    mat3 normalMatrix = mat3(transpose(inverse(modelView)));

    // vertex position in camera coord
    vec4 posEyeCoord = modelView * vec4(vPosition, 1.0); 

    // light position in camera coord
    // but no transformation (it's not moving like the objects)
    vec4 lightPosEyeCoord = mViewMatrix * lightPos;

    vec3 N = normalize(normalMatrix * vNormal);
    vec3 L = normalize(lightPosEyeCoord.xyz - posEyeCoord.xyz);
    vec3 V = normalize(-posEyeCoord.xyz);
    vec3 R = reflect(-L,N);

    vec4 ambientColor = ambient;
    vec4 diffuseColor = diffuse * max(dot(N, L), 0.0);
    vec4 specularColor = specular * pow(max(dot(R, V),0.0),specExp);
    if(dot(L, N) < 0.0) 
        specularColor = vec4(0.0,0.0,0.0,1.0);

    color = ambientColor + diffuseColor + specularColor;
}