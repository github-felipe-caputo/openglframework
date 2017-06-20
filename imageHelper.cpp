/*
 * imageHelper.cpp
 *
 * Helper functioncs.
 *
 * Authors: Felipe Victorino Caputo
 *
 */
#include "imageHelper.h"

// With the help of http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
GLuint load_bmp(char const* Filename) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;

    FILE * file = fopen(Filename,"rb");
    if (!file){printf("Image could not be opened: %s \n", Filename); return 0;}

    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    std::cout << dataPos << " " << imageSize << " " << width << " " << height << std::endl;

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // PROBLEM: not always 3, bitmaps can save alpha information
    if (dataPos==0)      dataPos=54; // The BMP header has size of 54, that DOES NOT MEAN the data comes right after

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    rewind(file);
    fseek(file, dataPos, SEEK_SET);
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

    // Nice trilinear filtering.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}
