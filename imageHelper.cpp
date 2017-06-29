/*
 * imageHelper.cpp
 *
 * Helper functioncs.
 *
 * Authors: Felipe Victorino Caputo
 *
 */
#include "imageHelper.h"

/*
 * load_bmp
 *
 * INPUT:
 *         Filename - path to the texture file.
 *
 * RETURN:
 *         The textureID of the file used as argument.
 *
 * DESCRIPTION:
 *         This function is responsible for loading a texture into
 *         OpenGL with trilinear filtering and returning the texture ID.
 *         Reads a bmp file.
 *
 *         With the help of
 *         http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
 */
GLuint load_bmp(char const* filename) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;

    FILE * file = fopen(filename,"rb");
    if (!file) {
        fprintf(stderr, "Image could not be opened: %s \n", filename);
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 ) {
        fprintf(stderr, "Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ) {
        fprintf(stderr, "Not a correct BMP file\n");
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

    // clean up
    free(data);

    return textureID;
}

/*
 * load_png
 *
 * INPUT:
 *         Filename - path to the texture file.
 *
 * RETURN:
 *         The textureID of the file used as argument.
 *
 * DESCRIPTION:
 *         This function is responsible for loading a texture into
 *         OpenGL with trilinear filtering and returning the texture ID.
 *         Reads a png file.
 *
 *         In particular this function will change the png file into one with
 *         8 bits and with an alpha channel (if the image does not have one
 *         it will fill it with 0xFF).
 *
 *         With the help of
 *         https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example
 *         http://www.libpng.org/pub/png/libpng-manual.txt
 */
GLuint load_png(char const* filename) {
    png_byte header[8];

    FILE * file = fopen(filename,"rb");
    if (!file) {
        printf("Image could not be opened: %s \n", filename);
        return 0;
    }

    // read the header
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "error: %s is not a PNG.\n", filename);
        fclose(file);
        return 0;
    }

    png_structp pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngStruct) {
        fprintf(stderr, "error: reading png struct.\n");
        fclose(file);
        return 0;
    }

    png_infop pngInfo = png_create_info_struct(pngStruct);
    if (!pngInfo) {
        fprintf(stderr, "error: reading png info.\n");
        png_destroy_read_struct(&pngStruct, &pngInfo, NULL);
        fclose(file);
        return 0;
    }

    // Set up error handling, usual method
    if(setjmp(png_jmpbuf(pngStruct))) {
        png_destroy_read_struct(&pngStruct, &pngInfo, NULL);
        fclose(file);
        return 0;
    }

    // control output
    png_init_io(pngStruct, file);
    png_set_sig_bytes(pngStruct, 8);
    png_read_info(pngStruct, pngInfo);

    png_uint_32 width   = png_get_image_width(pngStruct, pngInfo);
    png_uint_32 height  = png_get_image_height(pngStruct, pngInfo);
    int color_type      = png_get_color_type(pngStruct, pngInfo);
    int bit_depth       = png_get_bit_depth(pngStruct, pngInfo);

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

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(pngStruct);

    // Update our info
    png_read_update_info(pngStruct, pngInfo);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(pngStruct, pngInfo);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte* image_data = (png_byte*)malloc(rowbytes * height * sizeof(png_byte));
    if (image_data == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&pngStruct, &pngInfo, NULL);
        fclose(file);
        return 0;
    }

    // Row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep* row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
    if (row_pointers == NULL) {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&pngStruct, &pngInfo, NULL);
        fclose(file);
        free(image_data);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    // that's because OpenGL expects the data from bottom to top so we swap it,
    // row_pointers[last] = image_data[first]
    // row_pointers[second to last] = image_data[second]
    // ...
    for (int i = 0; i < height; i++) {
        row_pointers[height - 1 - i] = image_data + i * rowbytes;
    }

    png_read_image(pngStruct, row_pointers);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    // Nice trilinear filtering.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean up
    png_destroy_read_struct(&pngStruct, &pngInfo, NULL);
    fclose(file);
    free(image_data);
    free(row_pointers);

    return textureID;
}
