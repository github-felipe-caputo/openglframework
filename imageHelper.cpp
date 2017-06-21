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
GLuint load_bmp(char const* filename) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;

    FILE * file = fopen(filename,"rb");
    if (!file) {
        printf("Image could not be opened: %s \n", filename);
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 ) {
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ) {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

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

GLuint load_png(char const* filename) {
    FILE * file = fopen(filename,"rb");
    if (!file) {
        printf("Image could not be opened: %s \n", filename);
        return 0;
    }

    png_structp pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngStruct) {
        printf("Error reading png struct.\n");
        return 0;
    }

    png_infop pngInfo = png_create_info_struct(pngStruct);
    if (!pngInfo) {
        printf("Error reading png info.\n");
        return 0;
    }

    // Set up error handling, usual method
    if(setjmp(png_jmpbuf(pngStruct))) {
        return 0;
    }

    // control output
    png_init_io(pngStruct, file);
    png_read_info(pngStruct, pngInfo);

    // reads png into memory, forces 8 bit and forces rgb pallete
    //png_read_png(pngStruct, pngInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth;
    int color_type;
    width      = png_get_image_width(pngStruct, pngInfo);
    height     = png_get_image_height(pngStruct, pngInfo);
    color_type = png_get_color_type(pngStruct, pngInfo);
    bit_depth  = png_get_bit_depth(pngStruct, pngInfo);

    if(bit_depth == 16)
        png_set_strip_16(pngStruct);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(pngStruct);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(pngStruct);

    if(png_get_valid(pngStruct, pngInfo, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngStruct);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(pngStruct, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(pngStruct, pngInfo);

    std::cout << width << " " << height << " " << bit_depth << " " << color_type << std::endl;

    png_bytep *row_pointers;
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(pngStruct, pngInfo));
    }

    png_read_image(pngStruct, row_pointers);

    // Clean up
    png_destroy_read_struct(&pngStruct, &pngInfo, NULL);
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, row_pointers);

    // Nice trilinear filtering.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}
