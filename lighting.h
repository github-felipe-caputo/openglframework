/*
 * lighting.h
 *
 * Set up the lighting.
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#ifndef _LIGHTING_H
#define _LIGHTING_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include "shape.h" // shape is used because of the materials 

/*
 * The lighting class.
 */
class Lighting {
    float lightPosition[3];
    float lightIntensity[3];
    float ambientLight[3];

public:

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
    Lighting(float newLightPosition[], float newLightIntensity[], float newAmbientLight[]);

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
    Lighting(float lightPositionX, float lightPositionY, float lightPositionZ,
             float lightIntensityR, float lightIntensityG, float lightIntensityB, 
             float ambientLightR, float ambientLightG, float ambientLightB);

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
    void setPhongIllumination(GLuint program, Shape shape);

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
    void setPhongIllumination(GLuint program, 
                              float ambientMaterial[], float Ka,
                              float diffuseMaterial[], float Kd,
                              float specularMaterial[], float Ks, float specExp);

};

#endif