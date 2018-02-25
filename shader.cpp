/*
 * shader.cpp
 *
 * Set up the shaders, display logs.
 *
 * Based on code from http://www.lighthouse3d.com/
 *
 * Authors: Felipe Victorino Caputo
 *
 */

#include "shader.h"

namespace shader
{

/*
 * readFile
 *
 * INPUT:
 *         filename - the filename of the file we want to read.
 *
 * RETURN:
 *         The contents of the the file in a dinamycally alocated
 *         string of GLchar, or NULL if there was an error.
 *
 * DESCRIPTION:
 *         This function is responsible for reading a text file and
 *         converting the whole content into a single string.
 *
 */
GLchar* readFile(const char* filename) {
    FILE *fp;
    GLchar* fileContent = NULL;
    int fileLength = 0;

    if(filename != NULL) {
        fp = fopen( filename, "r" ); // read file

        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);  // go to end of file to find the length
            fileLength = ftell(fp);
            rewind( fp );            // back to beggining

            fileContent = new GLchar[ sizeof(GLchar) * (fileLength+1) ];

            fileLength = fread( fileContent, sizeof(GLchar), fileLength, fp );
            fileContent[fileLength] = '\0';

            fclose(fp);
        } else {
            perror(filename);
        }
    } else {
        fprintf( stderr, "error: no file name specified\n" );
    }

    return fileContent;
}

/*
 * readFile
 *
 * INPUT:
 *         shaderID - the ID of the shader we want the log.
 *
 *
 * DESCRIPTION:
 *         This function reports the log of the shader that was
 *         compiled.
 *
 */
void printShaderLog(GLuint shaderID) {
    GLint logLength = 0;
    GLsizei numChars = 0;
    char *log;

    // Get length
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength );

    if( logLength > 0 ) {
        log = new char[logLength];

        // Retrieve the log
        glGetShaderInfoLog( shaderID, logLength, &numChars, log );

        // Print it
        if( log[0] != '\0' ) {
            printf( "Shader log:  '%s'\n", log );
        }

        delete[] log;
    }
}

/*
 * makeShaderProgram
 *
 * INPUT:
 *         vert - the filename of vertex shader.
 *         frag - the filename of fragment shader.
 *
 * RETURN:
 *         The ID of the two shaders linked together
 *
 * DESCRIPTION:
 *         This function reads the vertex shader file and the
 *         the fragment shader file, and links those together
 *         into a "program", then its ID is returned.
 *
 */
GLuint makeShaderProgram(const char *vert, const char *frag) {
    GLuint vertID, fragID, programID;
    GLchar *vertContent = NULL, *fragContent = NULL;
    GLint flag = 0;

    // creating the ID of each shader
    vertID = glCreateShader( GL_VERTEX_SHADER );
    fragID = glCreateShader( GL_FRAGMENT_SHADER );

    // Reading the files
    vertContent = readFile(vert);
    if (vertContent == NULL) {
        fprintf( stderr, "error: error reading vertex shader file %s\n", vert);
        return 0;
    }

    fragContent = readFile(frag);
    if (fragContent == NULL) {
        fprintf( stderr, "error: error reading fragment shader file %s\n", frag);
        delete[] vertContent;
        return 0;
    }

    // Binding the source code to the shader
    glShaderSource(vertID, 1, (const GLchar **) &vertContent, NULL);
    glShaderSource(fragID, 1, (const GLchar **) &fragContent, NULL);

    delete[] vertContent;
    delete[] fragContent;

    // Compiling the shaders
    glCompileShader(vertID);
    glGetShaderiv(vertID, GL_COMPILE_STATUS, &flag );
    printShaderLog(vertID);
    if( flag == GL_FALSE ) {
        fprintf( stderr, "error: error compiling vertex shader file %s\n", vert);
        return 0;
    }

    glCompileShader(fragID);
    glGetShaderiv(fragID, GL_COMPILE_STATUS, &flag );
    printShaderLog(fragID);
    if( flag == GL_FALSE ) {
        fprintf( stderr, "error: error compiling fragment shader file %s\n", frag);
        return 0;
    }

    // Linking the shaders into a program
    programID = glCreateProgram();
    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);

    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &flag );
    if( flag == GL_FALSE ) {
        fprintf( stderr, "error: error linking vertex shader file '%s' and fragment shader file %s\n", vert, frag);
        return 0;
    }

    return(programID);
}

} // end namespace