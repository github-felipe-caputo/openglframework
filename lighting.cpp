/*
 * lighting.cpp
 *
 * Set up the lighting.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#include "lighting.h"

/* 
 * Lighting
 *
 * INPUT: 
 *         newLightPosition - the light position (x, y, z).
 *         newLightIntensity - the light intensity (r, g, b).
 *         newAmbientLight - tjhe value of the ambient light (r, g, b).
 *
 * DESCRIPTION:
 *         This is one of the constructors of the light class. It will create a
 *         light with position, intensity and ambient intensity.
 *
 */
Lighting::Lighting(float newLightPosition[], float newLightIntensity[], float newAmbientLight[]) {
    for(int i = 0; i < 3; ++i) {
        lightPosition[i] = newLightPosition[i];
        lightIntensity[i] = newLightIntensity[i];
        ambientLight[i] = newAmbientLight[i];
    }
}

/* 
 * Lighting
 *
 * INPUT: 
 *         lightPositionX - the light position (x, y, z).
 *         lightIntensityX - the light intensity (r, g, b).
 *         ambientLightX - tjhe value of the ambient light (r, g, b).
 *
 * DESCRIPTION:
 *         This is one of the constructors of the light class. It will create a
 *         light with position, intensity and ambient intensity. It works like the 
 *         above constructor, but you can use single values instead of arrays.
 *
 */
Lighting::Lighting(float lightPositionX, float lightPositionY, float lightPositionZ,
                   float lightIntensityR, float lightIntensityG, float lightIntensityB, 
                   float ambientLightR, float ambientLightG, float ambientLightB) {

    lightPosition[0] = lightPositionX;
    lightPosition[1] = lightPositionY;
    lightPosition[2] = lightPositionZ;

    lightIntensity[0] = lightIntensityR;
    lightIntensity[1] = lightIntensityG;
    lightIntensity[2] = lightIntensityB;

    ambientLight[0] = ambientLightR;
    ambientLight[1] = ambientLightG;
    ambientLight[2] = ambientLightB;
}

/* 
 * setPhongIllumination
 *
 * INPUT: 
 *         program - the OpenGL shaders that will be used.
 *         shape - the shape object that will be affected by this light
 *
 * DESCRIPTION:
 *         This function will set the phong illumination model for a certain shape.
 *         Some values are calculated here, but some will be calculated on the
 *         shaders.
 *
 *         Keep in mind that this model uses the Hammond product of vectors 
 *         (component wise product).
 *
 *         Check the shaders flatLighting and phongLighting to see examples of
 *         how they should look.
 *
 */
void Lighting::setPhongIllumination(GLuint program, Shape shape) {
    float lightPos[] = {lightPosition[0], lightPosition[1], lightPosition[2], 1.0f};

    float* ambientMaterial = shape.getAmbientMaterial();
    float* diffuseMaterial = shape.getDiffuseMaterial();
    float* specularMaterial = shape.getSpecularMaterial();

    float Ka = shape.getKa();
    float Kd = shape.getKd();
    float Ks = shape.getKs();
    float specExp = shape.getSpecExp();

    // Here we make the canculations with the light, material and coefficient
    // se we don't have to do it on the shader (we will just have to use the calculations
    // with the vectors on the shaders)
    float ambient[] = { ambientLight[0] * ambientMaterial[0] * Ka,
                        ambientLight[1] * ambientMaterial[1] * Ka,
                        ambientLight[2] * ambientMaterial[2] * Ka,
                        1.0f};

    float diffuse[] = { lightIntensity[0] * diffuseMaterial[0] * Kd,
                        lightIntensity[1] * diffuseMaterial[1] * Kd,
                        lightIntensity[2] * diffuseMaterial[2] * Kd,
                        1.0f};    

    float specular[] = { lightIntensity[0] * specularMaterial[0] * Ks,
                         lightIntensity[1] * specularMaterial[1] * Ks,
                         lightIntensity[2] * specularMaterial[2] * Ks,
                         1.0f};

    // Sending the necessary data
    GLuint lightPosID = glGetUniformLocation(program, "lightPos");
    glUniform4fv(lightPosID, 1, lightPos);

    GLuint ambientID = glGetUniformLocation(program, "ambient");
    glUniform4fv(ambientID, 1, ambient);

    GLuint diffuseID = glGetUniformLocation(program, "diffuse");
    glUniform4fv(diffuseID, 1, diffuse);

    GLuint specularID = glGetUniformLocation(program, "specular");
    glUniform4fv(specularID, 1, specular);

    GLuint specExpID = glGetUniformLocation(program, "specExp");
    glUniform1f(specExpID, specExp);
}

/* 
 * setPhongIllumination
 *
 * INPUT: 
 *         program - the OpenGL shaders that will be used.
 *         ambientMaterial - RGB of the ambient material.
 *         Ka - ambient coefficient.
 *         diffuseMaterial - RGB of the diffuse material.
 *         Kd - diffuse coefficient.
 *         specularMaterial - RGB of the specular material.
 *         Ks - specular coefficient.
 *         specExp - specular expoent.
 *
 * DESCRIPTION:
 *         This function will set the phong illumination model, however its
 *         use is different from the above function. Here you don't use a shape
 *         as an argument, because it is supposed to be used as a generic 
 *         light for all the objects, that way it's not necessary to set up
 *         materials for every shape, it can be set once in with this function and
 *         then used for a number of shapes.
 *
 *         Keep in mind that this model uses the Hammond product of vectors 
 *         (component wise product).
 *
 *         Check the shaders flatLighting and phongLighting to see examples of
 *         how they should look.
 *
 */
void Lighting::setPhongIllumination(GLuint program,
                                    float ambientMaterial[], float Ka,
                                    float diffuseMaterial[], float Kd,
                                    float specularMaterial[], float Ks, float specExp) {

    // Here we make the canculations with the light, material and coefficient
    // se we don't have to do it on the shader (we will just have to use the calculations
    // with the vectors on the shaders)
    float ambient[] = { ambientLight[0] * ambientMaterial[0] * Ka,
                        ambientLight[1] * ambientMaterial[1] * Ka,
                        ambientLight[2] * ambientMaterial[2] * Ka,
                        1.0f};

    float diffuse[] = { lightIntensity[0] * diffuseMaterial[0] * Kd,
                        lightIntensity[1] * diffuseMaterial[1] * Kd,
                        lightIntensity[2] * diffuseMaterial[2] * Kd,
                        1.0f};    

    float specular[] = { lightIntensity[0] * specularMaterial[0] * Ks,
                         lightIntensity[1] * specularMaterial[1] * Ks,
                         lightIntensity[2] * specularMaterial[2] * Ks,
                         1.0f};


    // Sending the necessary data
    GLuint lightPosID = glGetUniformLocation(program, "lightPos");
    glUniform4fv(lightPosID, 1, lightPosition);

    GLuint ambientID = glGetUniformLocation(program, "ambient");
    glUniform4fv(ambientID, 1, ambient);

    GLuint diffuseID = glGetUniformLocation(program, "diffuse");
    glUniform4fv(diffuseID, 1, diffuse);

    GLuint specularID = glGetUniformLocation(program, "specular");
    glUniform4fv(specularID, 1, specular);

    GLuint specExpID = glGetUniformLocation(program, "specExp");
    glUniform1f(specExpID, specExp);
}

